#!/usr/bin/env python3
"""Run one local test owner and publish exact reusable commit evidence.

The normal developer command executes the selected package script and records
the complete input identity only after a passing result. The pre-commit mode
may skip that same owner when the record still matches byte-for-byte. A miss,
malformed record, changed input, CI run, or explicit cold request executes the
owner normally.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import stat
import subprocess
import sys
import time
from collections.abc import Mapping
from contextlib import contextmanager
from pathlib import Path
from typing import IO, Iterator

from run_toolchain_shard import (
    EVIDENCE_LOCK_PATHS,
    ROOT,
    canonical_json_bytes,
    digest_evidence_tree,
    digest_files,
    environment_digest,
    host_identity_digest,
    relevant_worktree_digest,
    reject_symlink_ancestry,
    sha256_bytes,
    staged_tree_identity,
    tool_identity_digest,
    write_timing_report,
)


LOCAL_EVIDENCE_SCHEMA_VERSION = 1
LOCAL_EVIDENCE_MARKER_SCHEMA_VERSION = 1
LOCAL_EVIDENCE_TTL_SECONDS = 24 * 60 * 60
DEFAULT_EVIDENCE_DIR = ROOT / ".cache" / "local-gates"
EVIDENCE_MARKER = ".hxc-local-gate-evidence.json"
EVIDENCE_MARKER_PAYLOAD = {
    "schemaVersion": LOCAL_EVIDENCE_MARKER_SCHEMA_VERSION,
    "owner": "haxe.c-local-test-gates",
}
SCRIPT_PATTERN = re.compile(r"^(?:test:[a-z0-9][a-z0-9:-]*|snapshots:catalog)$")
PERFORMANCE_WORDS = ("benchmark", "performance", "profile", "timing")
REUSABLE_OWNER_SCRIPTS = frozenset(
    ("test:all-sources", "test:hxc-ir", "test:project-emitter")
)
OWNER_INPUT_ROOTS = {
    "test:all-sources": (
        "haxe_libraries",
        "src",
        "std",
        "test/all_sources",
        "vendor/reflaxe/src",
    ),
    "test:hxc-ir": (
        "haxe_libraries",
        "src",
        "std",
        "test/hxc_ir",
        "vendor/reflaxe/src",
    ),
    "test:project-emitter": (
        "haxe_libraries",
        "src",
        "std",
        "test/all_sources",
        "test/project_emitter",
        "vendor/reflaxe/src",
    ),
}
GIT_HOOK_AUTHOR_VARIABLES = (
    "GIT_AUTHOR_NAME",
    "GIT_AUTHOR_EMAIL",
    "GIT_AUTHOR_DATE",
)


class LocalGateFailure(Exception):
    """Raised when local evidence cannot be handled without guessing."""


def load_scripts() -> dict[str, str]:
    """Load the exact package-script registry used by direct and hook runs."""
    try:
        package = json.loads((ROOT / "package.json").read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise LocalGateFailure(f"cannot read package.json: {error}") from error
    if not isinstance(package, dict) or not isinstance(package.get("scripts"), dict):
        raise LocalGateFailure("package.json scripts must be an object")
    scripts = package["scripts"]
    if not all(
        isinstance(name, str) and isinstance(command, str)
        for name, command in scripts.items()
    ):
        raise LocalGateFailure("package.json scripts must map strings to strings")
    return scripts


def validate_owner_script(script: str, scripts: Mapping[str, str]) -> None:
    """Reject recursive, unknown, and performance-sample owners."""
    if SCRIPT_PATTERN.fullmatch(script) is None:
        raise LocalGateFailure(
            "local evidence owner must be a test:* script or snapshots:catalog"
        )
    if script == "test:local-gate":
        raise LocalGateFailure("local evidence runner cannot invoke itself")
    if script not in scripts:
        raise LocalGateFailure(f"package.json has no script named {script}")


def script_is_reusable(script: str) -> bool:
    """Admit only reviewed expensive semantic owners, never timing samples."""
    lowered = script.lower()
    return script in REUSABLE_OWNER_SCRIPTS and not any(
        word in lowered for word in PERFORMANCE_WORDS
    )


def command_definition_digest(script: str, command: str) -> str:
    """Bind evidence to one owner name and its exact package command."""
    return sha256_bytes(
        canonical_json_bytes({"script": script, "command": command})
    )


def runner_implementation_digest() -> str:
    """Bind records to both the local runner and its shared keying primitives."""
    digest = hashlib.sha256()
    for path in (
        Path(__file__),
        ROOT / "scripts/ci/run_toolchain_shard.py",
    ):
        try:
            payload = path.read_bytes()
        except OSError as error:
            raise LocalGateFailure(f"cannot hash evidence runner {path}: {error}") from error
        digest.update(len(payload).to_bytes(8, "big"))
        digest.update(payload)
    return digest.hexdigest()


def owner_inputs_digest(script: str) -> str:
    """Hash every file visible through one reusable Haxe owner's classpaths.

    Git's ordinary untracked-file query deliberately ignores ignored files.
    These owners discover or resolve Haxe modules from whole directory trees,
    so their reusable key independently hashes every file in those trees.
    This keeps an ignored shadow module from becoming an invisible cache input.
    """

    roots = OWNER_INPUT_ROOTS.get(script)
    if roots is None:
        raise LocalGateFailure(
            f"reusable owner {script} has no complete input-root declaration"
        )
    digest = hashlib.sha256()
    for relative in roots:
        encoded = relative.encode("utf-8")
        digest.update(len(encoded).to_bytes(8, "big"))
        digest.update(encoded)
        try:
            tree_digest = digest_evidence_tree(ROOT / relative)
        except Exception as error:
            raise LocalGateFailure(
                f"cannot hash {script} input root {relative}: {error}"
            ) from error
        digest.update(bytes.fromhex(tree_digest))
    return digest.hexdigest()


def default_git_index_path(environment: Mapping[str, str]) -> Path | None:
    """Resolve the normal worktree index without accepting a hook override."""
    selected_environment = dict(environment)
    selected_environment.pop("GIT_INDEX_FILE", None)
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--git-path", "index"],
            cwd=ROOT,
            env=selected_environment,
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
    except OSError:
        return None
    value = result.stdout.strip()
    if result.returncode != 0 or not value:
        return None
    path = Path(value)
    return (ROOT / path).resolve() if not path.is_absolute() else path.resolve()


def normalized_owner_environment(
    environment: Mapping[str, str],
) -> dict[str, str]:
    """Remove only proven wrapper noise before keying and running an owner.

    Git supplies the normal index plus commit-author/editor variables, while
    stock Beads marks its own hook delegation with ``BD_GIT_HOOK=1``. None of
    the admitted local owners consumes those transport values, so both direct
    and hook runs remove them and execute under the same environment. A
    partial commit's temporary index and every other environment difference
    remain in the key.
    """

    selected = dict(environment)
    selected.pop("BD_GIT_HOOK", None)
    for name in GIT_HOOK_AUTHOR_VARIABLES:
        selected.pop(name, None)
    if selected.get("GIT_EDITOR") == ":":
        selected.pop("GIT_EDITOR")

    index_value = selected.get("GIT_INDEX_FILE")
    default_index = (
        default_git_index_path(selected) if index_value is not None else None
    )
    if index_value is not None and default_index is not None:
        candidate = Path(index_value)
        candidate = (
            (ROOT / candidate).resolve()
            if not candidate.is_absolute()
            else candidate.resolve()
        )
        if candidate == default_index:
            selected.pop("GIT_INDEX_FILE")
    # Project-emitter loads the all-source runner through importlib. Without
    # this fixed owner setting, Python may create an ignored __pycache__ file
    # inside a hashed input tree during the test, making the first clean pass
    # look like input drift and preventing receipt publication.
    selected["PYTHONDONTWRITEBYTECODE"] = "1"
    return selected


def collect_inputs(
    script: str,
    command: str,
    environment: Mapping[str, str],
) -> dict[str, str]:
    """Collect every input family that can change a local owner's result."""
    try:
        hook_digest = sha256_bytes((ROOT / "scripts/hooks/pre-commit").read_bytes())
        return {
            "stagedTree": staged_tree_identity(),
            "worktreeDigest": relevant_worktree_digest(),
            "ownerInputsDigest": owner_inputs_digest(script),
            "commandDigest": command_definition_digest(script, command),
            "locksDigest": digest_files(EVIDENCE_LOCK_PATHS),
            "toolsDigest": tool_identity_digest(environment),
            "environmentDigest": environment_digest(environment),
            "runnerDigest": runner_implementation_digest(),
            "hookDigest": hook_digest,
            "hostDigest": host_identity_digest(),
        }
    except Exception as error:
        if isinstance(error, LocalGateFailure):
            raise
        raise LocalGateFailure(f"cannot collect local evidence inputs: {error}") from error


