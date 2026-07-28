#!/usr/bin/env python3
"""Explain one real Caxecraft Haxe edit from frontend rebuilds to generated C.

This diagnostic copies only Caxecraft's Haxe source into a temporary directory,
changes one private combat constant, and submits the before/after requests to
one owned Haxe compilation server with one stable output root. It then compares
Haxe's rebuilt class closure, canonical HxcIR functions/modules, semantic
sidecars, and every generated artifact.

The report deliberately stops at generated C. Directly changed ``.c`` files are
listed as candidate native objects, but header dependency files and real object
keys are not guessed. The existing native cache owns that later proof.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable, Mapping, Sequence
from dataclasses import dataclass
from pathlib import Path


CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
sys.path.insert(0, str(CASE))
from profile_compiler import (  # noqa: E402
    CompilerProfileFailure,
    compiler_revision,
    host_condition,
    load_average,
    run_observed_sample,
    timer_clock,
    workload_arguments,
)
from run import (  # noqa: E402
    CaxecraftFailure,
    assert_artifacts_equal,
    haxe_compilation_server,
    normal_artifacts,
    pinned_haxe_installation,
    verify_pinned_haxe,
    write_timing_report,
)


INCREMENTAL_PREFIX = "HXC_INCREMENTAL_INPUT="
STATIC_INITIALIZATION_PREFIX = "HXC_STATIC_INITIALIZATION="
SOURCE_RELATIVE_PATH = Path("caxecraft/domain/Vitals.hx")
EDIT_BEFORE = "inline final ATTACK_SAFE_TICKS:Int = 20;"
EDIT_AFTER = "inline final ATTACK_SAFE_TICKS:Int = 21;"
FUNCTION_START = re.compile(
    r'^  function (?P<identifier>"(?:[^"\\\\]|\\\\.)*") '
)
MODULE_START = re.compile(r'^module (?P<identifier>"(?:[^"\\\\]|\\\\.)*") ')
SIDECAR_PATHS = (
    "hxc.abi.json",
    "hxc.dispatch.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.specializations.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
)


class IncrementalEditProfileFailure(RuntimeError):
    """The edit probe could not establish complete deterministic evidence."""


@dataclass(frozen=True)
class TextSection:
    """One named canonical HxcIR section and its exact text digest."""

    identifier: str
    text: str

    @property
    def sha256(self) -> str:
        """Return the section's exact UTF-8 SHA-256 identity."""

        return hashlib.sha256(self.text.encode("utf-8")).hexdigest()


def prefixed_json(
    stdout: str, prefix: str, label: str
) -> dict[str, object]:
    """Decode exactly one compiler report with a known line prefix."""

    values = [
        line[len(prefix) :]
        for line in stdout.splitlines()
        if line.startswith(prefix)
    ]
    if len(values) != 1:
        raise IncrementalEditProfileFailure(
            f"{label} emitted {len(values)} records, expected exactly one"
        )
    try:
        value = json.loads(values[0])
    except json.JSONDecodeError as error:
        raise IncrementalEditProfileFailure(
            f"{label} emitted malformed JSON: {error}"
        ) from error
    if not isinstance(value, dict):
        raise IncrementalEditProfileFailure(
            f"{label} must be a JSON object"
        )
    return value


def hxcir_from_stdout(stdout: str, label: str) -> str:
    """Extract the canonical HxcIR text from the existing inspection report."""

    report = prefixed_json(
        stdout, STATIC_INITIALIZATION_PREFIX, f"{label} HxcIR"
    )
    if report.get("schemaVersion") != 1 or not isinstance(
        report.get("hxcir"), str
    ):
        raise IncrementalEditProfileFailure(
            f"{label} HxcIR report schema drifted"
        )
    return str(report["hxcir"])


def decoded_identifier(token: str, label: str) -> str:
    """Decode one quoted HxcIR identifier through JSON's exact string grammar."""

    try:
        value = json.loads(token)
    except json.JSONDecodeError as error:
        raise IncrementalEditProfileFailure(
            f"{label} contains an invalid quoted identifier"
        ) from error
    if not isinstance(value, str):
        raise IncrementalEditProfileFailure(
            f"{label} identifier is not text"
        )
    return value


