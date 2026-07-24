#!/usr/bin/env python3
"""Prove the deterministic, runtime-free Caxecraft domain under Eval and C."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable, Mapping, Sequence
from contextlib import contextmanager
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
sys.path.insert(0, str(CASE))
from check_assets import (  # noqa: E402
    AssetValidationError,
    negative_contracts,
    validate_asset_pack,
)
from check_localization import (  # noqa: E402
    LocalizationCheckFailure,
    check_app_boundary,
    check_generated,
    check_negative_contracts as check_localization_negative_contracts,
)
from play import (  # noqa: E402
    PLAYABLE_SNAPSHOT_FORMATS,
    snapshot_values as playable_snapshot_values,
)

BUILD_HXML = CASE / "build.hxml"
ORACLE_HXML = CASE / "oracle.hxml"
EXPECTED = CASE / "expected"
NATIVE = CASE / "test/native"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
COMMON_PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "meson.build",
}
SPLIT_HEADERS = (
    "include/hxc/detail/program_types.h",
    "include/hxc/modules/caxecraft/domain/AxisMove.h",
    "include/hxc/modules/caxecraft/domain/BlockCoord.h",
    "include/hxc/modules/caxecraft/domain/BlockKind.h",
    "include/hxc/modules/caxecraft/domain/CaxecraftTrace.h",
    "include/hxc/modules/caxecraft/domain/CharacterBody.h",
    "include/hxc/modules/caxecraft/domain/CharacterPhysics.h",
    "include/hxc/modules/caxecraft/domain/RaycastHit.h",
    "include/hxc/modules/caxecraft/domain/StepInput.h",
    "include/hxc/modules/caxecraft/domain/VoxelRaycast.h",
    "include/hxc/modules/caxecraft/domain/World.h",
    "include/hxc/modules/caxecraft/domain/WorldStorage.h",
    "include/hxc/modules/caxecraft/qa/DomainProbe.h",
    "include/hxc/program.h",
)
SPLIT_SOURCES = (
    "src/hxc/main.c",
    "src/hxc/support.c",
    "src/modules/caxecraft/domain/CaxecraftTrace.c",
    "src/modules/caxecraft/domain/CharacterPhysics.c",
    "src/modules/caxecraft/domain/VoxelRaycast.c",
    "src/modules/caxecraft/domain/World.c",
    "src/modules/caxecraft/domain/WorldStorage.c",
    "src/modules/caxecraft/qa/DomainProbe.c",
)
PACKAGE_HEADERS = (
    "include/hxc/detail/program_types.h",
    "include/hxc/packages/caxecraft/domain/package.h",
    "include/hxc/packages/caxecraft/qa/package.h",
    "include/hxc/program.h",
)
PACKAGE_SOURCES = (
    "src/hxc/main.c",
    "src/hxc/support.c",
    "src/packages/caxecraft/domain/package.c",
    "src/packages/caxecraft/qa/package.c",
)
UNITY_HEADERS = ("include/hxc/program.h",)
UNITY_SOURCES = ("src/program.c",)
LAYOUTS = ("split", "package", "unity")
HEADERS_BY_LAYOUT = {
    "split": SPLIT_HEADERS,
    "package": PACKAGE_HEADERS,
    "unity": UNITY_HEADERS,
}
SOURCES_BY_LAYOUT = {
    "split": SPLIT_SOURCES,
    "package": PACKAGE_SOURCES,
    "unity": UNITY_SOURCES,
}
PRODUCTION_FILES = {
    layout: COMMON_PRODUCTION_FILES | set(HEADERS_BY_LAYOUT[layout]) | set(SOURCES_BY_LAYOUT[layout])
    for layout in LAYOUTS
}
SNAPSHOT_FORMATS = {
    **{path: "header" for path in SPLIT_HEADERS},
    **{path: "c" for path in SPLIT_SOURCES},
    **{f"package/{path}": "header" for path in PACKAGE_HEADERS},
    **{f"package/{path}": "c" for path in PACKAGE_SOURCES},
    "hxc.runtime-plan.json": "json",
    "method-symbols.json": "json",
    "maintainability-package.json": "json",
    "maintainability-split.json": "json",
    "maintainability-unity.json": "json",
    "unity/include/hxc/program.h": "header",
    "unity/src/program.c": "c",
    "oracle.txt": "text",
    **PLAYABLE_SNAPSHOT_FORMATS,
}
MAINTAINABILITY_POLICY = ROOT / "docs/specs/generated-c-maintainability-policy.json"
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
SANITIZER_FLAGS = (
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-fno-omit-frame-pointer",
)
COVERAGE = frozenset(
    {
        "aabb-collision",
        "eval-differential",
        "seeded-property-corpus",
        "voxel-dda",
        "zero-runtime",
    }
)

sys.path.insert(0, str(ROOT / "scripts/test"))
from generated_c_maintainability import (  # noqa: E402
    ArtifactOwner,
    FunctionSourceMapping,
    GeneratedCArtifact,
    MaintainabilityError,
    OwnerKind,
    SourceSpan,
    SymbolLedgerEntry,
    analyze_generated_c,
    load_corpus_policy,
    replay_report_from_c,
    validate_report as validate_maintainability_report,
)
from c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


class CaxecraftFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    output: Path
    artifacts: dict[str, bytes]
    hxcir: str
    runtime_plan: dict[str, object]
    method_symbols: dict[str, object]
    maintainability_report: dict[str, object]


@dataclass(frozen=True)
class HaxeInstallation:
    """One exact compiler installation selected by this checkout's `.haxerc`."""

    version: str
    compiler: Path
    standard_library: Path
    haxelib_repository: Path
    neko_library: Path | None


@dataclass(frozen=True)
class HaxeServerConnection:
    """The owned server endpoint and the exact compiler that speaks to it."""

    endpoint: str
    installation: HaxeInstallation


class TimingRecorder:
    """Record where the exhaustive example spends time without host paths.

    A phase is a named piece of useful work. `haxe_requests` says how many
    compiler processes or server requests that phase intentionally makes; it
    separates compiler startup/reuse from native compilation and execution.
    """

    def __init__(self) -> None:
        self.started_ns = time.monotonic_ns()
        self.phases: list[dict[str, object]] = []

    @contextmanager
    def phase(self, name: str, *, haxe_requests: int = 0):
        started_ns = time.monotonic_ns()
        outcome = "passed"
        try:
            yield
        except BaseException:
            outcome = "failed"
            raise
        finally:
            self.phases.append(
                {
                    "name": name,
                    "outcome": outcome,
                    "durationMs": elapsed_milliseconds(started_ns),
                    "haxeRequests": haxe_requests,
                }
            )

    def report(self, *, mode: str, outcome: str) -> dict[str, object]:
        return {
            "schemaVersion": 1,
            "suite": "caxecraft-domain",
            "mode": mode,
            "outcome": outcome,
            "durationMs": elapsed_milliseconds(self.started_ns),
            "summary": {
                "haxeRequests": sum(
                    int(phase["haxeRequests"]) for phase in self.phases
                ),
            },
            "phases": self.phases,
        }