def evidence_key(inputs: Mapping[str, str]) -> str:
    """Return the content address for one complete input map."""
    return sha256_bytes(canonical_json_bytes(dict(inputs)))


def owner_record_path(evidence_dir: Path, script: str) -> Path:
    """Give one package owner a collision-resistant, reviewable record path."""
    slug = script.replace(":", "-")
    suffix = sha256_bytes(script.encode("utf-8"))[:16]
    return evidence_dir / f"{slug}-{suffix}.json"


def prepare_evidence_directory(path: Path) -> None:
    """Create or validate the narrowly owned local-evidence directory."""
    try:
        reject_symlink_ancestry(path)
    except Exception as error:
        raise LocalGateFailure(str(error)) from error
    if path.exists() and not path.is_dir():
        raise LocalGateFailure("local evidence path is not a directory")
    path.mkdir(parents=True, exist_ok=True)
    entries = sorted(path.iterdir(), key=lambda entry: entry.name.encode("utf-8"))
    marker = path / EVIDENCE_MARKER
    if not marker.exists():
        if entries:
            raise LocalGateFailure(
                "local evidence directory is nonempty but has no ownership marker"
            )
        write_timing_report(marker, EVIDENCE_MARKER_PAYLOAD)
        entries = [marker]
    if marker.is_symlink():
        raise LocalGateFailure("local evidence ownership marker is a symlink")
    try:
        marker_payload = json.loads(marker.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise LocalGateFailure(
            f"cannot validate local evidence ownership marker: {error}"
        ) from error
    if marker_payload != EVIDENCE_MARKER_PAYLOAD:
        raise LocalGateFailure("local evidence ownership marker does not match")
    for entry in entries:
        if entry == marker:
            continue
        if entry.is_symlink():
            raise LocalGateFailure(
                f"local evidence directory contains a symlink: {entry.name}"
            )
        if not entry.is_file() or re.fullmatch(
            r"(?:test|snapshots)-[a-z0-9-]+-[0-9a-f]{16}\.(?:json|lock)",
            entry.name,
        ) is None:
            raise LocalGateFailure(
                f"local evidence directory contains an unowned entry: {entry.name}"
            )


def evidence_record(
    script: str,
    inputs: Mapping[str, str],
    *,
    duration_ms: int,
    now: int,
) -> dict[str, object]:
    """Build the closed passing-result record stored after a stable run."""
    selected_inputs = dict(inputs)
    return {
        "schemaVersion": LOCAL_EVIDENCE_SCHEMA_VERSION,
        "owner": script,
        "outcome": "passed",
        "key": evidence_key(selected_inputs),
        "createdAtUnix": now,
        "expiresAtUnix": now + LOCAL_EVIDENCE_TTL_SECONDS,
        "durationMs": duration_ms,
        "inputs": selected_inputs,
    }


def validate_reusable_evidence(
    payload: object,
    script: str,
    inputs: Mapping[str, str],
    *,
    now: int,
) -> tuple[bool, str, int | None]:
    """Accept only one complete, unexpired passing record for exact inputs."""
    if not isinstance(payload, dict):
        return False, "record is not a JSON object", None
    expected_fields = {
        "schemaVersion",
        "owner",
        "outcome",
        "key",
        "createdAtUnix",
        "expiresAtUnix",
        "durationMs",
        "inputs",
    }
    if set(payload) != expected_fields:
        return False, "record fields do not match the closed schema", None
    if payload.get("schemaVersion") != LOCAL_EVIDENCE_SCHEMA_VERSION:
        return False, "record schema version does not match", None
    if payload.get("owner") != script or payload.get("outcome") != "passed":
        return False, "record is not a passing result for this owner", None
    created = payload.get("createdAtUnix")
    expires = payload.get("expiresAtUnix")
    duration = payload.get("durationMs")
    if (
        not isinstance(created, int)
        or isinstance(created, bool)
        or not isinstance(expires, int)
        or isinstance(expires, bool)
        or expires != created + LOCAL_EVIDENCE_TTL_SECONDS
        or not isinstance(duration, int)
        or isinstance(duration, bool)
        or duration < 0
    ):
        return False, "record lifetime or duration is malformed", None
    if created > now:
        return False, "record comes from the future", None
    if expires <= now:
        return False, "record has expired", None
    selected_inputs = dict(inputs)
    if payload.get("inputs") != selected_inputs:
        stored_inputs = payload.get("inputs")
        if isinstance(stored_inputs, dict):
            changed_names = sorted(
                name
                for name in set(stored_inputs) | set(selected_inputs)
                if stored_inputs.get(name) != selected_inputs.get(name)
            )
            if changed_names:
                return (
                    False,
                    "evidence inputs changed: " + ", ".join(changed_names),
                    None,
                )
        return False, "one or more evidence inputs changed", None
    expected_key = evidence_key(selected_inputs)
    if payload.get("key") != expected_key:
        return False, "record key does not match its current inputs", None
    return True, "exact passing evidence is reusable", now - created


def read_reusable_evidence(
    path: Path,
    script: str,
    inputs: Mapping[str, str],
    *,
    now: int,
) -> tuple[bool, str, int | None]:
    """Read a record without ever treating corruption as a cache hit."""
    if path.is_symlink():
        return False, "record path is a symlink", None
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        return False, "record is missing", None
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        return False, f"record cannot be read: {error}", None
    return validate_reusable_evidence(payload, script, inputs, now=now)


def invalidate_prior_evidence(path: Path) -> None:
    """Remove an older result before execution so failure cannot reveal it."""
    if path.is_symlink():
        raise LocalGateFailure("local evidence record path is a symlink")
    try:
        path.unlink()
    except FileNotFoundError:
        return
    except OSError as error:
        raise LocalGateFailure(
            f"cannot invalidate prior local evidence {path.name}: {error}"
        ) from error


def lock_path_for_record(record_path: Path) -> Path:
    """Keep the lock beside the record under the same owned directory."""
    return record_path.with_suffix(".lock")


def acquire_platform_lock(handle: IO[bytes]) -> None:
    """Acquire one process-scoped exclusive lock on POSIX or Windows."""
    if os.name == "nt":
        import msvcrt

        handle.seek(0)
        if handle.read(1) == b"":
            handle.write(b"\0")
            handle.flush()
        handle.seek(0)
        for _ in range(12):
            try:
                msvcrt.locking(handle.fileno(), msvcrt.LK_NBLCK, 1)
                return
            except OSError:
                time.sleep(5)
        raise LocalGateFailure("timed out waiting for the local evidence owner lock")
    else:
        import fcntl

        fcntl.flock(handle.fileno(), fcntl.LOCK_EX)


def release_platform_lock(handle: IO[bytes]) -> None:
    """Release the platform lock while keeping the owned lock file reusable."""
    if os.name == "nt":
        import msvcrt

        handle.seek(0)
        msvcrt.locking(handle.fileno(), msvcrt.LK_UNLCK, 1)
    else:
        import fcntl

        fcntl.flock(handle.fileno(), fcntl.LOCK_UN)


@contextmanager
def owner_evidence_lock(record_path: Path) -> Iterator[None]:
    """Serialize one owner's read/run/publish transaction across processes."""
    lock_path = lock_path_for_record(record_path)
    if lock_path.is_symlink():
        raise LocalGateFailure("local evidence lock path is a symlink")
    flags = os.O_CREAT | os.O_RDWR
    if hasattr(os, "O_NOFOLLOW"):
        flags |= os.O_NOFOLLOW
    descriptor: int | None = None
    try:
        descriptor = os.open(lock_path, flags, 0o600)
        opened = os.fstat(descriptor)
        linked = os.lstat(lock_path)
        if (
            not stat.S_ISREG(opened.st_mode)
            or not stat.S_ISREG(linked.st_mode)
            or (opened.st_dev, opened.st_ino) != (linked.st_dev, linked.st_ino)
        ):
            raise LocalGateFailure(
                "local evidence lock path changed while it was opened"
            )
        with os.fdopen(descriptor, "a+b") as handle:
            descriptor = None
            acquire_platform_lock(handle)
            try:
                yield
            finally:
                release_platform_lock(handle)
    except LocalGateFailure:
        raise
    except OSError as error:
        raise LocalGateFailure(
            f"cannot lock local evidence owner {record_path.name}: {error}"
        ) from error
    finally:
        if descriptor is not None:
            os.close(descriptor)


def environment_requests_ci(environment: Mapping[str, str]) -> bool:
    """Treat every nonempty, non-false CI value as an authoritative cold run."""
    value = environment.get("CI", "")
    return value.strip().lower() not in ("", "0", "false", "no", "off")


def execute_package_owner(
    script: str,
    environment: Mapping[str, str],
) -> tuple[int, int]:
    """Execute one npm owner and return its status and wall duration."""
    start = time.monotonic_ns()
    try:
        result = subprocess.run(
            ["npm", "run", script],
            cwd=ROOT,
            env=dict(environment),
            check=False,
        )
    except OSError as error:
        raise LocalGateFailure(f"cannot execute npm run {script}: {error}") from error
    duration_ms = max(0, (time.monotonic_ns() - start + 500_000) // 1_000_000)
    return result.returncode, duration_ms


def run_owner(
    script: str,
    *,
    hook_mode: bool,
    cold: bool,
    evidence_dir: Path,
    environment: Mapping[str, str],
    now: int | None = None,
) -> int:
    """Run or exactly reuse one package owner and return its process status."""
    scripts = load_scripts()
    validate_owner_script(script, scripts)
    command = scripts[script]
    owner_environment = normalized_owner_environment(environment)
    reusable_kind = script_is_reusable(script)
    ci_cold = environment_requests_ci(owner_environment)
    if not reusable_kind:
        reason = "this owner is not in the reviewed expensive-reuse set"
        print(f"local-gate: {script}: executing cold because {reason}", flush=True)
        returncode, _ = execute_package_owner(script, owner_environment)
        if returncode != 0:
            print(
                f"local-gate: {script}: failed with exit {returncode}; "
                "no evidence was published",
                file=sys.stderr,
                flush=True,
            )
        return returncode

    if cold or ci_cold:
        reason = "CI is set" if ci_cold else "an explicit --cold request was made"
        print(
            f"local-gate: {script}: executing cold because {reason}",
            flush=True,
        )
        cold_executed = False
        try:
            prepare_evidence_directory(evidence_dir)
            record_path = owner_record_path(evidence_dir, script)
            with owner_evidence_lock(record_path):
                invalidate_prior_evidence(record_path)
                returncode, _ = execute_package_owner(
                    script, owner_environment
                )
                cold_executed = True
                # A cold result is authoritative execution evidence, not a
                # reusable local receipt. Remove a record even if another
                # process somehow recreated it during owner shutdown.
                invalidate_prior_evidence(record_path)
        except LocalGateFailure as error:
            if cold_executed:
                print(
                    "local-gate: ERROR: cold owner ran but local evidence "
                    f"cleanup failed: {error}",
                    file=sys.stderr,
                    flush=True,
                )
                return 2
            print(
                "local-gate: WARNING: cold execution ignored unusable "
                f"local evidence state: {error}",
                file=sys.stderr,
                flush=True,
            )
            returncode, _ = execute_package_owner(script, owner_environment)
        if returncode != 0:
            print(
                f"local-gate: {script}: failed with exit {returncode}; "
                "no evidence was published",
                file=sys.stderr,
                flush=True,
            )
        return returncode

    prepare_evidence_directory(evidence_dir)
    record_path = owner_record_path(evidence_dir, script)
    with owner_evidence_lock(record_path):
        current_time = int(time.time()) if now is None else now
        inputs_before = collect_inputs(script, command, owner_environment)
        if hook_mode:
            reusable, reason, age = read_reusable_evidence(
                record_path,
                script,
                inputs_before,
                now=current_time,
            )
            if reusable:
                assert age is not None
                print(
                    "local-gate: reused exact passing evidence "
                    f"for {script} "
                    f"({age}s old, key {evidence_key(inputs_before)[:16]})",
                    flush=True,
                )
                return 0
            print(f"local-gate: {script}: executing ({reason})", flush=True)
        else:
            print(
                f"local-gate: {script}: executing and publishing direct evidence",
                flush=True,
            )

        invalidate_prior_evidence(record_path)
        returncode, duration_ms = execute_package_owner(script, owner_environment)
        if returncode != 0:
            print(
                f"local-gate: {script}: failed with exit {returncode}; "
                "no evidence was published",
                file=sys.stderr,
                flush=True,
            )
            return returncode
        inputs_after = collect_inputs(script, command, owner_environment)
        if inputs_after != inputs_before:
            print(
                f"local-gate: {script}: passed, but inputs changed during "
                "execution; no reusable evidence was published",
                flush=True,
            )
            return 0
        write_timing_report(
            record_path,
            evidence_record(
                script,
                inputs_before,
                duration_ms=duration_ms,
                now=current_time,
            ),
        )
        print(
            f"local-gate: {script}: published exact passing evidence "
            f"(key {evidence_key(inputs_before)[:16]})",
            flush=True,
        )
        return 0


def parser() -> argparse.ArgumentParser:
    """Build the small explicit command-line contract."""
    selected = argparse.ArgumentParser(
        description="Run one local test owner with exact pre-commit evidence reuse."
    )
    selected.add_argument("--hook", action="store_true", help="reuse before executing")
    selected.add_argument("--cold", action="store_true", help="always execute")
    selected.add_argument(
        "--evidence-dir",
        type=Path,
        default=DEFAULT_EVIDENCE_DIR,
        help=argparse.SUPPRESS,
    )
    selected.add_argument("script", help="package.json test owner")
    return selected


def main(arguments: list[str] | None = None) -> int:
    """Parse the command and keep unsafe evidence failures fail closed."""
    selected = parser().parse_args(arguments)
    try:
        return run_owner(
            selected.script,
            hook_mode=selected.hook,
            cold=selected.cold,
            evidence_dir=selected.evidence_dir,
            environment=os.environ,
        )
    except LocalGateFailure as error:
        print(f"local-gate: ERROR: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