def split_hxcir_sections(
    hxcir: str,
    *,
    start: re.Pattern[str],
    end_prefix: str,
    label: str,
) -> tuple[dict[str, TextSection], str]:
    """Split canonical named blocks while retaining the exact outer skeleton.

    The skeleton contains every line outside the selected sections. For
    functions, it therefore retains dispatch, types, instances, globals, and
    module ownership; for modules, it retains the schema and dispatch plan.
    """

    lines = hxcir.splitlines(keepends=True)
    sections: dict[str, TextSection] = {}
    skeleton: list[str] = []
    index = 0
    while index < len(lines):
        match = start.match(lines[index].rstrip("\n"))
        if match is None:
            skeleton.append(lines[index])
            index += 1
            continue
        identifier_token = match.group("identifier")
        identifier = decoded_identifier(identifier_token, f"{label} start")
        if identifier in sections:
            raise IncrementalEditProfileFailure(
                f"{label} repeats {identifier!r}"
            )
        expected_end = f"{end_prefix}{identifier_token}"
        section_lines = [lines[index]]
        index += 1
        while index < len(lines):
            section_lines.append(lines[index])
            current = lines[index].rstrip("\n")
            index += 1
            if current == expected_end:
                break
        else:
            raise IncrementalEditProfileFailure(
                f"{label} {identifier!r} has no closing line"
            )
        sections[identifier] = TextSection(
            identifier, "".join(section_lines)
        )
    return sections, "".join(skeleton)


def hxcir_function_sections(
    hxcir: str,
) -> tuple[dict[str, TextSection], str]:
    """Return every HxcIR function and the non-function semantic skeleton."""

    return split_hxcir_sections(
        hxcir,
        start=FUNCTION_START,
        end_prefix="  end function ",
        label="HxcIR function",
    )


def hxcir_module_sections(
    hxcir: str,
) -> tuple[dict[str, TextSection], str]:
    """Return every HxcIR module and the schema/dispatch outer skeleton."""

    return split_hxcir_sections(
        hxcir,
        start=MODULE_START,
        end_prefix="end module ",
        label="HxcIR module",
    )


def digest_text(value: str) -> str:
    """Hash exact canonical text without retaining it in the report."""

    return hashlib.sha256(value.encode("utf-8")).hexdigest()


def section_diff(
    before: Mapping[str, TextSection],
    after: Mapping[str, TextSection],
) -> dict[str, object]:
    """Classify exact named-section identities without inferring semantics."""

    before_ids = set(before)
    after_ids = set(after)
    shared = before_ids.intersection(after_ids)
    changed = sorted(
        identifier
        for identifier in shared
        if before[identifier].sha256 != after[identifier].sha256
    )
    unchanged = sorted(shared.difference(changed))
    return {
        "added": sorted(after_ids - before_ids),
        "removed": sorted(before_ids - after_ids),
        "changed": changed,
        "unchanged": unchanged,
        "beforeCount": len(before),
        "afterCount": len(after),
    }


def section_line_differences(
    before: Mapping[str, TextSection],
    after: Mapping[str, TextSection],
    identifiers: Sequence[str],
    *,
    limit: int = 40,
) -> list[dict[str, object]]:
    """Expose bounded canonical line changes for sections already known to differ."""

    result: list[dict[str, object]] = []
    for identifier in identifiers:
        old = before.get(identifier)
        new = after.get(identifier)
        if old is None or new is None:
            continue
        lines = [
            line
            for line in difflib.ndiff(
                old.text.splitlines(), new.text.splitlines()
            )
            if line.startswith("- ") or line.startswith("+ ")
        ]
        result.append(
            {
                "id": identifier,
                "changedLineCount": len(lines),
                "lines": lines[:limit],
                "truncated": len(lines) > limit,
            }
        )
    return result


def artifact_diff(
    before: Mapping[str, bytes],
    after: Mapping[str, bytes],
) -> dict[str, object]:
    """Compare the complete generated artifact set by path and exact bytes."""

    before_paths = set(before)
    after_paths = set(after)
    shared = before_paths.intersection(after_paths)
    changed = sorted(
        path for path in shared if before[path] != after[path]
    )
    unchanged = sorted(shared.difference(changed))
    return {
        "added": sorted(after_paths - before_paths),
        "removed": sorted(before_paths - after_paths),
        "changed": changed,
        "unchanged": unchanged,
        "beforeCount": len(before),
        "afterCount": len(after),
    }


def selected_artifact_status(
    before: Mapping[str, bytes],
    after: Mapping[str, bytes],
    paths: Sequence[str],
) -> list[dict[str, object]]:
    """Report whether each named semantic sidecar was absent, reused, or changed."""

    result: list[dict[str, object]] = []
    for path in paths:
        old = before.get(path)
        new = after.get(path)
        if old is None and new is None:
            status = "absent"
        elif old is None:
            status = "added"
        elif new is None:
            status = "removed"
        elif old == new:
            status = "reused-exact-bytes"
        else:
            status = "changed"
        result.append({"path": path, "status": status})
    return result


