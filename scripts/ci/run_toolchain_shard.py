#!/usr/bin/env python3
"""Validate or run one isolated shard of the canonical toolchain gate."""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import math
import os
import platform
import shutil
import subprocess
import sys
import tempfile
import time
from collections.abc import Mapping
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, TextIO


ROOT = Path(__file__).resolve().parents[2]
PACKAGE = ROOT / "package.json"

# Keep these as contiguous slices of package.json's canonical test:toolchain
# sequence. validate_partition() rejects missing, duplicated, unknown, or
# reordered entries before any suite executes.
SHARD_ORDER = (
    "contracts",
    "lowering-objects",
    "lowering-semantics",
    "caxecraft",
)

SHARDS: dict[str, tuple[str, ...]] = {
    "contracts": (
        "deps:verify",
        "test:beads-plan",
        "test:diagnostics",
        "test:hxc-config",
        "test:all-sources",
        "test:bootstrap",
        "test:typed-c",
        "test:c-import",
        "test:raylib-provisioning",
        "test:typed-ast",
        "test:c-ast",
        "test:declaration-plan",
        "test:symbol-registry",
        "test:project-emitter",
        "test:runtime-features",
        "test:array-runtime",
        "test:string-runtime",
        "test:string-output",
        "test:hello",
        "test:hxc-ir",
        "test:primitive-semantics",
        "test:stdlib-ledger",
    ),
    "lowering-objects": (
        "test:body-lowering",
        "test:function-lowering",
        "test:aggregate-lowering",
        "test:class-layout",
        "test:constructor-lowering",
        "test:virtual-dispatch",
        "test:enum-lowering",
        "test:generic-specialization",
    ),
    "lowering-semantics": (
        "test:evaluation-order",
        "test:static-initialization",
        "test:arithmetic-semantics",
        "test:primitive-differential",
        "test:span-lowering",
        "test:project-layout",
    ),
    "caxecraft": (
        "test:caxecraft-localization",
        "test:caxecraft-inventory",
        "test:caxecraft-gameplay",
        "test:caxecraft-pilot",
        "test:caxecraft-scenario-model",
        "test:caxecraft-caxeflow",
        "test:caxecraft-editor",
        "test:caxecraft-scenario-determinism",
        "test:caxecraft-domain:full",
        "snapshots:catalog",
    ),
}

# Each admitted local-parallel shard writes normal test output only below
# suite-owned TemporaryDirectory roots, binds compiler servers to ephemeral
# loopback ports, and treats repository inputs as read-only. Tests that rely on
# warm server reuse or stale-output transitions remain serial inside a shard.
LOCAL_PARALLEL_ISOLATION: dict[str, str] = {
    "contracts": "temporary-roots+ephemeral-ports",
    "lowering-objects": "temporary-roots+ephemeral-ports",
    "lowering-semantics": "temporary-roots+ephemeral-ports",
    "caxecraft": "temporary-roots+ephemeral-ports",
}

MAX_LOCAL_JOBS = len(SHARD_ORDER)
EVIDENCE_SCHEMA_VERSION = 1
EVIDENCE_TTL_SECONDS = 24 * 60 * 60
DEFAULT_EVIDENCE_DIR = ROOT / ".cache" / "toolchain-shards"
EVIDENCE_MARKER = ".hxc-toolchain-evidence.json"
EVIDENCE_MARKER_PAYLOAD = {
    "schemaVersion": EVIDENCE_SCHEMA_VERSION,
    "owner": "reflaxe.c-local-toolchain-shards",
}
CAXECRAFT_SCRIPT = "test:caxecraft-domain:full"
CAXECRAFT_TIMING_ENV = "HXC_CAXECRAFT_TIMING_REPORT"
CAXECRAFT_FULL_PHASES = (
    ("asset-contracts", 0),
    ("eval-oracle", 1),
    ("backend-render", 3),
    ("cold-determinism", 4),
    ("warm-server", 4),
    ("snapshot-validation", 0),
    ("native-build-run", 0),
)

# These locks are called out separately in the evidence record so a reviewer
# can see which dependency contracts participated in the key. The staged Git
# tree also covers them, but that broad hash is intentionally not the only
# explanation for why old evidence stopped matching.
EVIDENCE_LOCK_PATHS = (
    Path(".haxerc"),
    Path("package-lock.json"),
    Path("docs/specs/toolchain-lock.json"),
    Path("docs/specs/raylib-provisioning-lock.json"),
    Path("docs/specs/raylib-core-binding-lock.json"),
)

# Shards inherit the runner's complete environment, so an arbitrary variable
# may select a Python module, Node behavior, SDK, compiler, or build mode. Only
# these two values are proven not to change shard semantics: one controls outer
# worker count, and the other is replaced with a private report path before the
# Caxecraft command starts.
EVIDENCE_ENVIRONMENT_EXCLUSIONS = frozenset(
    ("HXC_TEST_JOBS", CAXECRAFT_TIMING_ENV)
)

# The shard family invokes these tools directly or through its focused
# runners. Missing tools are identities too: a later installation must
# invalidate a cached success instead of inheriting it.
EVIDENCE_TOOLS = (
    "bash",
    "git",
    "python3",
    "node",
    "npm",
    "haxe",
    "haxelib",
    "lix",
    "cc",
    "gcc",
    "g++",
    "clang",
    "clang++",
    "cmake",
    "make",
    "meson",
    "nm",
    "ninja",
    "pkg-config",
    "jq",
    "locale",
)

RELEVANT_UNTRACKED_ROOTS = (
    ".github",
    "docs/specs",
    "examples",
    "haxe_libraries",
    "runtime",
    "scripts",
    "src",
    "std",
    "test",
)