def elapsed_milliseconds(started_ns: int) -> int:
    return max(0, (time.monotonic_ns() - started_ns + 500_000) // 1_000_000)


def write_timing_report(path: Path, payload: dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        dir=path.parent,
        prefix=f".{path.name}.",
        suffix=".tmp",
        text=True,
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="") as handle:
            json.dump(payload, handle, ensure_ascii=False, indent=2)
            handle.write("\n")
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(locale: str, *, server: bool) -> dict[str, str]:
    environment = os.environ.copy()
    environment["LC_ALL"] = locale
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def pinned_haxe_installation(
    *,
    scope_root: Path = ROOT,
    environment: Mapping[str, str] | None = None,
) -> HaxeInstallation:
    """Resolve the exact Lix/HaxeShim installation without using global Haxe.

    HaxeShim normally performs this lookup before it launches the compiler. A
    direct native client is needed here because HaxeShim's multi-version server
    proxy adds a private routing argument that the native Haxe server does not
    understand. Mirroring its documented directory convention lets us bind the
    real compiler server to loopback while retaining the repository pin.
    """

    source_environment = os.environ if environment is None else environment
    pin_path = scope_root / ".haxerc"
    try:
        pin: object = json.loads(pin_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CaxecraftFailure(f"cannot read the Haxe pin {pin_path}: {error}") from error
    if not isinstance(pin, dict):
        raise CaxecraftFailure(".haxerc must contain a JSON object")
    version = pin.get("version")
    if (
        not isinstance(version, str)
        or re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9.+_-]*", version) is None
    ):
        raise CaxecraftFailure(".haxerc contains an unsafe or missing Haxe version")
    if pin.get("resolveLibs") != "scoped":
        raise CaxecraftFailure("Caxecraft requires .haxerc resolveLibs=scoped")

    haxe_root_text = source_environment.get("HAXE_ROOT") or source_environment.get(
        "HAXESHIM_ROOT"
    )
    if haxe_root_text is None:
        home_key = "APPDATA" if os.name == "nt" else "HOME"
        home = source_environment.get(home_key)
        if not home:
            raise CaxecraftFailure(
                f"cannot resolve the Lix Haxe installation: {home_key} is unset"
            )
        haxe_root = Path(home) / "haxe"
    else:
        haxe_root = Path(haxe_root_text)

    versions_root = (haxe_root / "versions").resolve()
    installation_root = (versions_root / version).resolve()
    if installation_root.parent != versions_root:
        raise CaxecraftFailure("the pinned Haxe version escaped the versions directory")
    executable_name = "haxe.exe" if os.name == "nt" else "haxe"
    compiler = installation_root / executable_name
    standard_library = installation_root / "std"
    if not compiler.is_file() or (os.name != "nt" and not os.access(compiler, os.X_OK)):
        raise CaxecraftFailure(
            f"pinned Haxe {version} is not installed at {compiler}; run npm ci"
        )
    if not standard_library.is_dir():
        raise CaxecraftFailure(
            f"pinned Haxe {version} has no standard library at {standard_library}"
        )

    neko_library: Path | None = None
    platform_path = installation_root / "platform.txt"
    if platform_path.is_file():
        try:
            platform = platform_path.read_text(encoding="utf-8").strip()
        except (OSError, UnicodeError) as error:
            raise CaxecraftFailure(
                f"cannot read pinned Haxe platform metadata: {error}"
            ) from error
        if re.fullmatch(r"[A-Za-z0-9_-]+", platform) is None:
            raise CaxecraftFailure("pinned Haxe platform metadata is malformed")
        candidate = haxe_root / "neko" / "versions" / f"2.4.0-{platform}"
        if candidate.is_dir():
            neko_library = candidate.resolve()

    return HaxeInstallation(
        version,
        compiler,
        standard_library,
        (haxe_root / "haxelib").resolve(),
        neko_library,
    )


def pinned_haxe_environment(
    locale: str, installation: HaxeInstallation
) -> dict[str, str]:
    """Reproduce HaxeShim's compiler environment for the direct native client."""

    environment = haxe_environment(locale, server=True)
    environment.update(
        {
            "HAXE_STD_PATH": str(installation.standard_library),
            "HAXEPATH": str(installation.compiler.parent),
            "HAXELIB_PATH": str(installation.haxelib_repository),
            "HAXE_VERSION": installation.version,
        }
    )
    if installation.neko_library is not None:
        if os.name == "nt":
            library_key = "PATH"
        elif sys.platform == "darwin":
            library_key = "DYLD_LIBRARY_PATH"
        else:
            library_key = "LD_LIBRARY_PATH"
        library_path = str(installation.neko_library)
        entries = environment.get(library_key, "").split(os.pathsep)
        if library_path not in entries:
            current = environment.get(library_key)
            environment[library_key] = (
                f"{current}{os.pathsep}{library_path}" if current else library_path
            )
    return environment


def verify_pinned_haxe(installation: HaxeInstallation) -> None:
    """Fail before starting a server if the resolved binary is not the pin."""

    result = subprocess.run(
        [str(installation.compiler), "--version"],
        cwd=ROOT,
        env=pinned_haxe_environment("C", installation),
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    observed = f"{result.stdout}{result.stderr}".strip()
    if result.returncode != 0 or observed != installation.version:
        raise CaxecraftFailure(
            "resolved Haxe compiler does not match .haxerc: "
            f"expected {installation.version!r}, observed {observed!r}, "
            f"exit={result.returncode}"
        )


def target_arguments(
    output: Path,
    *,
    layout: str,
    reverse: bool,
    report: bool,
    times: bool = False,
    phase_timing: bool = False,
) -> list[str]:
    arguments = [BUILD_HXML.name, "-D", "hxc_runtime_diagnostics=off"]
    if reverse:
        arguments.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        arguments.extend(["-D", "reflaxe_c_static_initialization_report"])
    if layout in ("package", "unity"):
        arguments.extend(["-D", f"hxc_project_layout={layout}"])
    elif layout != "split":
        raise CaxecraftFailure(f"unknown Caxecraft project layout {layout!r}")
    if phase_timing:
        arguments.extend(["-D", "reflaxe_c_phase_timing"])
    if times:
        arguments.append("--times")
    arguments.extend(["--custom-target", f"c={output}"])
    return arguments


def resolve_haxe_arguments(
    arguments: Sequence[str], *, locale: str
) -> tuple[str, ...]:
    """Ask HaxeShim to expand HXML/scoped libraries for a native server client."""

    result = subprocess.run(
        [
            development_tool("haxe"),
            "--cwd",
            str(CASE),
            "--run",
            "resolve-args",
            *arguments,
        ],
        cwd=ROOT,
        env=haxe_environment(locale, server=False),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise CaxecraftFailure(
            "HaxeShim could not resolve the pinned Caxecraft build arguments\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    resolved = tuple(result.stdout.splitlines())
    if not resolved or any(
        not argument or "\x00" in argument or "\r" in argument or "\n" in argument
        for argument in resolved
    ):
        raise CaxecraftFailure("HaxeShim emitted malformed resolved arguments")
    if "--haxe-version" in resolved or any(argument.endswith(".hxml") for argument in resolved):
        raise CaxecraftFailure(
            "HaxeShim left a proxy-only or unresolved build argument in the native request"
        )
    return resolved


def compile_target(
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    locale: str = "C",
    connect: HaxeServerConnection | None = None,
    report: bool = False,
    times: bool = False,
    phase_timing: bool = False,
) -> subprocess.CompletedProcess[str]:
    arguments = target_arguments(
        output,
        layout=layout,
        reverse=reverse,
        report=report,
        times=times,
        phase_timing=phase_timing,
    )
    if connect is None:
        command = [development_tool("haxe"), "--cwd", str(CASE), *arguments]
        cwd = ROOT
        environment = haxe_environment(locale, server=False)
    else:
        resolved = resolve_haxe_arguments(arguments, locale=locale)
        command = [
            str(connect.installation.compiler),
            "--connect",
            connect.endpoint,
            *resolved,
        ]
        cwd = CASE
        environment = pinned_haxe_environment(locale, connect.installation)
    return subprocess.run(
        command,
        cwd=cwd,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )


def load_json(path: Path, label: str) -> dict[str, object]:
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CaxecraftFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise CaxecraftFailure(f"{label} must contain a JSON object")
    return value


def extract_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
    lines = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(lines) != 1:
        raise CaxecraftFailure(f"{label} emitted an invalid HxcIR report envelope")
    report = json.loads(lines[0])
    if not isinstance(report, dict) or report.get("schemaVersion") != 1:
        raise CaxecraftFailure(f"{label} emitted an invalid HxcIR report")
    hxcir = report.get("hxcir")
    if not isinstance(hxcir, str):
        raise CaxecraftFailure(f"{label} omitted its HxcIR dump")
    return hxcir


def normal_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"), key=lambda item: item.as_posix().encode("utf-8"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def generated_files(output: Path) -> set[str]:
    return {
        path.relative_to(output).as_posix()
        for path in output.rglob("*")
        if path.is_file()
    }


def method_symbol_projection(symbols: dict[str, object]) -> dict[str, object]:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise CaxecraftFailure("generated symbol table omitted its symbols array")
    callables: list[dict[str, str]] = []
    for entry in entries:
        if not isinstance(entry, dict):
            raise CaxecraftFailure("generated symbol table contains a malformed entry")
        source = entry.get("sourceSymbol")
        c_name = entry.get("cName")
        kind = entry.get("kind")
        readable_name = entry.get("readableName")
        if (
            kind in ("function", "method")
            and isinstance(source, str)
            and source.startswith("caxecraft.")
        ):
            if not isinstance(c_name, str):
                raise CaxecraftFailure(f"callable symbol {source!r} has no C name")
            if not isinstance(readable_name, list) or not all(
                isinstance(part, str) and part for part in readable_name
            ):
                raise CaxecraftFailure(
                    f"callable symbol {source!r} has no readable source name"
                )
            callables.append(
                {
                    "sourceSymbol": source,
                    "readableSymbol": ".".join(readable_name),
                    "cName": c_name,
                    "kind": kind,
                }
            )
    callables.sort(
        key=lambda entry: (
            entry["readableSymbol"].encode("utf-8"),
            entry["sourceSymbol"].encode("utf-8"),
        )
    )
    return {
        "schemaVersion": 2,
        "algorithm": "caxecraft-callable-symbol-projection-v2",
        "callables": callables,
    }


def text_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise CaxecraftFailure(f"{label} must be a string array")
    return list(value)


def validate_runtime_plan(plan: dict[str, object]) -> None:
    proof = plan.get("noRuntimeProof")
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != "analyzed-runtime-free"
        or plan.get("profile") != "portable"
        or plan.get("environment") != "hosted"
        or plan.get("requestedPolicy") != "none"
        or plan.get("resolvedPolicy") != "none"
        or plan.get("policyProvenance") != "direct-define:hxc_runtime"
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or proof.get("scope") != "reachable-whole-program"
    ):
        raise CaxecraftFailure("Caxecraft runtime-free policy/proof drifted")
    for key in (
        "selectedFeatures",
        "features",
        "artifacts",
        "artifactDetails",
        "libraries",
        "defines",
        "rootReasons",
        "dependencyEdges",
        "manualOverrides",
        "symbols",
    ):
        if plan.get(key) != []:
            raise CaxecraftFailure(f"Caxecraft unexpectedly populated runtime-plan {key}")
    absence = proof.get("runtimeAbsence")
    if not isinstance(absence, dict) or any(
        absence.get(key) != []
        for key in ("sources", "features", "symbols", "libraries", "includes", "defines")
    ):
        raise CaxecraftFailure("Caxecraft no-runtime proof retained a runtime effect")
    decisions = text_list(plan.get("directDecisions"), "runtime direct decisions")
    for expected in (
        "bounded-haxe-enum-values",
        "closed-anonymous-value-records",
        "primitive-static-storage",
        "selected-program-local-helpers",
        "ub-safe-primitive-operations",
    ):
        if expected not in decisions:
            raise CaxecraftFailure(f"Caxecraft runtime proof omitted {expected!r}")


def validate_method_symbols(projection: dict[str, object]) -> None:
    methods = projection.get("callables")
    if (
        projection.get("schemaVersion") != 2
        or projection.get("algorithm") != "caxecraft-callable-symbol-projection-v2"
        or not isinstance(methods, list)
    ):
        raise CaxecraftFailure("Caxecraft callable-symbol projection drifted")
    pairs: list[tuple[str, str]] = []
    for entry in methods:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("sourceSymbol"), str)
            or not isinstance(entry.get("readableSymbol"), str)
            or not isinstance(entry.get("cName"), str)
            or entry.get("kind") not in ("function", "method")
        ):
            raise CaxecraftFailure("Caxecraft callable-symbol projection is malformed")
        pairs.append((entry["sourceSymbol"], entry["cName"]))
    order = [
        (entry["readableSymbol"].encode("utf-8"), entry["sourceSymbol"].encode("utf-8"))
        for entry in methods
    ]
    if order != sorted(order):
        raise CaxecraftFailure("Caxecraft callable-symbol projection is not UTF-8 sorted")
    readable_sources = {entry["readableSymbol"] for entry in methods}
    required = {
        "caxecraft.domain.CaxecraftTrace.propertyTrace",
        "caxecraft.domain.CaxecraftTrace.runTrace",
        "caxecraft.domain.VoxelRaycast.trace",
        "caxecraft.domain.World.generate",
        "caxecraft.qa.DomainProbe.selfCheck",
    }
    missing = sorted(required - readable_sources)
    if missing:
        raise CaxecraftFailure(f"Caxecraft callable symbols omitted {missing!r}")
    if "caxecraft.domain.CharacterPhysics.step" not in readable_sources:
        raise CaxecraftFailure("Caxecraft callable symbols omitted CharacterPhysics.step")
    report_values = [
        value
        for entry in methods
        for value in (entry["sourceSymbol"], entry["readableSymbol"], entry["cName"])
    ]
    if any("hxrt" in value.lower() for value in report_values):
        raise CaxecraftFailure("Caxecraft method symbols unexpectedly mention hxrt")


def projected_method_name(
    projection: dict[str, object], source_symbol: str, *, prefix: bool = False
) -> str:
    methods = projection.get("callables")
    if not isinstance(methods, list):
        raise CaxecraftFailure("Caxecraft callable-symbol projection is malformed")
    matches = [
        item.get("cName")
        for item in methods
        if isinstance(item, dict)
        and isinstance(item.get("sourceSymbol"), str)
        and isinstance(item.get("readableSymbol"), str)
        and (
            (
                item["sourceSymbol"].startswith(source_symbol)
                or item["readableSymbol"].startswith(source_symbol)
            )
            if prefix
            else (
                item["sourceSymbol"] == source_symbol
                or item["readableSymbol"] == source_symbol
            )
        )
        and isinstance(item.get("cName"), str)
    ]
    if len(matches) != 1:
        raise CaxecraftFailure(
            f"cannot resolve one generated C callable for {source_symbol!r}: {matches!r}"
        )
    return matches[0]


def native_method_symbol_header(projection: dict[str, object]) -> str:
    aliases = (
        ("CAXECRAFT_SELF_CHECK", "caxecraft.qa.DomainProbe.selfCheck"),
        (
            "CAXECRAFT_TERRAIN_TRACE",
            "caxecraft.domain.CaxecraftTrace.terrainTrace",
        ),
        ("CAXECRAFT_EDIT_TRACE", "caxecraft.domain.CaxecraftTrace.editTrace"),
        ("CAXECRAFT_RAY_TRACE", "caxecraft.domain.CaxecraftTrace.rayTrace"),
        (
            "CAXECRAFT_COLLISION_TRACE",
            "caxecraft.domain.CaxecraftTrace.collisionTrace",
        ),
        ("CAXECRAFT_RUN_TRACE", "caxecraft.domain.CaxecraftTrace.runTrace"),
        (
            "CAXECRAFT_PROPERTY_TRACE",
            "caxecraft.domain.CaxecraftTrace.propertyTrace(i32)",
        ),
    )
    definitions = []
    for alias, source_symbol in aliases:
        c_name = projected_method_name(projection, source_symbol)
        if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", c_name) is None:
            raise CaxecraftFailure(
                f"native method symbol is not a C identifier: {c_name!r}"
            )
        definitions.append(f"#define {alias} {c_name}")
    return "\n".join(
        (
            "#ifndef CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED",
            "#define CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED",
            "",
            "/* Test-only callable aliases projected from the compiler symbol report. */",
            *definitions,
            "",
            "#endif /* CAXECRAFT_TEST_METHOD_SYMBOLS_H_INCLUDED */",
            "",
        )
    )


def projected_semantic_function_id(
    projection: dict[str, object], readable_symbol: str
) -> str:
    callables = projection.get("callables")
    if not isinstance(callables, list):
        raise CaxecraftFailure("Caxecraft callable-symbol projection is malformed")
    matches = [
        entry["sourceSymbol"].split("(", 1)[0]
        for entry in callables
        if isinstance(entry, dict)
        and entry.get("readableSymbol") == readable_symbol
        and isinstance(entry.get("sourceSymbol"), str)
    ]
    if len(matches) != 1:
        raise CaxecraftFailure(
            f"cannot resolve one semantic function for {readable_symbol!r}: {matches!r}"
        )
    return f"function.{matches[0]}"


def validate_hxcir(hxcir: str, projection: dict[str, object]) -> None:
    player_step_id = projected_semantic_function_id(
        projection, "caxecraft.domain.CharacterPhysics.step"
    )
    for marker in (
        "hxcir schema=19",
        'function "function.caxecraft.domain.World.generate"',
        'function "function.caxecraft.domain.VoxelRaycast.trace"',
        f'function "{player_step_id}"',
        "initialize-fixed-array",
        "initialize-span",
        "span-parameter-borrow",
    ):
        if marker not in hxcir:
            raise CaxecraftFailure(f"Caxecraft HxcIR omitted {marker!r}")
    for forbidden in ("runtime(feature=", "allocate", "cleanup action"):
        if forbidden in hxcir:
            raise CaxecraftFailure(f"Caxecraft HxcIR unexpectedly contains {forbidden!r}")
    if str(ROOT) in hxcir or "\\" in hxcir:
        raise CaxecraftFailure("Caxecraft HxcIR leaked a host path")


def validate_generated_text(
    header: bytes, source: bytes, method_symbols: dict[str, object]
) -> None:
    combined = header + b"\n" + source
    if str(ROOT).encode() in combined or b"\\" in combined or b"hxrt" in combined.lower():
        raise CaxecraftFailure("generated Caxecraft C leaked a host path or hxrt")
    text = combined.decode("utf-8")
    forbidden = re.compile(r"(?:\bhxrt_[A-Za-z0-9_]*\b|\b(?:malloc|calloc|realloc|free)\s*\()")
    match = forbidden.search(text)
    if match is not None:
        raise CaxecraftFailure(f"generated Caxecraft C selected {match.group(0)!r}")
    # Match compiler statements, not story/localization text that happens to say "goto".
    goto_match = re.search(r"(?m)^\s*goto\s+[A-Za-z_][A-Za-z0-9_]*\s*;$", text)
    compiler_label = re.search(r"(?m)^\s*hxc_[A-Za-z0-9_]+:\s*$", text)
    if goto_match is not None or compiler_label is not None:
        marker = goto_match.group(0) if goto_match is not None else compiler_label.group(0).strip()
        raise CaxecraftFailure(
            f"generated Caxecraft C retained compiler control-flow marker {marker!r}"
        )
    for marker in (
        "uint8_t hxc_storage[16384]",
        projected_method_name(
            method_symbols,
            "caxecraft.domain.VoxelRaycast.trace(span:mutable<u8>, f64, f64, f64, f64, f64, f64, f64)",
        ),
        projected_method_name(
            method_symbols, "caxecraft.domain.CharacterPhysics.step", prefix=True
        ),
        projected_method_name(
            method_symbols, "caxecraft.qa.DomainProbe.selfCheck"
        ),
    ):
        if marker not in text:
            raise CaxecraftFailure(f"generated Caxecraft C omitted {marker!r}")


def validate_block_coord_declaration(content: str) -> None:
    expected_record = (
        "struct hxc_caxecraft_domain_BlockCoord {\n"
        "  int32_t hxc_x;\n"
        "  int32_t hxc_y;\n"
        "  int32_t hxc_z;\n"
        "};"
    )
    if expected_record not in content:
        raise CaxecraftFailure(
            "BlockCoord lost its source-shaped tag or readable hxc_x/y/z members"
        )
    for forbidden in ("HXC_GENERATED_PATH_", "closedzx", "_h4aec2e39", "zx2D"):
        if forbidden in content:
            raise CaxecraftFailure(
                f"BlockCoord.h leaked machine identity {forbidden!r} into ordinary C"
            )


def validate_block_coord_header(content: str) -> None:
    guard = "HXC_CAXECRAFT_DOMAIN_BLOCK_COORD_H_INCLUDED"
    validate_block_coord_declaration(content)
    if not content.startswith(f"#ifndef {guard}\n#define {guard}\n") or not content.endswith(
        f"#endif /* {guard} */\n"
    ):
        raise CaxecraftFailure("BlockCoord.h lost its readable standalone guard")


HXCIR_FUNCTION_SOURCE = re.compile(
    r'^  function "[^"]+" name="(?P<name>[^"]+)"[^\n]* '
    r'@"(?P<path>[^"]+)":(?P<start_line>[0-9]+):(?P<start_column>[0-9]+)-'
    r'(?P<end_line>[0-9]+):(?P<end_column>[0-9]+)$',
    re.MULTILINE,
)


def hxcir_function_sources(hxcir: str) -> dict[str, SourceSpan]:
    result: dict[str, SourceSpan] = {}
    for match in HXCIR_FUNCTION_SOURCE.finditer(hxcir):
        name = match.group("name")
        if name in result:
            raise CaxecraftFailure(
                f"Caxecraft HxcIR has overloaded function identity {name!r}; "
                "the maintainability mapping needs an overload-aware key"
            )
        result[name] = SourceSpan(
            match.group("path"),
            int(match.group("start_line")),
            int(match.group("start_column")),
            int(match.group("end_line")),
            int(match.group("end_column")),
        )
    if not result:
        raise CaxecraftFailure("Caxecraft HxcIR omitted function source spans")
    return result


def maintainability_owner(path: str, layout: str) -> ArtifactOwner:
    for prefix in ("include/hxc/modules/", "src/modules/"):
        if path.startswith(prefix):
            suffix = path[len(prefix) :]
            module_path = suffix.rsplit(".", 1)[0].replace("/", ".")
            return ArtifactOwner(OwnerKind.SOURCE_MODULE, module_path)
    for prefix in ("include/hxc/packages/", "src/packages/"):
        if path.startswith(prefix):
            suffix = path[len(prefix) :]
            if suffix in ("package.h", "package.c"):
                return ArtifactOwner(
                    OwnerKind.SOURCE_PACKAGE,
                    package_path="<root>",
                )
            for ending in ("/package.h", "/package.c"):
                if suffix.endswith(ending):
                    package_path = suffix[: -len(ending)]
                    return ArtifactOwner(
                        OwnerKind.SOURCE_PACKAGE,
                        package_path=package_path or "<root>",
                    )
    if path == "src/hxc/main.c":
        return ArtifactOwner(OwnerKind.COMPILER_ENTRY)
    if layout == "unity" and path == "src/program.c":
        return ArtifactOwner(OwnerKind.AMALGAMATION)
    return ArtifactOwner(OwnerKind.COMPILER_SUPPORT)


def function_definition_present(content: str, c_name: str) -> bool:
    return re.search(
        rf"(?m)^[^;{{}}\n]*\b{re.escape(c_name)}\([^;{{}}\n]*\)\n\{{",
        content,
    ) is not None


FUNCTION_DEFINITION_NAME = re.compile(
    r"(?m)^[^;{}\n]*\b(?P<name>[A-Za-z_][A-Za-z0-9_]*)"
    r"\([^;{}\n]*\)\n\{"
)


def maintainability_function_mappings(
    contents: dict[str, bytes],
    method_symbols: dict[str, object],
    hxcir: str,
    layout: str,
) -> dict[str, tuple[FunctionSourceMapping, ...]]:
    methods = method_symbols.get("callables")
    if not isinstance(methods, list):
        raise CaxecraftFailure("Caxecraft callable projection omitted callables")
    spans = hxcir_function_sources(hxcir)
    decoded = {path: content.decode("utf-8") for path, content in contents.items()}
    sources = {path: content for path, content in decoded.items() if path.endswith(".c")}
    mappings: dict[str, list[FunctionSourceMapping]] = {
        path: [] for path in contents
    }
    for method in methods:
        if not isinstance(method, dict):
            raise CaxecraftFailure("Caxecraft method projection contains a malformed entry")
        source_symbol = method.get("sourceSymbol")
        c_name = method.get("cName")
        if not isinstance(source_symbol, str) or not isinstance(c_name, str):
            raise CaxecraftFailure("Caxecraft method projection contains an incomplete entry")
        function_id = source_symbol.split("(", 1)[0]
        module_path = function_id.rsplit(".", 1)[0]
        source = spans.get(function_id)
        if source is None:
            raise CaxecraftFailure(
                f"Caxecraft HxcIR omitted source span for {function_id!r}"
            )
        owners = [
            path
            for path, content in sources.items()
            if function_definition_present(content, c_name)
        ]
        if len(owners) != 1:
            raise CaxecraftFailure(
                f"Caxecraft method {source_symbol!r} matched {owners!r}, expected one C definition"
            )
        mappings[owners[0]].append(
            FunctionSourceMapping(
                function_id,
                c_name,
                ArtifactOwner(OwnerKind.SOURCE_MODULE, module_path, source),
                source,
            )
        )
    for path, content in decoded.items():
        mapped_names = {mapping.c_name for mapping in mappings[path]}
        for match in FUNCTION_DEFINITION_NAME.finditer(content):
            c_name = match.group("name")
            if c_name in mapped_names:
                continue
            mappings[path].append(
                FunctionSourceMapping(
                    f"compiler.{path}:{c_name}",
                    c_name,
                    maintainability_owner(path, layout),
                )
            )
            mapped_names.add(c_name)
    return {
        path: tuple(sorted(values, key=lambda value: value.function_id.encode("utf-8")))
        for path, values in mappings.items()
    }


def maintainability_symbol_ledger(symbols: dict[str, object]) -> tuple[SymbolLedgerEntry, ...]:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise CaxecraftFailure("Caxecraft symbol table omitted entries")
    result: list[SymbolLedgerEntry] = []
    for entry in entries:
        if not isinstance(entry, dict):
            raise CaxecraftFailure("Caxecraft symbol table contains a malformed entry")
        c_name = entry.get("cName")
        source_symbol = entry.get("sourceSymbol")
        reasons = entry.get("escapeReasons")
        if not isinstance(c_name, str) or not isinstance(source_symbol, str):
            continue
        if not isinstance(reasons, list) or not all(isinstance(item, str) for item in reasons):
            raise CaxecraftFailure(f"Caxecraft symbol {source_symbol!r} has malformed reasons")
        result.append(
            SymbolLedgerEntry(
                c_name,
                source_symbol,
                entry.get("collisionResolved") is True,
                tuple(sorted(reasons, key=lambda value: value.encode("utf-8"))),
            )
        )
    return tuple(
        sorted(result, key=lambda value: (value.c_name.encode("utf-8"), value.source_symbol.encode("utf-8")))
    )


def generated_maintainability_report(
    output: Path,
    layout: str,
    method_symbols: dict[str, object],
    symbols: dict[str, object],
    hxcir: str,
) -> dict[str, object]:
    if layout not in HEADERS_BY_LAYOUT or layout not in SOURCES_BY_LAYOUT:
        raise CaxecraftFailure(f"unknown maintainability layout {layout!r}")
    paths = (*HEADERS_BY_LAYOUT[layout], *SOURCES_BY_LAYOUT[layout])
    contents = {path: (output / path).read_bytes() for path in paths}
    mappings = maintainability_function_mappings(
        contents, method_symbols, hxcir, layout
    )
    artifacts = tuple(
        GeneratedCArtifact(
            path,
            contents[path],
            maintainability_owner(path, layout),
            mappings[path],
        )
        for path in paths
    )
    policy = load_corpus_policy(
        MAINTAINABILITY_POLICY,
        corpus_id="caxecraft-domain",
        layout=layout,
    )
    report = analyze_generated_c(
        corpus_id="caxecraft-domain",
        layout=layout,
        artifacts=artifacts,
        symbols=maintainability_symbol_ledger(symbols),
        policy=policy,
    )
    if report.get("status") != "within-reviewed-budgets":
        raise CaxecraftFailure(
            f"generated Caxecraft {layout} C exceeded its maintainability policy: "
            f"{report.get('firstViolation')!r}"
        )
    return report


def validate_maintainability_inputs(
    report: dict[str, object], layout: str, contents: dict[str, bytes]
) -> None:
    validate_maintainability_report(report)
    inputs = report.get("inputs")
    expected = [
        {"path": path, "sha256": hashlib.sha256(content).hexdigest()}
        for path, content in sorted(contents.items(), key=lambda item: item[0].encode("utf-8"))
    ]
    if (
        report.get("corpusId") != "caxecraft-domain"
        or report.get("layout") != layout
        or report.get("status") != "within-reviewed-budgets"
        or inputs != expected
    ):
        raise CaxecraftFailure(
            f"checked-in Caxecraft {layout} maintainability report does not describe its C inputs"
        )
    policy = load_corpus_policy(
        MAINTAINABILITY_POLICY,
        corpus_id="caxecraft-domain",
        layout=layout,
    )
    replayed = replay_report_from_c(report, contents, policy)
    if replayed != report:
        raise CaxecraftFailure(
            f"checked-in Caxecraft {layout} maintainability report does not reproduce from its exact C bytes"
        )


MAINTAINABILITY_LAYOUT_SUMMARY_FIELDS = (
    "functionCount",
    "functionIdentityMappedCount",
    "functionIdentityMappedBasisPoints",
    "sourceSpanMappedFunctionCount",
    "sourceSpanMappedFunctionBasisPoints",
    "lineDirectiveCount",
    "branchCount",
    "temporaryDeclarationCount",
    "temporaryReferenceCount",
    "temporaryDeclarationsPerKFunctionCodeLines",
    "roleEncodedIdentifierCount",
    "unexplainedRoleEncodedIdentifierCount",
    "byteEscapedIdentifierCount",
    "unexplainedByteEscapedIdentifierCount",
    "digestIdentifierCount",
    "unexplainedDigestIdentifierCount",
    "hashSuffixIdentifierCount",
    "unexplainedHashSuffixIdentifierCount",
    "gotoCount",
    "unauthorizedGotoCount",
    "gotoCategoryCounts",
)

MAINTAINABILITY_LAYOUT_FUNCTION_FIELDS = (
    "functionId",
    "cName",
    "identityMapped",
    "source",
    "physicalLineCount",
    "codeLineCount",
    "bodyCodeLineCount",
    "branchCount",
    "maxNestingDepth",
    "temporaryDeclarationCount",
    "temporaryReferenceCount",
    "temporaryDeclarationsPerKCodeLines",
)


def maintainability_layout_projection(report: dict[str, object]) -> dict[str, object]:
    summary = report.get("summary")
    files = report.get("files")
    if not isinstance(summary, dict) or not isinstance(files, list):
        raise CaxecraftFailure("Caxecraft maintainability layout report is incomplete")
    maximum = summary.get("maxIdentifier")
    if not isinstance(maximum, dict):
        raise CaxecraftFailure(
            "Caxecraft maintainability layout report omitted its maximum identifier"
        )
    source_functions: list[dict[str, object]] = []
    for file_record in files:
        if not isinstance(file_record, dict) or not isinstance(
            file_record.get("functions"), list
        ):
            raise CaxecraftFailure(
                "Caxecraft maintainability layout report has a malformed file"
            )
        for function in file_record["functions"]:
            if not isinstance(function, dict):
                raise CaxecraftFailure(
                    "Caxecraft maintainability layout report has a malformed function"
                )
            if function.get("source") is None:
                continue
            source_functions.append(
                {field: function.get(field) for field in MAINTAINABILITY_LAYOUT_FUNCTION_FIELDS}
            )
    source_functions.sort(key=lambda item: str(item["functionId"]).encode("utf-8"))
    return {
        "summary": {
            field: summary.get(field)
            for field in MAINTAINABILITY_LAYOUT_SUMMARY_FIELDS
        },
        "maxIdentifier": {
            "name": maximum.get("name"),
            "bytes": maximum.get("bytes"),
        },
        "sourceFunctions": source_functions,
        "identifierFindings": [
            {
                key: value
                for key, value in finding.items()
                if key != "coordinate"
            }
            for finding in report.get("identifierFindings", [])
            if isinstance(finding, dict)
        ],
    }


def require_maintainability_layout_parity(
    baseline: dict[str, object], *candidates: dict[str, object]
) -> None:
    baseline_projection = maintainability_layout_projection(baseline)
    if any(
        maintainability_layout_projection(candidate) != baseline_projection
        for candidate in candidates
    ):
        raise CaxecraftFailure(
            "split, package, and unity maintainability reports disagree on semantic function metrics"
        )


def validate_symbol_readability(symbols: dict[str, object]) -> None:
    entries = symbols.get("symbols")
    collisions = symbols.get("collisions")
    if (
        symbols.get("schemaVersion") != 2
        or symbols.get("algorithm") != "hxc-c-symbol-v2"
        or not isinstance(entries, list)
        or not isinstance(collisions, list)
    ):
        raise CaxecraftFailure("Caxecraft readable symbol-table contract drifted")
    collision_names: set[str] = set()
    for collision in collisions:
        if not isinstance(collision, dict) or not isinstance(
            collision.get("symbols"), list
        ):
            raise CaxecraftFailure(
                "Caxecraft symbol table contains a malformed collision ledger"
            )
        for item in collision["symbols"]:
            if not isinstance(item, dict) or not isinstance(item.get("cName"), str):
                raise CaxecraftFailure(
                    "Caxecraft symbol table contains a malformed collision entry"
                )
            collision_names.add(item["cName"])
    block_coord = []
    for entry in entries:
        if not isinstance(entry, dict) or not isinstance(entry.get("cName"), str):
            raise CaxecraftFailure("Caxecraft symbol table contains a malformed entry")
        c_name = entry["cName"]
        readable = entry.get("readableName")
        reasons = entry.get("escapeReasons")
        if len(c_name) > 120 or re.search(r"zx[0-9A-Fa-f]{2}", c_name):
            raise CaxecraftFailure(
                f"Caxecraft symbol retained an encoded/overlong C name: {c_name!r}"
            )
        if entry.get("collisionResolved") is True and c_name not in collision_names:
            raise CaxecraftFailure(
                f"Caxecraft collision suffix is absent from its ledger: {c_name!r}"
            )
        if re.search(r"_h[0-9a-f]{12,64}$", c_name) and not (
            entry.get("collisionResolved") is True
            or isinstance(reasons, list)
            and "length-limit" in reasons
        ):
            raise CaxecraftFailure(
                f"Caxecraft symbol has an unexplained hash suffix: {c_name!r}"
            )
        if readable == ["caxecraft", "domain", "BlockCoord"]:
            block_coord.append(c_name)
    if block_coord != ["hxc_caxecraft_domain_BlockCoord"]:
        raise CaxecraftFailure(
            f"BlockCoord semantic/display identity drifted: {block_coord!r}"
        )


def generated_c_bytes(output: Path, layout: str) -> tuple[bytes, bytes]:
    headers = HEADERS_BY_LAYOUT.get(layout)
    sources = SOURCES_BY_LAYOUT.get(layout)
    if headers is None or sources is None:
        raise CaxecraftFailure(f"unknown generated-C layout {layout!r}")
    return (
        b"\n".join((output / path).read_bytes() for path in headers),
        b"\n".join((output / path).read_bytes() for path in sources),
    )


def render_project(
    output: Path,
    *,
    label: str,
    layout: str = "split",
    reverse: bool = False,
    locale: str = "C",
    connect: str | None = None,
) -> RenderedProject:
    result = compile_target(
        output,
        layout=layout,
        reverse=reverse,
        locale=locale,
        connect=connect,
        report=True,
    )
    if result.returncode != 0 or result.stderr:
        raise CaxecraftFailure(
            f"{label} failed or emitted diagnostics\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if not any(
        line.startswith(REPORT_PREFIX) for line in result.stdout.splitlines()
    ):
        raise CaxecraftFailure(f"{label} omitted its requested HxcIR report")
    actual_files = generated_files(output)
    expected_files = PRODUCTION_FILES.get(layout)
    if expected_files is None or actual_files != expected_files:
        raise CaxecraftFailure(
            f"{label} generated file set drifted: {sorted(actual_files)!r}"
        )
    manifest = load_json(output / "hxc.manifest.json", f"{label} compiler manifest")
    configuration = manifest.get("configuration")
    build = manifest.get("build")
    if (
        not isinstance(configuration, dict)
        or configuration.get("projectLayout") != layout
        or not isinstance(build, dict)
        or build.get("sources") != list(SOURCES_BY_LAYOUT[layout])
        or build.get("privateHeaders") != list(HEADERS_BY_LAYOUT[layout])
    ):
        raise CaxecraftFailure(
            f"{label} layout/build manifest drifted: "
            f"projectLayout={configuration.get('projectLayout') if isinstance(configuration, dict) else None!r}, "
            f"sources={build.get('sources') if isinstance(build, dict) else None!r}, "
            f"privateHeaders={build.get('privateHeaders') if isinstance(build, dict) else None!r}"
        )
    runtime_plan = load_json(output / "hxc.runtime-plan.json", f"{label} runtime plan")
    validate_runtime_plan(runtime_plan)
    stdlib = load_json(output / "hxc.stdlib-report.json", f"{label} stdlib report")
    if (
        stdlib.get("schemaVersion") != 1
        or stdlib.get("status") != "analyzed-no-stdlib-use"
        or stdlib.get("modules") != []
        or stdlib.get("capabilities") != []
    ):
        raise CaxecraftFailure(f"{label} unexpectedly selected a Haxe stdlib surface")
    symbols = load_json(output / "hxc.symbols.json", f"{label} symbol table")
    validate_symbol_readability(symbols)
    projection = method_symbol_projection(symbols)
    validate_method_symbols(projection)
    hxcir = extract_hxcir(result, label)
    validate_hxcir(hxcir, projection)
    header, source = generated_c_bytes(output, layout)
    validate_generated_text(header, source, projection)
    maintainability_report = generated_maintainability_report(
        output, layout, projection, symbols, hxcir
    )
    if layout == "split":
        validate_block_coord_header(
            (output / "include/hxc/modules/caxecraft/domain/BlockCoord.h").read_text(
                encoding="utf-8"
            )
        )
    elif layout == "package":
        validate_block_coord_declaration(
            (output / "include/hxc/packages/caxecraft/domain/package.h").read_text(
                encoding="utf-8"
            )
        )
    return RenderedProject(
        output,
        normal_artifacts(output),
        hxcir,
        runtime_plan,
        projection,
        maintainability_report,
    )


def run_oracle() -> bytes:
    result = subprocess.run(
        [
            development_tool("haxe"),
            "--cwd",
            str(CASE),
            ORACLE_HXML.name,
        ],
        cwd=ROOT,
        env=haxe_environment("C", server=False),
        check=False,
        capture_output=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise CaxecraftFailure(
            "Eval oracle failed or emitted stderr\n"
            f"exit={result.returncode}\nstdout={result.stdout!r}\nstderr={result.stderr!r}"
        )
    lines = result.stdout.splitlines()
    if len(lines) != 38 or lines[0] != b"0" or not result.stdout.endswith(b"\n"):
        raise CaxecraftFailure(
            "Eval oracle must emit a zero self-check, five canonical hashes, and "
            "32 property hashes"
        )
    for line in lines:
        try:
            value = int(line.decode("ascii"))
        except (UnicodeError, ValueError) as error:
            raise CaxecraftFailure(f"Eval oracle emitted a non-integer line: {line!r}") from error
        if value < -(1 << 31) or value > (1 << 31) - 1:
            raise CaxecraftFailure(f"Eval oracle value is outside int32: {value}")
    return result.stdout


def first_difference(left: bytes, right: bytes) -> int:
    for index, (left_byte, right_byte) in enumerate(zip(left, right)):
        if left_byte != right_byte:
            return index
    return min(len(left), len(right))


def assert_artifacts_equal(
    left: dict[str, bytes], right: dict[str, bytes], label: str
) -> None:
    if left.keys() != right.keys():
        raise CaxecraftFailure(
            f"{label} artifact sets differ: {sorted(left)!r} != {sorted(right)!r}"
        )
    for path in left:
        if left[path] != right[path]:
            raise CaxecraftFailure(
                f"{label} first differs in {path!r} at byte "
                f"{first_difference(left[path], right[path])}"
            )


def alternate_locale() -> str:
    result = subprocess.run(
        ["locale", "-a"], check=False, capture_output=True, text=True, timeout=10
    )
    available = {line.strip() for line in result.stdout.splitlines()}
    # Prefer a language locale whose collation and formatting differ visibly
    # from C. Minimal CI images commonly provide only C.UTF-8, which remains a
    # useful fallback because it still exercises an explicit UTF-8 locale.
    for candidate in (
        "es_MX.UTF-8",
        "de_DE.UTF-8",
        "fr_FR.UTF-8",
        "en_US.UTF-8",
        "en_US.utf8",
        "C.UTF-8",
        "C.utf8",
    ):
        if candidate in available:
            return candidate
    return "C"


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise CaxecraftFailure(
                f"Haxe server exited early\nstdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise CaxecraftFailure("Haxe server did not accept connections")


@contextmanager
def haxe_compilation_server():
    """Own one loopback-only Haxe server and always reap its process.

    Haxe's server caches parsed and typed modules between requests. It still
    checks source-file changes and keeps separate caches for different define
    signatures. This helper does not weaken either check; it only avoids
    starting a new compiler process for every request.
    """

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    port = available_port()
    endpoint = f"127.0.0.1:{port}"
    server = subprocess.Popen(
        [str(installation.compiler), "--server-listen", endpoint],
        cwd=ROOT,
        env=pinned_haxe_environment("C", installation),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        yield HaxeServerConnection(endpoint, installation)
    finally:
        if server.poll() is None:
            server.terminate()
            try:
                server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                server.kill()
                server.wait(timeout=5)


def check_determinism(
    projects: tuple[tuple[str, RenderedProject], ...],
    root: Path,
    timing: TimingRecorder,
) -> None:
    with timing.phase("cold-determinism", haxe_requests=2 * len(projects)):
        for layout, first in projects:
            repeated = render_project(
                root / layout / "repeated",
                label=f"repeated cold {layout} Caxecraft render",
                layout=layout,
            )
            reversed_project = render_project(
                root / layout / "reversed",
                label=f"reverse-order/locale {layout} Caxecraft render",
                layout=layout,
                reverse=True,
                locale=alternate_locale(),
            )
            assert_artifacts_equal(
                first.artifacts, repeated.artifacts, f"repeated cold {layout} render"
            )
            assert_artifacts_equal(
                first.artifacts,
                reversed_project.artifacts,
                f"reverse-order/locale {layout} render",
            )

    with timing.phase("warm-server", haxe_requests=2 * len(projects)):
        with haxe_compilation_server() as endpoint:
            for layout, first in projects:
                warm_first = render_project(
                    root / layout / "warm-first",
                    label=f"first warm-server {layout} Caxecraft render",
                    layout=layout,
                    connect=endpoint,
                )
                warm_repeated = render_project(
                    root / layout / "warm-repeated",
                    label=f"repeated warm-server {layout} Caxecraft render",
                    layout=layout,
                    connect=endpoint,
                )
                assert_artifacts_equal(
                    first.artifacts, warm_first.artifacts, f"cold/warm {layout} render"
                )
                assert_artifacts_equal(
                    warm_first.artifacts,
                    warm_repeated.artifacts,
                    f"warm-server repeated {layout} render",
                )


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-snapshot-") as temporary:
        root = Path(temporary)
        split = render_project(
            root / "split",
            label="Caxecraft split snapshot render",
        )
        package = render_project(
            root / "package",
            label="Caxecraft package snapshot render",
            layout="package",
        )
        unity = render_project(
            root / "unity",
            label="Caxecraft unity snapshot render",
            layout="unity",
        )
        if (
            any(project.hxcir != split.hxcir for project in (package, unity))
            or any(
                project.runtime_plan != split.runtime_plan
                for project in (package, unity)
            )
            or any(
                project.method_symbols != split.method_symbols
                for project in (package, unity)
            )
        ):
            raise CaxecraftFailure(
                "split/package/unity snapshot renders changed semantic plans or names"
            )
        require_maintainability_layout_parity(
            split.maintainability_report,
            package.maintainability_report,
            unity.maintainability_report,
        )
        oracle = run_oracle().decode("ascii")
        playable = playable_snapshot_values()
        return {
            **{
                path: (split.output / path).read_text(encoding="utf-8")
                for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
            },
            **{
                f"package/{path}": (package.output / path).read_text(
                    encoding="utf-8"
                )
                for path in (*PACKAGE_HEADERS, *PACKAGE_SOURCES)
            },
            "hxc.runtime-plan.json": split.runtime_plan,
            "method-symbols.json": split.method_symbols,
            "maintainability-package.json": package.maintainability_report,
            "maintainability-split.json": split.maintainability_report,
            "maintainability-unity.json": unity.maintainability_report,
            "unity/include/hxc/program.h": (
                unity.output / "include/hxc/program.h"
            ).read_text(encoding="utf-8"),
            "unity/src/program.c": (unity.output / "src/program.c").read_text(
                encoding="utf-8"
            ),
            "oracle.txt": oracle,
            **playable,
        }


def expected_values() -> dict[str, object]:
    missing = [name for name in SNAPSHOT_FORMATS if not (EXPECTED / name).is_file()]
    if missing:
        raise CaxecraftFailure(
            "Caxecraft baseline is missing; run "
            "`npm run snapshots:update -- --suite caxecraft-domain`: "
            f"{missing!r}"
        )
    return {
        name: load_json(EXPECTED / name, f"expected {name}")
        if format_name == "json"
        else (EXPECTED / name).read_text(encoding="utf-8")
        for name, format_name in SNAPSHOT_FORMATS.items()
    }


def validate_expected(
    values: dict[str, object],
) -> tuple[dict[str, dict[str, bytes]], bytes]:
    oracle = values.get("oracle.txt")
    runtime_plan = values.get("hxc.runtime-plan.json")
    method_symbols = values.get("method-symbols.json")
    package_report = values.get("maintainability-package.json")
    split_report = values.get("maintainability-split.json")
    unity_report = values.get("maintainability-unity.json")
    split_text = {
        path: values.get(path) for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
    }
    package_text = {
        path: values.get(f"package/{path}")
        for path in (*PACKAGE_HEADERS, *PACKAGE_SOURCES)
    }
    unity_text = {
        "include/hxc/program.h": values.get("unity/include/hxc/program.h"),
        "src/program.c": values.get("unity/src/program.c"),
    }
    if not isinstance(oracle, str) or not all(
        isinstance(value, str)
        for value in (
            *split_text.values(),
            *package_text.values(),
            *unity_text.values(),
        )
    ):
        raise CaxecraftFailure("Caxecraft text baseline is malformed")
    if (
        not isinstance(runtime_plan, dict)
        or not isinstance(method_symbols, dict)
        or not isinstance(package_report, dict)
        or not isinstance(split_report, dict)
        or not isinstance(unity_report, dict)
    ):
        raise CaxecraftFailure("Caxecraft JSON baseline is malformed")
    validate_runtime_plan(runtime_plan)
    validate_method_symbols(method_symbols)
    split_bytes = {
        path: value.encode("utf-8")
        for path, value in split_text.items()
        if isinstance(value, str)
    }
    package_bytes = {
        path: value.encode("utf-8")
        for path, value in package_text.items()
        if isinstance(value, str)
    }
    unity_bytes = {
        path: value.encode("utf-8")
        for path, value in unity_text.items()
        if isinstance(value, str)
    }
    oracle_bytes = oracle.encode("ascii")
    validate_generated_text(
        b"\n".join(split_bytes[path] for path in SPLIT_HEADERS),
        b"\n".join(split_bytes[path] for path in SPLIT_SOURCES),
        method_symbols,
    )
    validate_generated_text(
        b"\n".join(package_bytes[path] for path in PACKAGE_HEADERS),
        b"\n".join(package_bytes[path] for path in PACKAGE_SOURCES),
        method_symbols,
    )
    validate_generated_text(
        unity_bytes["include/hxc/program.h"],
        unity_bytes["src/program.c"],
        method_symbols,
    )
    validate_maintainability_inputs(split_report, "split", split_bytes)
    validate_maintainability_inputs(package_report, "package", package_bytes)
    validate_maintainability_inputs(unity_report, "unity", unity_bytes)
    require_maintainability_layout_parity(split_report, package_report, unity_report)
    block_coord = split_text.get(
        "include/hxc/modules/caxecraft/domain/BlockCoord.h"
    )
    if not isinstance(block_coord, str):
        raise CaxecraftFailure("checked-in Caxecraft baseline omitted BlockCoord.h")
    validate_block_coord_header(block_coord)
    package_domain = package_text.get(
        "include/hxc/packages/caxecraft/domain/package.h"
    )
    if not isinstance(package_domain, str):
        raise CaxecraftFailure(
            "checked-in Caxecraft package baseline omitted its domain header"
        )
    validate_block_coord_declaration(package_domain)
    lines = oracle_bytes.splitlines()
    if len(lines) != 38 or lines[0] != b"0" or not oracle_bytes.endswith(b"\n"):
        raise CaxecraftFailure("checked-in Caxecraft oracle baseline drifted")
    return {
        "split": split_bytes,
        "package": package_bytes,
        "unity": unity_bytes,
    }, oracle_bytes


def validate_snapshots(
    split: RenderedProject,
    package: RenderedProject,
    unity: RenderedProject,
    oracle: bytes,
) -> None:
    expected = expected_values()
    playable = playable_snapshot_values()
    actual: dict[str, object] = {
        **{
            path: (split.output / path).read_text(encoding="utf-8")
            for path in (*SPLIT_HEADERS, *SPLIT_SOURCES)
        },
        **{
            f"package/{path}": (package.output / path).read_text(encoding="utf-8")
            for path in (*PACKAGE_HEADERS, *PACKAGE_SOURCES)
        },
        "hxc.runtime-plan.json": split.runtime_plan,
        "method-symbols.json": split.method_symbols,
        "maintainability-package.json": package.maintainability_report,
        "maintainability-split.json": split.maintainability_report,
        "maintainability-unity.json": unity.maintainability_report,
        "unity/include/hxc/program.h": (
            unity.output / "include/hxc/program.h"
        ).read_text(encoding="utf-8"),
        "unity/src/program.c": (unity.output / "src/program.c").read_text(
            encoding="utf-8"
        ),
        "oracle.txt": oracle.decode("ascii"),
        **playable,
    }
    if actual == expected:
        return
    for name in SNAPSHOT_FORMATS:
        if actual[name] != expected[name]:
            if isinstance(actual[name], str) and isinstance(expected[name], str):
                difference = "".join(
                    difflib.unified_diff(
                        expected[name].splitlines(keepends=True),
                        actual[name].splitlines(keepends=True),
                        fromfile=f"expected/{name}",
                        tofile=f"actual/{name}",
                        n=3,
                    )
                )
                detail = difference[:4000]
            else:
                detail = "semantic JSON differs"
            raise CaxecraftFailure(
                f"Caxecraft snapshot {name!r} drifted; run "
                "`npm run snapshots:update -- --suite caxecraft-domain`\n"
                f"{detail}"
            )


def prepare_native_fixture(
    fixture: Path, project: RenderedProject, layout: str
) -> None:
    shutil.copytree(project.output / "include", fixture / "generated/include")
    (fixture / "generated/src").mkdir(parents=True)
    (fixture / "native").mkdir(parents=True)
    shutil.copy2(NATIVE / "domain_harness.c", fixture / "native/domain_harness.c")
    (fixture / "native/method_symbols.h").write_text(
        native_method_symbol_header(project.method_symbols), encoding="utf-8"
    )
    if layout == "unity":
        shutil.copy2(
            project.output / "src/program.c", fixture / "generated/src/program.c"
        )
        shutil.copy2(
            NATIVE / "generated_program.c", fixture / "native/generated_program.c"
        )
    elif layout in ("split", "package"):
        for relative in SOURCES_BY_LAYOUT[layout]:
            if relative == "src/hxc/main.c":
                continue
            destination = fixture / "generated" / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(project.output / relative, destination)
    else:
        raise CaxecraftFailure(f"unknown native Caxecraft layout {layout!r}")


def native_project(layout: str, oracle: bytes, *, sanitizer: bool) -> CFixtureProject:
    if layout in ("split", "package"):
        sources = (
            "native/domain_harness.c",
            *(
                f"generated/{path}"
                for path in SOURCES_BY_LAYOUT[layout]
                if path != "src/hxc/main.c"
            ),
        )
        headers = (
            "native/method_symbols.h",
            *(f"generated/{path}" for path in HEADERS_BY_LAYOUT[layout]),
        )
    elif layout == "unity":
        sources = ("native/domain_harness.c", "native/generated_program.c")
        headers = (
            "native/method_symbols.h",
            "generated/include/hxc/program.h",
            "generated/src/program.c",
        )
    else:
        raise CaxecraftFailure(f"unknown native Caxecraft layout {layout!r}")
    return CFixtureProject(
        identifier=f"caxecraft-domain-{layout}",
        sources=sources,
        headers=headers,
        include_directories=("generated/include",),
        expected_stdout=oracle.decode("ascii"),
        coverage=tuple(sorted(COVERAGE)),
        link_arguments=SANITIZER_FLAGS if sanitizer else (),
    )


def inspect_generated_object_symbols(
    build_root: Path, report: dict[str, object], layout: str
) -> None:
    toolchains = report.get("toolchains")
    if not isinstance(toolchains, list):
        raise CaxecraftFailure("native Caxecraft report omitted toolchains")
    for entry in toolchains:
        if not isinstance(entry, dict) or not isinstance(entry.get("family"), str):
            raise CaxecraftFailure("native Caxecraft report has a malformed toolchain")
        family = entry["family"]
        project_root = build_root / family / f"caxecraft-domain-{layout}"
        objects = sorted(project_root.glob("*.o"))
        if not objects:
            raise CaxecraftFailure(
                f"cannot inspect {family} generated Caxecraft object symbols"
            )
        imported: set[str] = set()
        for generated_object in objects:
            result = subprocess.run(
                ["nm", "-u", str(generated_object)],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if result.returncode != 0:
                raise CaxecraftFailure(
                    f"cannot inspect {family} generated Caxecraft object symbols\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
            imported.update(
                line.split()[-1].lstrip("_").lower()
                for line in result.stdout.splitlines()
                if line.split()
            )
        forbidden = sorted(
            symbol
            for symbol in imported
            if symbol in {"malloc", "calloc", "realloc", "free"}
            or symbol.startswith("hxrt_")
        )
        if forbidden:
            raise CaxecraftFailure(
                f"{family} generated Caxecraft object imports forbidden "
                f"symbols: {', '.join(forbidden)}"
            )


def check_standalone_headers(
    project: RenderedProject, layout: str, requested_toolchain: str
) -> None:
    headers = HEADERS_BY_LAYOUT.get(layout)
    if headers is None:
        raise CaxecraftFailure(f"unknown standalone-header layout {layout!r}")
    include_root = project.output / "include"
    for toolchain in resolve_toolchains(
        requested_toolchain, repository_root=ROOT
    ):
        for header in headers:
            result = subprocess.run(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    "-I",
                    str(include_root),
                    "-x",
                    "c",
                    "-fsyntax-only",
                    "-",
                ],
                input=f'#include "{header.removeprefix("include/")}"\n',
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if result.returncode != 0 or result.stdout or result.stderr:
                raise CaxecraftFailure(
                    f"{toolchain.family} rejected standalone {layout} header "
                    f"{header}\n{result.stdout}{result.stderr}"
                )


def run_native(
    project: RenderedProject,
    layout: str,
    oracle: bytes,
    *,
    requested_toolchain: str,
    root: Path,
    full: bool,
) -> None:
    fixture = root / f"fixture-{layout}"
    prepare_native_fixture(fixture, project, layout)
    if full:
        progress(f"standalone {layout} headers")
        check_standalone_headers(project, layout, requested_toolchain)
    modes = (
        (
            ("o0", ("-O0",), False),
            ("o2", ("-O2",), False),
            ("sanitizer", ("-O1", *SANITIZER_FLAGS), True),
        )
        if full
        else (("o2", ("-O2",), False),)
    )
    for mode, extra_flags, sanitizer in modes:
        progress(f"native {layout}/{mode}")
        build_root = root / mode
        report = run_c_fixture_corpus(
            suite=f"caxecraft-domain-{layout}-{mode}",
            projects=(native_project(layout, oracle, sanitizer=sanitizer),),
            fixture_root=fixture,
            build_root=build_root,
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*STRICT_FLAGS, *extra_flags),
            timeout_seconds=120,
        )
        validate_report(report, required_coverage=COVERAGE)
        inspect_generated_object_symbols(build_root, report, layout)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile and run the checked-in generated baseline without Haxe",
    )
    parser.add_argument(
        "--full",
        action="store_true",
        help="run the exhaustive determinism and O0/O2/sanitizer CI matrix",
    )
    parser.add_argument(
        "--timing-report",
        type=Path,
        default=(
            Path(os.environ["HXC_CAXECRAFT_TIMING_REPORT"])
            if "HXC_CAXECRAFT_TIMING_REPORT" in os.environ
            else None
        ),
        help="write path-free phase and Haxe-request timing JSON",
    )
    return parser.parse_args(list(argv))


def progress(stage: str) -> None:
    print(f"caxecraft-domain: [{stage}]", flush=True)


def checked_in_projects(
    root: Path, values: dict[str, object]
) -> tuple[dict[str, RenderedProject], bytes]:
    generated_by_layout, oracle = validate_expected(values)
    runtime_plan = values.get("hxc.runtime-plan.json")
    method_symbols = values.get("method-symbols.json")
    if (
        not isinstance(runtime_plan, dict)
        or not isinstance(method_symbols, dict)
    ):
        raise CaxecraftFailure("checked-in Caxecraft JSON baseline is malformed")
    projects: dict[str, RenderedProject] = {}
    for layout in LAYOUTS:
        output = root / layout
        for relative, content in generated_by_layout[layout].items():
            destination = output / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(content)
        maintainability_report = values.get(f"maintainability-{layout}.json")
        if not isinstance(maintainability_report, dict):
            raise CaxecraftFailure(
                f"checked-in Caxecraft {layout} maintainability baseline is malformed"
            )
        projects[layout] = RenderedProject(
            output,
            {},
            "",
            runtime_plan,
            method_symbols,
            maintainability_report,
        )
    return projects, oracle


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    timing = TimingRecorder()
    timing_mode = "native-only" if args.native_only else ("full" if args.full else "quick")
    try:
        progress("asset + localization contracts")
        with timing.phase("asset-contracts"):
            validate_asset_pack(CASE / "assets")
            negative_contracts()
            check_generated()
            check_app_boundary()
            check_localization_negative_contracts()
        with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-domain-") as temporary:
            root = Path(temporary)
            if args.native_only:
                progress("load checked-in split/package/unity baselines")
                with timing.phase("checked-in-load"):
                    checked_in, oracle = checked_in_projects(
                        root / "checked-in", expected_values()
                    )
                    split = checked_in["split"]
                    package = checked_in["package"]
                    unity = checked_in["unity"]
            else:
                progress("Eval oracle")
                with timing.phase("eval-oracle", haxe_requests=1):
                    oracle = run_oracle()
                with timing.phase("backend-render", haxe_requests=3):
                    progress("cold split render + HxcIR")
                    first = render_project(
                        root / "first",
                        label="first cold Caxecraft render",
                    )
                    progress("cold package render + semantic parity")
                    package = render_project(
                        root / "package",
                        label="cold package Caxecraft render",
                        layout="package",
                    )
                    progress("cold unity render + semantic parity")
                    unity = render_project(
                        root / "unity",
                        label="cold unity Caxecraft render",
                        layout="unity",
                    )
                    if (
                        any(
                            project.hxcir != first.hxcir
                            for project in (package, unity)
                        )
                        or any(
                            project.runtime_plan != first.runtime_plan
                            for project in (package, unity)
                        )
                        or any(
                            project.method_symbols != first.method_symbols
                            for project in (package, unity)
                        )
                    ):
                        raise CaxecraftFailure(
                            "split, package, and unity layouts changed HxcIR, "
                            "runtime, or method symbols"
                        )
                    require_maintainability_layout_parity(
                        first.maintainability_report,
                        package.maintainability_report,
                        unity.maintainability_report,
                    )
                if args.full:
                    progress("split/package cold/reversed/locale/warm determinism")
                    check_determinism(
                        (("split", first), ("package", package)),
                        root / "determinism",
                        timing,
                    )
                progress("checked-in split/package/unity snapshots")
                with timing.phase("snapshot-validation"):
                    validate_snapshots(first, package, unity, oracle)
                split = first
            with timing.phase("native-build-run"):
                progress("split native differential")
                run_native(
                    split,
                    "split",
                    oracle,
                    requested_toolchain=args.toolchain,
                    root=root / "native-split",
                    full=args.full or args.native_only,
                )
                if package is not None:
                    progress("package native differential")
                    run_native(
                        package,
                        "package",
                        oracle,
                        requested_toolchain=args.toolchain,
                        root=root / "native-package",
                        full=args.full or args.native_only,
                    )
                if unity is not None:
                    progress("unity native differential")
                    run_native(
                        unity,
                        "unity",
                        oracle,
                        requested_toolchain=args.toolchain,
                        root=root / "native-unity",
                        full=args.full or args.native_only,
                    )
    except (
        AssetValidationError,
        CFixtureFailure,
        CaxecraftFailure,
        LocalizationCheckFailure,
        MaintainabilityError,
        OSError,
        UnicodeError,
        ValueError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        if args.timing_report is not None:
            write_timing_report(
                args.timing_report,
                timing.report(mode=timing_mode, outcome="failed"),
            )
        print(f"caxecraft-domain: ERROR: {error}", file=sys.stderr)
        return 1

    if args.timing_report is not None:
        write_timing_report(
            args.timing_report,
            timing.report(mode=timing_mode, outcome="passed"),
        )

    mode = (
        "checked-in split/package/unity C baselines"
        if args.native_only
        else "Eval/split+package+unity generated-C differential"
    )
    matrix = "full O0/O2/ASan+UBSan" if args.full or args.native_only else "quick O2"
    parity = (
        "checked-in split/package/unity layout validation"
        if args.native_only
        else "split/package/unity layout semantic parity"
    )
    print(
        "caxecraft-domain: OK: "
        f"{mode}, 32 seeded properties, exact traces, {matrix}, "
        f"zero hxrt/allocation symbols, schema-checked C maintainability, and {parity} passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