def replace_source_classpath(
    arguments: Sequence[str], source_root: Path
) -> tuple[str, ...]:
    """Replace Caxecraft's first relative `src` classpath with the copied source."""

    result = list(arguments)
    replacements = 0
    for index in range(len(result) - 1):
        if result[index] == "-cp" and result[index + 1] == "src":
            result[index + 1] = str(source_root)
            replacements += 1
    if replacements != 1:
        raise IncrementalEditProfileFailure(
            "resolved Caxecraft arguments did not contain one source classpath"
        )
    result.extend(
        (
            "-D",
            "reflaxe_c_incremental_input_report",
            "-D",
            "reflaxe_c_static_initialization_report",
        )
    )
    return tuple(result)


def compact_sample(sample: Mapping[str, object]) -> dict[str, object]:
    """Keep timings useful without copying the multi-megabyte span stream."""

    return {
        "wallDurationMs": sample.get("wallDurationMs"),
        "host": sample.get("host"),
        "phases": sample.get("phases"),
        "details": sample.get("details"),
        "exclusiveAccounting": sample.get("exclusiveAccounting"),
    }


def profile_incremental_edit() -> dict[str, object]:
    """Run the fixed Vitals edit and return path-free compiler evidence."""

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    clock = timer_clock(installation)
    logical_cpus = os.cpu_count() or 1
    starting_load = load_average()
    with tempfile.TemporaryDirectory(
        prefix="hxc-caxecraft-incremental-edit-"
    ) as temporary:
        temporary_root = Path(temporary)
        source_root = temporary_root / "src"
        output = temporary_root / "generated"
        shutil.copytree(CASE / "src", source_root)
        arguments = replace_source_classpath(
            workload_arguments(output, "playable"), source_root
        )
        source = source_root / SOURCE_RELATIVE_PATH
        original = source.read_text(encoding="utf-8")
        if original.count(EDIT_BEFORE) != 1 or EDIT_AFTER in original:
            raise IncrementalEditProfileFailure(
                "the Vitals edit anchor is missing or ambiguous"
            )

        with haxe_compilation_server() as connection:
            print(
                "caxecraft-incremental-edit: [cold cache prime]",
                flush=True,
            )
            prime = run_observed_sample(
                installation,
                arguments,
                clock,
                connection=connection,
                timeout=360,
            )
            prime_artifacts = normal_artifacts(output)
            prime_hxcir = hxcir_from_stdout(prime.stdout, "cold prime")
            print(
                "caxecraft-incremental-edit: [warm unchanged baseline]",
                flush=True,
            )
            baseline = run_observed_sample(
                installation,
                arguments,
                clock,
                connection=connection,
                timeout=360,
            )
            before_artifacts = normal_artifacts(output)
            assert_artifacts_equal(
                prime_artifacts,
                before_artifacts,
                "incremental profile cold/warm baseline",
            )
            before_hxcir = hxcir_from_stdout(
                baseline.stdout, "baseline"
            )
            source.write_text(
                original.replace(EDIT_BEFORE, EDIT_AFTER, 1),
                encoding="utf-8",
            )
            print(
                "caxecraft-incremental-edit: [one-module edit request]",
                flush=True,
            )
            edited = run_observed_sample(
                installation,
                arguments,
                clock,
                connection=connection,
                timeout=360,
            )
            after_artifacts = normal_artifacts(output)
            after_hxcir = hxcir_from_stdout(edited.stdout, "edited")

        prime_input = prefixed_json(
            prime.stdout, INCREMENTAL_PREFIX, "prime incremental input"
        )
        baseline_input = prefixed_json(
            baseline.stdout, INCREMENTAL_PREFIX, "baseline incremental input"
        )
        edited_input = prefixed_json(
            edited.stdout, INCREMENTAL_PREFIX, "edited incremental input"
        )
        before_functions, before_function_skeleton = hxcir_function_sections(
            before_hxcir
        )
        after_functions, after_function_skeleton = hxcir_function_sections(
            after_hxcir
        )
        before_modules, before_module_skeleton = hxcir_module_sections(
            before_hxcir
        )
        after_modules, after_module_skeleton = hxcir_module_sections(
            after_hxcir
        )
        prime_functions, prime_function_skeleton = hxcir_function_sections(
            prime_hxcir
        )
        prime_modules, prime_module_skeleton = hxcir_module_sections(
            prime_hxcir
        )
        generated = artifact_diff(before_artifacts, after_artifacts)
        function_difference = section_diff(
            before_functions, after_functions
        )
        module_difference = section_diff(before_modules, after_modules)
        changed_c_sources = [
            path
            for path in generated["changed"]
            if isinstance(path, str) and path.endswith(".c")
        ]
        changed_headers = [
            path
            for path in generated["changed"]
            if isinstance(path, str) and path.endswith(".h")
        ]
        report: dict[str, object] = {
            "schemaVersion": 1,
            "suite": "caxecraft-incremental-edit-profile",
            "boundary": "haxe-source-to-generated-c",
            "edit": {
                "source": SOURCE_RELATIVE_PATH.as_posix(),
                "kind": "implementation-constant",
                "publicTypeChanged": False,
            },
            "compiler": compiler_revision(),
            "haxe": {
                "version": installation.version,
                "transport": "one-owned-loopback-server",
            },
            "frontend": {
                "prime": prime_input,
                "baseline": baseline_input,
                "edited": edited_input,
            },
            "hxcir": {
                "functions": function_difference,
                "modules": module_difference,
                "changedModuleLines": section_line_differences(
                    before_modules,
                    after_modules,
                    module_difference["changed"],
                ),
                "nonFunctionSkeleton": {
                    "beforeSha256": digest_text(
                        before_function_skeleton
                    ),
                    "afterSha256": digest_text(after_function_skeleton),
                    "changed": (
                        before_function_skeleton
                        != after_function_skeleton
                    ),
                },
                "schemaAndDispatchSkeleton": {
                    "beforeSha256": digest_text(before_module_skeleton),
                    "afterSha256": digest_text(after_module_skeleton),
                    "changed": before_module_skeleton
                    != after_module_skeleton,
                },
                "coldToWarmUnchanged": {
                    "normalArtifacts": "byte-identical",
                    "functions": section_diff(
                        prime_functions, before_functions
                    ),
                    "modules": section_diff(
                        prime_modules, before_modules
                    ),
                    "nonFunctionSkeletonChanged": (
                        prime_function_skeleton
                        != before_function_skeleton
                    ),
                    "schemaAndDispatchSkeletonChanged": (
                        prime_module_skeleton != before_module_skeleton
                    ),
                },
            },
            "generatedArtifacts": generated,
            "semanticSidecars": selected_artifact_status(
                before_artifacts, after_artifacts, SIDECAR_PATHS
            ),
            "castTranslationUnits": {
                "changed": changed_c_sources,
                "unchanged": [
                    path
                    for path in generated["unchanged"]
                    if isinstance(path, str) and path.endswith(".c")
                ],
            },
            "nativeProjection": {
                "measured": False,
                "directlyChangedSourceCandidates": changed_c_sources,
                "changedGeneratedHeaders": changed_headers,
                "reason": (
                    "real object reuse requires compiler depfiles, native "
                    "toolchain identities, and the existing native-cache owner"
                ),
            },
            "timing": {
                "prime": compact_sample(prime.sample),
                "baseline": compact_sample(baseline.sample),
                "edited": compact_sample(edited.sample),
            },
        }
        ending_load = load_average()
        report["host"] = {
            "platform": sys.platform,
            "machine": platform.machine(),
            "processor": platform.processor(),
            "logicalCpuCount": logical_cpus,
            "loadAverageOneMinuteStart": starting_load,
            "loadAverageOneMinuteEnd": ending_load,
            "condition": host_condition(
                (starting_load, ending_load), logical_cpus
            ),
        }
        serialized = json.dumps(
            report, ensure_ascii=False, sort_keys=True
        )
        for forbidden in (str(ROOT), str(temporary_root)):
            if forbidden in serialized:
                raise IncrementalEditProfileFailure(
                    "incremental edit report leaked a host path"
                )
        return report


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    """Parse the optional ignored report destination."""

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=CASE / "_build/incremental-edit-profile.json",
    )
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    """Write one report and print a concise changed-artifact summary."""

    options = parse_args(arguments)
    try:
        report = profile_incremental_edit()
        write_timing_report(options.output, report)
        generated = report["generatedArtifacts"]
        hxcir = report["hxcir"]
        if not isinstance(generated, dict) or not isinstance(hxcir, dict):
            raise IncrementalEditProfileFailure(
                "incremental edit summary is malformed"
            )
        functions = hxcir.get("functions")
        if not isinstance(functions, dict):
            raise IncrementalEditProfileFailure(
                "incremental edit function summary is malformed"
            )
    except (
        CaxecraftFailure,
        CompilerProfileFailure,
        IncrementalEditProfileFailure,
        OSError,
        subprocess.TimeoutExpired,
    ) as error:
        print(
            f"caxecraft-incremental-edit: ERROR: {error}",
            file=sys.stderr,
        )
        return 1
    print(
        "caxecraft-incremental-edit: OK: "
        f"{len(generated['changed'])} generated artifacts and "
        f"{len(functions['changed'])} HxcIR functions changed; "
        f"report {options.output}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