@dataclass(frozen=True)
class JobSelection:
    jobs: int
    source: str
    logical_cpus: int
    one_minute_load: float | None
    reason: str


class ToolchainShardFailure(Exception):
    """Raised when the shard contract or one of its commands fails."""


def canonical_json_bytes(value: object) -> bytes:
    return json.dumps(
        value,
        ensure_ascii=False,
        sort_keys=True,
        separators=(",", ":"),
    ).encode("utf-8")


def sha256_bytes(value: bytes) -> str:
    return hashlib.sha256(value).hexdigest()


def digest_files(paths: Iterable[Path]) -> str:
    digest = hashlib.sha256()
    for relative in sorted(paths, key=lambda item: item.as_posix().encode("utf-8")):
        path = ROOT / relative
        try:
            payload = path.read_bytes()
        except OSError as error:
            raise ToolchainShardFailure(
                f"cannot hash evidence input {relative.as_posix()}: {error}"
            ) from error
        encoded = relative.as_posix().encode("utf-8")
        digest.update(len(encoded).to_bytes(8, "big"))
        digest.update(encoded)
        digest.update(len(payload).to_bytes(8, "big"))
        digest.update(payload)
    return digest.hexdigest()


def git_bytes(arguments: list[str]) -> bytes:
    try:
        result = subprocess.run(
            ["git", *arguments],
            cwd=ROOT,
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except OSError as error:
        raise ToolchainShardFailure(f"cannot run git {' '.join(arguments)}: {error}") from error
    if result.returncode != 0:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        raise ToolchainShardFailure(
            f"git {' '.join(arguments)} failed with exit {result.returncode}: {detail}"
        )
    return result.stdout


def staged_tree_identity() -> str:
    tree = git_bytes(["write-tree"]).decode("ascii", errors="strict").strip()
    if len(tree) not in (40, 64) or any(
        character not in "0123456789abcdef" for character in tree
    ):
        raise ToolchainShardFailure(f"git write-tree returned invalid identity {tree!r}")
    return tree


def relevant_worktree_digest() -> str:
    """Hash execution inputs that are not represented by the staged tree.

    Pre-commit runs against the working checkout, so an unstaged compiler edit
    must invalidate earlier evidence even though the staged Git tree did not
    change. Relevant untracked files are included too; unrelated root-level
    scratch artifacts do not make an otherwise identical retry expensive.
    """

    digest = hashlib.sha256()
    tracked = git_bytes(["ls-files", "-z", "--", *RELEVANT_UNTRACKED_ROOTS])
    for encoded_name in (name for name in tracked.split(b"\0") if name):
        try:
            name = encoded_name.decode("utf-8", errors="strict")
        except UnicodeDecodeError as error:
            raise ToolchainShardFailure(
                "relevant tracked input path is not valid UTF-8"
            ) from error
        if (ROOT / name).is_symlink():
            raise ToolchainShardFailure(
                f"relevant tracked input must not be a symlink: {name}"
            )
    tracked_diff = git_bytes(["diff", "--binary", "--no-ext-diff", "--", "."])
    digest.update(len(tracked_diff).to_bytes(8, "big"))
    digest.update(tracked_diff)

    untracked = git_bytes(
        [
            "ls-files",
            "-z",
            "--others",
            "--exclude-standard",
            "--",
            *RELEVANT_UNTRACKED_ROOTS,
        ]
    )
    names = sorted(
        (name for name in untracked.split(b"\0") if name),
        key=lambda item: item,
    )
    for encoded_name in names:
        try:
            name = encoded_name.decode("utf-8", errors="strict")
        except UnicodeDecodeError as error:
            raise ToolchainShardFailure(
                "relevant untracked input path is not valid UTF-8"
            ) from error
        path = ROOT / name
        try:
            if path.is_symlink():
                raise ToolchainShardFailure(
                    f"relevant untracked input must not be a symlink: {name}"
                )
            payload = path.read_bytes()
            kind = b"file"
        except ToolchainShardFailure:
            raise
        except (OSError, UnicodeError) as error:
            raise ToolchainShardFailure(
                f"cannot hash relevant untracked input {name}: {error}"
            ) from error
        digest.update(len(encoded_name).to_bytes(8, "big"))
        digest.update(encoded_name)
        digest.update(kind)
        digest.update(len(payload).to_bytes(8, "big"))
        digest.update(payload)
    return digest.hexdigest()


def environment_digest(environment: Mapping[str, str]) -> str:
    values = {
        name: sha256_bytes(value.encode("utf-8"))
        for name, value in sorted(environment.items())
        if name not in EVIDENCE_ENVIRONMENT_EXCLUSIONS
    }
    return sha256_bytes(canonical_json_bytes(values))


def host_identity_digest() -> str:
    locale_executable = shutil.which("locale")
    locale_inventory: dict[str, object]
    if locale_executable is None:
        locale_inventory = {"available": False}
    else:
        try:
            result = subprocess.run(
                [locale_executable, "-a"],
                cwd=ROOT,
                check=False,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                timeout=10,
            )
        except (OSError, subprocess.TimeoutExpired) as error:
            raise ToolchainShardFailure(
                f"cannot identify installed locale inventory: {error}"
            ) from error
        locale_inventory = {
            "available": True,
            "exitCode": result.returncode,
            "outputSha256": sha256_bytes(result.stdout),
        }
    return sha256_bytes(
        canonical_json_bytes(
            {
                "osName": os.name,
                "sysPlatform": sys.platform,
                "machine": platform.machine(),
                "pythonImplementation": platform.python_implementation(),
                "pythonVersion": platform.python_version(),
                "localeInventory": locale_inventory,
            }
        )
    )


def resolve_evidence_tool(tool: str) -> str | None:
    """Resolve a tool the same way an npm script does.

    npm prepends this checkout's `node_modules/.bin` directory to PATH. The
    evidence key must therefore describe the pinned local Haxe/Lix shim rather
    than an unrelated global command with the same name.
    """

    local_executable = ROOT / "node_modules/.bin" / tool
    return str(local_executable) if local_executable.is_file() else shutil.which(tool)


def tool_identity_digest() -> str:
    identities: dict[str, object] = {}
    for tool in EVIDENCE_TOOLS:
        executable = resolve_evidence_tool(tool)
        if executable is None:
            identities[tool] = {"available": False}
            continue
        resolved = Path(executable).resolve()
        try:
            executable_digest = sha256_bytes(resolved.read_bytes())
        except OSError as error:
            raise ToolchainShardFailure(f"cannot hash tool {tool}: {error}") from error
        version_argument = "--version"
        try:
            result = subprocess.run(
                [executable, version_argument],
                cwd=ROOT,
                check=False,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                timeout=10,
            )
        except (OSError, subprocess.TimeoutExpired) as error:
            raise ToolchainShardFailure(f"cannot identify tool {tool}: {error}") from error
        identities[tool] = {
            "available": True,
            "binarySha256": executable_digest,
            "versionExitCode": result.returncode,
            "versionOutputSha256": sha256_bytes(result.stdout),
        }
        if tool in ("cc", "gcc", "g++", "clang", "clang++"):
            try:
                target = subprocess.run(
                    [executable, "-dumpmachine"],
                    cwd=ROOT,
                    check=False,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    timeout=10,
                )
            except (OSError, subprocess.TimeoutExpired) as error:
                raise ToolchainShardFailure(
                    f"cannot identify native target for {tool}: {error}"
                ) from error
            identities[tool]["targetExitCode"] = target.returncode
            identities[tool]["targetOutputSha256"] = sha256_bytes(target.stdout)
    return sha256_bytes(canonical_json_bytes(identities))


def command_definition_digest(shard: str, scripts: dict[str, str]) -> str:
    registry = [
        {
            "shard": registered_shard,
            "isolation": LOCAL_PARALLEL_ISOLATION[registered_shard],
            "commands": [
                {"script": script, "command": scripts[script]}
                for script in SHARDS[registered_shard]
            ],
        }
        for registered_shard in SHARD_ORDER
    ]
    return sha256_bytes(
        canonical_json_bytes(
            {
                "selectedShard": shard,
                "canonicalSequence": list(canonical_sequence(scripts)),
                "registry": registry,
            }
        )
    )


def build_evidence_inputs(
    shard: str,
    scripts: dict[str, str],
    *,
    staged_tree: str,
    worktree_digest: str,
    locks_digest: str,
    tools_digest: str,
    env_digest: str,
    runner_digest: str,
    hook_digest: str,
    host_digest: str,
) -> dict[str, str]:
    return {
        "stagedTree": staged_tree,
        "worktreeDigest": worktree_digest,
        "commandDigest": command_definition_digest(shard, scripts),
        "locksDigest": locks_digest,
        "toolsDigest": tools_digest,
        "environmentDigest": env_digest,
        "runnerDigest": runner_digest,
        "hookDigest": hook_digest,
        "hostDigest": host_digest,
    }


def evidence_key(inputs: dict[str, str]) -> str:
    return sha256_bytes(canonical_json_bytes(inputs))


def load_scripts() -> dict[str, str]:
    try:
        package = json.loads(PACKAGE.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise ToolchainShardFailure(f"cannot read package.json: {error}") from error
    if not isinstance(package, dict) or not isinstance(package.get("scripts"), dict):
        raise ToolchainShardFailure("package.json scripts must be an object")
    scripts = package["scripts"]
    if not all(isinstance(name, str) and isinstance(value, str) for name, value in scripts.items()):
        raise ToolchainShardFailure("package.json scripts must map strings to strings")
    return scripts


def canonical_sequence(scripts: dict[str, str]) -> tuple[str, ...]:
    command = scripts.get("test:toolchain")
    if not isinstance(command, str) or not command:
        raise ToolchainShardFailure("package.json has no canonical test:toolchain command")

    sequence: list[str] = []
    for position, part in enumerate(command.split(" && "), start=1):
        prefix = "npm run "
        if not part.startswith(prefix):
            raise ToolchainShardFailure(
                f"canonical command {position} is not an npm script invocation: {part!r}"
            )
        script = part[len(prefix) :]
        if not script or any(character.isspace() for character in script):
            raise ToolchainShardFailure(
                f"canonical command {position} has unsupported arguments: {part!r}"
            )
        sequence.append(script)
    return tuple(sequence)


def validate_partition(scripts: dict[str, str]) -> tuple[str, ...]:
    if tuple(SHARDS) != SHARD_ORDER:
        raise ToolchainShardFailure("SHARDS keys must exactly follow SHARD_ORDER")

    flattened = tuple(script for shard in SHARD_ORDER for script in SHARDS[shard])
    canonical = canonical_sequence(scripts)
    if flattened != canonical:
        missing = [script for script in canonical if script not in flattened]
        extra = [script for script in flattened if script not in canonical]
        duplicate = sorted(
            script for script in set(flattened) if flattened.count(script) > 1
        )
        details = []
        if missing:
            details.append("missing=" + ",".join(missing))
        if extra:
            details.append("extra=" + ",".join(extra))
        if duplicate:
            details.append("duplicate=" + ",".join(duplicate))
        if not details:
            mismatch = next(
                index
                for index, (actual, expected) in enumerate(
                    zip(flattened, canonical), start=1
                )
                if actual != expected
            )
            details.append(
                f"order[{mismatch}]={flattened[mismatch - 1]!r},"
                f"expected={canonical[mismatch - 1]!r}"
            )
        raise ToolchainShardFailure(
            "toolchain shards do not exactly partition test:toolchain: "
            + "; ".join(details)
        )

    unknown = sorted(script for script in flattened if script not in scripts)
    if unknown:
        raise ToolchainShardFailure(
            "toolchain shards reference unknown package scripts: " + ", ".join(unknown)
        )
    if tuple(LOCAL_PARALLEL_ISOLATION) != SHARD_ORDER:
        raise ToolchainShardFailure(
            "local parallel isolation declarations must exactly cover SHARD_ORDER"
        )
    return canonical


def elapsed_milliseconds(start_ns: int) -> int:
    return max(0, (time.monotonic_ns() - start_ns + 500_000) // 1_000_000)


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


def collect_evidence_inputs(
    scripts: dict[str, str],
) -> dict[str, dict[str, str]]:
    common = {
        "staged_tree": staged_tree_identity(),
        "worktree_digest": relevant_worktree_digest(),
        "locks_digest": digest_files(EVIDENCE_LOCK_PATHS),
        "tools_digest": tool_identity_digest(),
        "env_digest": environment_digest(os.environ),
        "runner_digest": sha256_bytes(Path(__file__).read_bytes()),
        "hook_digest": sha256_bytes((ROOT / "scripts/hooks/pre-commit").read_bytes()),
        "host_digest": host_identity_digest(),
    }
    return {
        shard: build_evidence_inputs(shard, scripts, **common)
        for shard in SHARD_ORDER
    }


def evidence_record(
    shard: str,
    inputs: dict[str, str],
    *,
    now: int,
    report_digest: str = "0" * 64,
) -> dict[str, object]:
    return {
        "schemaVersion": EVIDENCE_SCHEMA_VERSION,
        "shard": shard,
        "outcome": "passed",
        "key": evidence_key(inputs),
        "reportDigest": report_digest,
        "createdAtUnix": now,
        "expiresAtUnix": now + EVIDENCE_TTL_SECONDS,
        "inputs": inputs,
    }


def validate_reusable_evidence(
    payload: object,
    shard: str,
    inputs: dict[str, str],
    *,
    now: int,
) -> tuple[bool, str, int | None]:
    if not isinstance(payload, dict):
        return False, "record is not a JSON object", None
    expected_fields = {
        "schemaVersion",
        "shard",
        "outcome",
        "key",
        "reportDigest",
        "createdAtUnix",
        "expiresAtUnix",
        "inputs",
    }
    if set(payload) != expected_fields:
        return False, "record fields do not match the closed schema", None
    if payload.get("schemaVersion") != EVIDENCE_SCHEMA_VERSION:
        return False, "record schema version does not match", None
    if payload.get("shard") != shard or payload.get("outcome") != "passed":
        return False, "record does not describe a passing result for this shard", None
    created = payload.get("createdAtUnix")
    expires = payload.get("expiresAtUnix")
    if (
        not isinstance(created, int)
        or isinstance(created, bool)
        or not isinstance(expires, int)
        or isinstance(expires, bool)
        or expires != created + EVIDENCE_TTL_SECONDS
    ):
        return False, "record lifetime is malformed", None
    if created > now:
        return False, "record comes from the future", None
    if expires <= now:
        return False, "record has expired", None
    if payload.get("inputs") != inputs:
        return False, "one or more evidence inputs changed", None
    expected_key = evidence_key(inputs)
    if payload.get("key") != expected_key:
        return False, "record key does not match its current inputs", None
    report_digest = payload.get("reportDigest")
    if (
        not isinstance(report_digest, str)
        or len(report_digest) != 64
        or any(character not in "0123456789abcdef" for character in report_digest)
    ):
        return False, "record report digest is malformed", None
    return True, "exact passing evidence is reusable", now - created


def read_reusable_evidence(
    path: Path,
    shard: str,
    inputs: dict[str, str],
    *,
    now: int,
) -> tuple[bool, str, int | None]:
    if path.is_symlink():
        return False, "record path is a symlink", None
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        return False, "record is missing", None
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        return False, f"record cannot be read: {error}", None
    return validate_reusable_evidence(payload, shard, inputs, now=now)


def write_success_evidence(
    path: Path,
    shard: str,
    inputs: dict[str, str],
    *,
    now: int,
    report_digest: str,
) -> None:
    write_timing_report(
        path,
        evidence_record(shard, inputs, now=now, report_digest=report_digest),
    )


def reject_symlink_ancestry(path: Path) -> None:
    absolute = path.absolute()
    current = Path(absolute.anchor)
    for part in absolute.parts[1:]:
        current /= part
        if current.is_symlink():
            raise ToolchainShardFailure(
                f"toolchain evidence path has a symlink ancestor: {current}"
            )


def prepare_evidence_directory(path: Path) -> None:
    reject_symlink_ancestry(path)
    if path.exists() and not path.is_dir():
        raise ToolchainShardFailure("toolchain evidence path is not a directory")
    path.mkdir(parents=True, exist_ok=True)
    allowed = {EVIDENCE_MARKER, *(f"{shard}.json" for shard in SHARD_ORDER)}
    entries = sorted(path.iterdir(), key=lambda item: item.name.encode("utf-8"))
    marker = path / EVIDENCE_MARKER
    if not marker.exists():
        if entries:
            raise ToolchainShardFailure(
                "toolchain evidence directory is nonempty but has no ownership marker"
            )
        write_timing_report(marker, EVIDENCE_MARKER_PAYLOAD)
        entries = [marker]
    if marker.is_symlink():
        raise ToolchainShardFailure("toolchain evidence ownership marker is a symlink")
    try:
        marker_payload = json.loads(marker.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise ToolchainShardFailure(
            f"cannot validate toolchain evidence ownership marker: {error}"
        ) from error
    if marker_payload != EVIDENCE_MARKER_PAYLOAD:
        raise ToolchainShardFailure("toolchain evidence ownership marker does not match")
    for entry in entries:
        if entry.name not in allowed:
            raise ToolchainShardFailure(
                f"toolchain evidence directory contains unowned entry: {entry.name}"
            )
        if entry.is_symlink():
            raise ToolchainShardFailure(
                f"toolchain evidence directory contains a symlink: {entry.name}"
            )


def classify_reusable_evidence(
    evidence_dir: Path,
    inputs_by_shard: dict[str, dict[str, str]],
    *,
    now: int,
) -> tuple[dict[str, tuple[int, str]], dict[str, str]]:
    reused: dict[str, tuple[int, str]] = {}
    rejected: dict[str, str] = {}
    for shard in SHARD_ORDER:
        reusable, reason, age = read_reusable_evidence(
            evidence_dir / f"{shard}.json",
            shard,
            inputs_by_shard[shard],
            now=now,
        )
        if reusable:
            assert age is not None
            reused[shard] = (age, evidence_key(inputs_by_shard[shard]))
        else:
            rejected[shard] = reason
    return reused, rejected


def pending_shards(reused: Iterable[str]) -> tuple[str, ...]:
    reused_set = set(reused)
    return tuple(shard for shard in SHARD_ORDER if shard not in reused_set)


def validate_caxecraft_timing_prefix(payload: object) -> dict[str, object]:
    """Validate a path-free full-suite phase prefix, including failures."""

    if not isinstance(payload, dict):
        raise ToolchainShardFailure("Caxecraft timing report is not a JSON object")
    expected_fields = {
        "schemaVersion",
        "suite",
        "mode",
        "outcome",
        "durationMs",
        "summary",
        "phases",
    }
    duration = payload.get("durationMs")
    if (
        set(payload) != expected_fields
        or payload.get("schemaVersion") != 1
        or payload.get("suite") != "caxecraft-domain"
        or payload.get("mode") != "full"
        or payload.get("outcome") not in ("passed", "failed")
        or not isinstance(duration, int)
        or isinstance(duration, bool)
        or duration < 0
    ):
        raise ToolchainShardFailure(
            "Caxecraft timing report does not describe a full-suite phase prefix"
        )
    summary = payload.get("summary")
    phases = payload.get("phases")
    if not isinstance(phases, list) or len(phases) > len(CAXECRAFT_FULL_PHASES):
        raise ToolchainShardFailure(
            "Caxecraft timing report contains an invalid phase-prefix length"
        )
    phase_fields = {"name", "outcome", "durationMs", "haxeRequests"}
    observed_requests = 0
    saw_failed_phase = False
    for (expected_name, expected_phase_requests), phase in zip(
        CAXECRAFT_FULL_PHASES, phases
    ):
        if not isinstance(phase, dict) or set(phase) != phase_fields:
            raise ToolchainShardFailure(
                f"Caxecraft timing phase {expected_name} is malformed"
            )
        phase_duration = phase.get("durationMs")
        phase_outcome = phase.get("outcome")
        if (
            phase.get("name") != expected_name
            or phase_outcome not in ("passed", "failed")
            or phase.get("haxeRequests") != expected_phase_requests
            or not isinstance(phase_duration, int)
            or isinstance(phase_duration, bool)
            or phase_duration < 0
        ):
            raise ToolchainShardFailure(
                f"Caxecraft timing phase {expected_name} does not match its contract"
            )
        if saw_failed_phase:
            raise ToolchainShardFailure(
                "Caxecraft timing report contains work after a failed phase"
            )
        saw_failed_phase = phase_outcome == "failed"
        observed_requests += expected_phase_requests
    summary_requests = (
        summary.get("haxeRequests") if isinstance(summary, dict) else None
    )
    if (
        not isinstance(summary, dict)
        or set(summary) != {"haxeRequests"}
        or not isinstance(summary_requests, int)
        or isinstance(summary_requests, bool)
        or summary_requests != observed_requests
    ):
        raise ToolchainShardFailure("Caxecraft timing request summary is malformed")
    if payload.get("outcome") == "passed" and saw_failed_phase:
        raise ToolchainShardFailure(
            "Caxecraft timing report marks a failed phase as overall success"
        )
    return payload


def validate_caxecraft_timing_report(payload: object) -> dict[str, object]:
    """Require the complete successful full-suite phase/request contract."""

    validated = validate_caxecraft_timing_prefix(payload)
    phases = validated["phases"]
    if validated.get("outcome") != "passed" or len(phases) != len(
        CAXECRAFT_FULL_PHASES
    ):
        raise ToolchainShardFailure(
            "Caxecraft timing report does not describe a complete passing full run"
        )
    return validated


def run_shard(
    shard: str,
    scripts: dict[str, str],
    *,
    timing_report: Path | None = None,
    stream: TextIO | None = None,
) -> None:
    if shard not in SHARDS:
        raise ToolchainShardFailure(
            f"unknown shard {shard!r}; choose one of: {', '.join(SHARD_ORDER)}"
        )
    commands = SHARDS[shard]
    shard_start = time.monotonic_ns()
    records: list[dict[str, object]] = []
    failure: ToolchainShardFailure | None = None
    for index, script in enumerate(commands, start=1):
        print(
            f"toolchain-shard: {shard}: [{index}/{len(commands)}] npm run {script}",
            file=stream,
            flush=True,
        )
        command_start = time.monotonic_ns()
        phase_timing: dict[str, object] | None = None
        with tempfile.TemporaryDirectory(
            prefix="hxc-toolchain-command-detail-"
        ) as detail_directory:
            command_environment = os.environ.copy()
            caxecraft_timing_path: Path | None = None
            if script == CAXECRAFT_SCRIPT:
                caxecraft_timing_path = Path(detail_directory) / "caxecraft-timing.json"
                command_environment[CAXECRAFT_TIMING_ENV] = str(
                    caxecraft_timing_path
                )
            try:
                result = subprocess.run(
                    ["npm", "run", script],
                    cwd=ROOT,
                    check=False,
                    stdout=stream,
                    stderr=subprocess.STDOUT if stream is not None else None,
                    env=command_environment,
                )
                exit_code: int | None = result.returncode
            except OSError as error:
                exit_code = None
                failure = ToolchainShardFailure(
                    f"shard {shard} could not start {script}: {error}"
                )
            if caxecraft_timing_path is not None:
                if caxecraft_timing_path.is_file():
                    try:
                        raw_phase_timing = json.loads(
                            caxecraft_timing_path.read_text(encoding="utf-8")
                        )
                        phase_timing = (
                            validate_caxecraft_timing_report(raw_phase_timing)
                            if exit_code == 0
                            else validate_caxecraft_timing_prefix(raw_phase_timing)
                        )
                    except (OSError, UnicodeError, json.JSONDecodeError) as error:
                        failure = ToolchainShardFailure(
                            f"Caxecraft timing report cannot be read: {error}"
                        )
                    except ToolchainShardFailure as error:
                        failure = error
                elif exit_code == 0:
                    failure = ToolchainShardFailure(
                        "Caxecraft passed without writing its required timing report"
                    )
        duration_ms = elapsed_milliseconds(command_start)
        outcome = "passed" if exit_code == 0 and failure is None else "failed"
        record: dict[str, object] = {
            "script": script,
            "outcome": outcome,
            "exitCode": exit_code,
            "durationMs": duration_ms,
        }
        if phase_timing is not None:
            record["phaseTiming"] = phase_timing
        records.append(record)
        print(
            f"toolchain-shard: {shard}: {script}: {outcome} in {duration_ms}ms",
            file=stream,
            flush=True,
        )
        if failure is not None:
            break
        if exit_code != 0:
            failure = ToolchainShardFailure(
                f"shard {shard} stopped at {script} with exit {result.returncode}"
            )
            break

    payload: dict[str, object] = {
        "schemaVersion": 1,
        "shard": shard,
        "outcome": "failed" if failure is not None else "passed",
        "durationMs": elapsed_milliseconds(shard_start),
        "commands": records,
    }
    if timing_report is not None:
        write_timing_report(timing_report, payload)
    if failure is not None:
        raise failure
    print(
        f"toolchain-shard: OK: {shard}: {len(commands)} command(s) in "
        f"{payload['durationMs']}ms",
        file=stream,
        flush=True,
    )


def probe_one_minute_load() -> float | None:
    try:
        load, _, _ = os.getloadavg()
    except (AttributeError, OSError):
        return None
    if not math.isfinite(load) or load < 0:
        return None
    return load


def effective_cpu_count() -> int:
    process_cpu_count = getattr(os, "process_cpu_count", None)
    if callable(process_cpu_count):
        count = process_cpu_count()
        if isinstance(count, int) and not isinstance(count, bool) and count > 0:
            return count
    affinity = getattr(os, "sched_getaffinity", None)
    if callable(affinity):
        try:
            count = len(affinity(0))
        except OSError:
            count = 0
        if count > 0:
            return count
    return max(1, os.cpu_count() or 1)


def resolve_job_selection(
    value: int | None,
    *,
    environment: Mapping[str, str] | None = None,
    logical_cpus: int | None = None,
    one_minute_load: float | None = None,
    load_was_probed: bool = False,
) -> JobSelection:
    selected_environment = os.environ if environment is None else environment
    source = "automatic"
    if value is None and "HXC_TEST_JOBS" in selected_environment:
        raw = selected_environment["HXC_TEST_JOBS"]
        try:
            value = int(raw)
        except ValueError as error:
            raise ToolchainShardFailure(
                f"HXC_TEST_JOBS must be an integer, got {raw!r}"
            ) from error
        source = "HXC_TEST_JOBS"
    elif value is not None:
        source = "--jobs"

    cpu_count = logical_cpus if logical_cpus is not None else effective_cpu_count()
    cpu_count = max(1, cpu_count)
    if value is not None:
        if value < 1 or value > MAX_LOCAL_JOBS:
            raise ToolchainShardFailure(
                f"parallel jobs must be between 1 and {MAX_LOCAL_JOBS}, got {value}"
            )
        return JobSelection(
            jobs=value,
            source=source,
            logical_cpus=cpu_count,
            one_minute_load=None,
            reason="explicit override",
        )

    if not load_was_probed:
        one_minute_load = probe_one_minute_load()
    if one_minute_load is None:
        jobs = 2 if cpu_count >= 4 else 1
        return JobSelection(
            jobs=jobs,
            source="automatic-fallback",
            logical_cpus=cpu_count,
            one_minute_load=None,
            reason="system load is unavailable; using the portable conservative fallback",
        )
    if not math.isfinite(one_minute_load) or one_minute_load < 0:
        jobs = 2 if cpu_count >= 4 else 1
        return JobSelection(
            jobs=jobs,
            source="automatic-fallback",
            logical_cpus=cpu_count,
            one_minute_load=None,
            reason="system load is invalid; using the portable conservative fallback",
        )
    load_per_cpu = one_minute_load / cpu_count
    free_capacity = cpu_count - one_minute_load
    if cpu_count >= 8 and load_per_cpu <= 0.5 and free_capacity >= 4:
        jobs = 4
        reason = "idle multicore host has capacity for all four isolated shards"
    elif cpu_count >= 4 and load_per_cpu <= 0.85 and free_capacity >= 2:
        jobs = 2
        reason = "moderate host capacity supports two isolated shards"
    else:
        jobs = 1
        reason = "host load leaves too little safe headroom for parallel compiler work"
    return JobSelection(
        jobs=jobs,
        source="automatic",
        logical_cpus=cpu_count,
        one_minute_load=one_minute_load,
        reason=reason,
    )


def requested_jobs(value: int | None) -> int:
    return resolve_job_selection(value).jobs


def validate_successful_shard_report(
    payload: object,
    shard: str,
) -> dict[str, object]:
    if not isinstance(payload, dict):
        raise ToolchainShardFailure(f"{shard} timing report is not a JSON object")
    expected_fields = {"schemaVersion", "shard", "outcome", "durationMs", "commands"}
    if set(payload) != expected_fields:
        raise ToolchainShardFailure(f"{shard} timing report fields are malformed")
    duration = payload.get("durationMs")
    if (
        payload.get("schemaVersion") != 1
        or payload.get("shard") != shard
        or payload.get("outcome") != "passed"
        or not isinstance(duration, int)
        or isinstance(duration, bool)
        or duration < 0
    ):
        raise ToolchainShardFailure(
            f"{shard} timing report does not describe a complete passing shard"
        )
    commands = payload.get("commands")
    if not isinstance(commands, list) or len(commands) != len(SHARDS[shard]):
        raise ToolchainShardFailure(
            f"{shard} timing report does not contain every expected command"
        )
    command_fields = {"script", "outcome", "exitCode", "durationMs"}
    for expected_script, command in zip(SHARDS[shard], commands):
        expected_command_fields = (
            command_fields | {"phaseTiming"}
            if expected_script == CAXECRAFT_SCRIPT
            else command_fields
        )
        if not isinstance(command, dict) or set(command) != expected_command_fields:
            raise ToolchainShardFailure(
                f"{shard} timing report contains a malformed command record"
            )
        command_duration = command.get("durationMs")
        if (
            command.get("script") != expected_script
            or command.get("outcome") != "passed"
            or command.get("exitCode") != 0
            or not isinstance(command_duration, int)
            or isinstance(command_duration, bool)
            or command_duration < 0
        ):
            raise ToolchainShardFailure(
                f"{shard} timing report command {expected_script} did not pass exactly"
            )
        if expected_script == CAXECRAFT_SCRIPT:
            validate_caxecraft_timing_report(command.get("phaseTiming"))
    return payload


def run_all_shards(
    scripts: dict[str, str],
    *,
    jobs: int,
    selection: JobSelection | None = None,
    timing_dir: Path | None = None,
    resume: bool = False,
    evidence_dir: Path | None = None,
) -> None:
    parallel_start = time.monotonic_ns()
    if selection is None:
        selection = JobSelection(
            jobs=jobs,
            source="caller",
            logical_cpus=effective_cpu_count(),
            one_minute_load=None,
            reason="worker count supplied by caller",
        )
    print(
        "toolchain-parallel: starting "
        f"{len(SHARD_ORDER)} isolated shards with {jobs} worker(s); "
        f"{selection.reason}",
        flush=True,
    )
    failures: dict[str, str] = {}
    initial_evidence_inputs: dict[str, dict[str, str]] = {}
    reused: dict[str, tuple[int, str]] = {}
    selected_evidence_dir = evidence_dir or DEFAULT_EVIDENCE_DIR
    if resume:
        prepare_evidence_directory(selected_evidence_dir)
        initial_evidence_inputs = collect_evidence_inputs(scripts)
        reused, rejected = classify_reusable_evidence(
            selected_evidence_dir,
            initial_evidence_inputs,
            now=int(time.time()),
        )
        for shard in SHARD_ORDER:
            if shard in reused:
                age = reused[shard][0]
                print(
                    f"toolchain-parallel: {shard}: reused exact passing evidence "
                    f"({age}s old)",
                    flush=True,
                )
            else:
                print(
                    f"toolchain-parallel: {shard}: will execute ({rejected[shard]})",
                    flush=True,
                )
    active_shards = pending_shards(reused)

    with tempfile.TemporaryDirectory(prefix="hxc-toolchain-parallel-") as temporary:
        temporary_root = Path(temporary)
        log_paths = {
            shard: temporary_root / f"{index:02d}-{shard}.log"
            for index, shard in enumerate(SHARD_ORDER)
        }
        if timing_dir is not None:
            report_paths = {
                shard: timing_dir / f"toolchain-{shard}-timing.json"
                for shard in SHARD_ORDER
            }
        else:
            report_paths = {
                shard: temporary_root / f"toolchain-{shard}-timing.json"
                for shard in SHARD_ORDER
            }

        def worker(shard: str) -> str | None:
            try:
                with log_paths[shard].open("w", encoding="utf-8", newline="") as log:
                    run_shard(
                        shard,
                        scripts,
                        timing_report=report_paths[shard],
                        stream=log,
                    )
                return None
            except (ToolchainShardFailure, OSError) as error:
                return str(error)

        if active_shards:
            with concurrent.futures.ThreadPoolExecutor(
                max_workers=min(jobs, len(active_shards))
            ) as executor:
                futures = {
                    executor.submit(worker, shard): shard for shard in active_shards
                }
                pending = set(futures)
                while pending:
                    done, pending = concurrent.futures.wait(
                        pending,
                        timeout=30,
                        return_when=concurrent.futures.FIRST_COMPLETED,
                    )
                    if not done:
                        waiting = ", ".join(
                            shard
                            for shard in SHARD_ORDER
                            if any(futures[future] == shard for future in pending)
                        )
                        print(f"toolchain-parallel: waiting: {waiting}", flush=True)
                        continue
                    for future in sorted(
                        done, key=lambda item: SHARD_ORDER.index(futures[item])
                    ):
                        shard = futures[future]
                        error = future.result()
                        if error is not None:
                            failures[shard] = error
                            outcome = "failed"
                        else:
                            outcome = "passed"
                        print(
                            f"toolchain-parallel: {shard}: {outcome}", flush=True
                        )

        reports: list[dict[str, object]] = []
        report_digests: dict[str, str] = {}
        for shard in SHARD_ORDER:
            print(f"\n===== toolchain shard: {shard} =====", flush=True)
            if shard in reused:
                print("toolchain-shard: reused exact prior passing evidence", flush=True)
                continue
            try:
                print(log_paths[shard].read_text(encoding="utf-8"), end="", flush=True)
            except OSError as error:
                failures[shard] = f"cannot read isolated log: {error}"
            try:
                report = json.loads(report_paths[shard].read_text(encoding="utf-8"))
            except (OSError, UnicodeError, json.JSONDecodeError) as error:
                failures[shard] = f"cannot read timing report: {error}"
            else:
                if isinstance(report, dict):
                    reports.append(report)
                    if shard not in failures:
                        try:
                            validated = validate_successful_shard_report(report, shard)
                        except ToolchainShardFailure as error:
                            failures[shard] = str(error)
                        else:
                            report_digests[shard] = sha256_bytes(
                                canonical_json_bytes(validated)
                            )
                else:
                    failures[shard] = "timing report is not a JSON object"

        if resume:
            try:
                final_evidence_inputs = collect_evidence_inputs(scripts)
            except ToolchainShardFailure as error:
                failures["evidence"] = str(error)
                final_evidence_inputs = {}
            if final_evidence_inputs != initial_evidence_inputs:
                failures["evidence"] = (
                    "evidence inputs changed while the shards were running; "
                    "no result was cached"
                )
            else:
                evidence_now = int(time.time())
                for shard in active_shards:
                    if shard in failures:
                        continue
                    write_success_evidence(
                        selected_evidence_dir / f"{shard}.json",
                        shard,
                        initial_evidence_inputs[shard],
                        now=evidence_now,
                        report_digest=report_digests[shard],
                    )

        if timing_dir is not None:
            write_timing_report(
                timing_dir / "toolchain-parallel-summary.json",
                {
                    "schemaVersion": 2,
                    "outcome": "failed" if failures else "passed",
                    "durationMs": elapsed_milliseconds(parallel_start),
                    "jobs": jobs,
                    "workerSelection": {
                        "source": selection.source,
                        "logicalCpuCount": selection.logical_cpus,
                        "oneMinuteLoad": selection.one_minute_load,
                        "reason": selection.reason,
                    },
                    "executedShards": reports,
                    "reusedShards": [
                        {
                            "shard": shard,
                            "evidenceAgeSeconds": reused[shard][0],
                            "evidenceKey": reused[shard][1],
                        }
                        for shard in SHARD_ORDER
                        if shard in reused
                    ],
                },
            )

    if failures:
        failure_order = (*SHARD_ORDER, "evidence")
        details = "; ".join(
            f"{shard}: {failures[shard]}" for shard in failure_order if shard in failures
        )
        raise ToolchainShardFailure("parallel toolchain failure: " + details)
    print(
        f"toolchain-parallel: OK: {len(SHARD_ORDER)} shard(s), {jobs} worker(s)",
        flush=True,
    )


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true", help="validate exact partition coverage")
    mode.add_argument("--list", action="store_true", help="list shards and commands")
    mode.add_argument("--run", metavar="SHARD", help="run one validated shard")
    mode.add_argument(
        "--run-all",
        action="store_true",
        help="run every isolation-approved shard with bounded local concurrency",
    )
    parser.add_argument(
        "--timing-report",
        type=Path,
        help="write an ordered machine-readable timing report for --run",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        help="worker count for --run-all (default: resource-aware, 1-4)",
    )
    parser.add_argument(
        "--timing-dir",
        type=Path,
        help="preserve per-shard and aggregate timing JSON for --run-all",
    )
    parser.add_argument(
        "--resume",
        action="store_true",
        help="reuse exact, unexpired local passing shard evidence for --run-all",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        scripts = load_scripts()
        canonical = validate_partition(scripts)
        if args.timing_report is not None and args.run is None:
            raise ToolchainShardFailure("--timing-report requires --run SHARD")
        if (
            args.jobs is not None
            or args.timing_dir is not None
            or args.resume
        ) and not args.run_all:
            raise ToolchainShardFailure(
                "--jobs, --timing-dir, and --resume require --run-all"
            )
        if args.list:
            for shard in SHARD_ORDER:
                print(f"{shard}: {' '.join(SHARDS[shard])}")
            return 0
        if args.check:
            print(
                "toolchain-shards: OK: "
                f"{len(canonical)} command(s) in {len(SHARD_ORDER)} exact shard(s)"
            )
            return 0
        if args.run_all:
            selection = resolve_job_selection(args.jobs)
            run_all_shards(
                scripts,
                jobs=selection.jobs,
                selection=selection,
                timing_dir=args.timing_dir,
                resume=args.resume,
            )
            return 0
        run_shard(args.run, scripts, timing_report=args.timing_report)
        return 0
    except (ToolchainShardFailure, OSError) as error:
        print(f"toolchain-shards: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
