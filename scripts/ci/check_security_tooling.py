#!/usr/bin/env python3
"""Fail closed when repository leak-prevention and CI supply-chain pins drift."""

from __future__ import annotations

import fnmatch
import json
import os
import re
import stat
import subprocess
import sys
import tempfile
import tomllib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
WORKFLOW_ROOT = ROOT / ".github/workflows"
WORKFLOW = WORKFLOW_ROOT / "governance.yml"
INSTALLER = ROOT / "scripts/ci/install-gitleaks.sh"
GITLEAKS_CONFIG = ROOT / ".gitleaks.toml"
PACKAGE = ROOT / "package.json"
EXPECTED_ACTIONS = {
    "actions/checkout": "9c091bb21b7c1c1d1991bb908d89e4e9dddfe3e0",
    "actions/setup-node": "249970729cb0ef3589644e2896645e5dc5ba9c38",
}
EXPECTED_GITLEAKS_REGEX = (
    r'(hxc-stdlib-api-v1:|(stdlibPathInventory|applicableSource|apiSurface|source)Sha256"\s*:\s*")'
    r"[0-9a-f]{64}"
)
PUBLIC_PREFLIGHT_COMMAND = (
    "npm run format:haxe:check && npm run security:gitleaks && "
    "npm run security:beads-history && npm run test:security-tooling && "
    "npm run test:governance"
)
REQUIRED_IGNORES = {
    ".env",
    ".env.*",
    ".npmrc",
    ".pypirc",
    ".netrc",
    ".aws/",
    ".gnupg/",
    ".ssh/",
    "credentials.json",
    "service-account*.json",
    "id_rsa",
    "id_ed25519",
    "*.pem",
    "*.key",
    "*.p12",
    "*.pfx",
    "*.jks",
    "*.keystore",
}


class SecurityToolingFailure(RuntimeError):
    pass


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise SecurityToolingFailure(
            f"cannot read {path.relative_to(ROOT)}: {error}"
        ) from error


def require_executable(relative: str) -> None:
    path = ROOT / relative
    try:
        mode = path.stat().st_mode
    except OSError as error:
        raise SecurityToolingFailure(f"cannot stat {relative}: {error}") from error
    if not stat.S_ISREG(mode) or mode & stat.S_IXUSR == 0:
        raise SecurityToolingFailure(f"{relative} must be an executable regular file")


def extract(source: str, pattern: str, label: str) -> str:
    match = re.search(pattern, source)
    if match is None:
        raise SecurityToolingFailure(f"missing {label}")
    return match.group(1)


def write_executable(path: Path, source: str) -> None:
    path.write_text(source, encoding="utf-8")
    path.chmod(0o755)


def validate_hook_failure_propagation() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-hook-chain-") as directory:
        checkout = Path(directory)
        fake_bin = checkout / "bin"
        repository_hooks = checkout / "scripts/hooks"
        fake_bin.mkdir()
        repository_hooks.mkdir(parents=True)

        write_executable(
            fake_bin / "git",
            """#!/usr/bin/env sh
if [ "$1" = "rev-parse" ] && [ "$2" = "--show-toplevel" ]; then
  printf '%s\n' "$HXC_HOOK_TEST_ROOT"
  exit 0
fi
exit 91
""",
        )
        write_executable(
            fake_bin / "bd",
            """#!/usr/bin/env sh
printf '%s\n' "$*" >> "$HXC_BEADS_HOOK_MARKER"
exit 0
""",
        )

        environment = os.environ.copy()
        environment.pop("BD_GIT_HOOK", None)
        environment.update(
            {
                "PATH": f"{fake_bin}:/usr/bin:/bin",
                "HXC_HOOK_TEST_ROOT": str(checkout),
            }
        )

        for hook_name, expected_status in (("pre-commit", 73), ("pre-push", 74)):
            repository_marker = checkout / f"{hook_name}.repository-ran"
            beads_marker = checkout / f"{hook_name}.beads-ran"
            write_executable(
                repository_hooks / hook_name,
                """#!/usr/bin/env sh
printf '%s\n' "$*" > "$HXC_REPOSITORY_HOOK_MARKER"
exit "$HXC_REPOSITORY_HOOK_STATUS"
""",
            )
            environment.update(
                {
                    "HXC_REPOSITORY_HOOK_MARKER": str(repository_marker),
                    "HXC_REPOSITORY_HOOK_STATUS": str(expected_status),
                    "HXC_BEADS_HOOK_MARKER": str(beads_marker),
                }
            )

            result = subprocess.run(
                ["sh", str(ROOT / f".beads/hooks/{hook_name}"), "probe", hook_name],
                cwd=checkout,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=10,
            )
            if result.returncode != expected_status:
                raise SecurityToolingFailure(
                    f"{hook_name} wrapper returned {result.returncode}, not delegated "
                    f"status {expected_status}"
                )
            if not repository_marker.is_file():
                raise SecurityToolingFailure(
                    f"{hook_name} wrapper did not invoke its repository hook"
                )
            if repository_marker.read_text(encoding="utf-8") != f"probe {hook_name}\n":
                raise SecurityToolingFailure(
                    f"{hook_name} wrapper did not preserve repository-hook arguments"
                )
            if beads_marker.exists():
                raise SecurityToolingFailure(
                    f"{hook_name} wrapper ran Beads after its repository hook failed"
                )


def validate_installer() -> tuple[str, str]:
    source = read_text(INSTALLER)
    version = extract(
        source, r'readonly GITLEAKS_VERSION="([^"]+)"', "Gitleaks version pin"
    )
    digest = extract(
        source, r'readonly GITLEAKS_SHA256="([0-9a-f]+)"', "Gitleaks digest pin"
    )
    if re.fullmatch(r"[0-9]+\.[0-9]+\.[0-9]+", version) is None:
        raise SecurityToolingFailure(f"invalid Gitleaks version: {version!r}")
    if re.fullmatch(r"[0-9a-f]{64}", digest) is None:
        raise SecurityToolingFailure("Gitleaks SHA-256 must contain exactly 64 hex digits")
    required_url = (
        'readonly GITLEAKS_DOWNLOAD_URL="https://github.com/gitleaks/gitleaks/'
        'releases/download/v${GITLEAKS_VERSION}/${GITLEAKS_ASSET}"'
    )
    if required_url not in source:
        raise SecurityToolingFailure("Gitleaks download must use the pinned upstream release")
    if "actual_sha256" not in source or "checksum mismatch" not in source:
        raise SecurityToolingFailure("Gitleaks installer must verify bytes before extraction")

    with tempfile.TemporaryDirectory(prefix="hxc-security-tooling-") as directory:
        root = Path(directory)
        tampered = root / "tampered.tar.gz"
        destination = root / "bin"
        tampered.write_bytes(b"not the reviewed Gitleaks archive")
        result = subprocess.run(
            [
                "bash",
                str(INSTALLER),
                "--archive",
                str(tampered),
                "--install-dir",
                str(destination),
            ],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if result.returncode == 0 or "checksum mismatch" not in result.stderr:
            raise SecurityToolingFailure("tampered Gitleaks archive was not rejected")
        if (destination / "gitleaks").exists():
            raise SecurityToolingFailure("tampered Gitleaks bytes reached the install path")
    return version, digest


def workflow_files() -> list[Path]:
    return sorted(
        (
            path
            for path in WORKFLOW_ROOT.iterdir()
            if path.suffix in {".yml", ".yaml"}
        ),
        key=lambda path: path.name.encode("utf-8"),
    )


def validate_workflows() -> int:
    count = 0
    for path in workflow_files():
        source = read_text(path)
        if "pull_request_target:" in source:
            raise SecurityToolingFailure(
                f"{path.name} must not run untrusted changes with pull_request_target"
            )
        if "write-all" in source or "id-token: write" in source:
            raise SecurityToolingFailure(
                f"{path.name} broadened workflow authority without a reviewed need"
            )
        for line_number, line in enumerate(source.splitlines(), start=1):
            match = re.match(r"^\s*uses:\s*([^\s#]+)", line)
            if match is None or match.group(1).startswith("./"):
                continue
            action = match.group(1)
            if "@" not in action:
                raise SecurityToolingFailure(
                    f"{path.name}:{line_number} external action has no revision"
                )
            name, revision = action.rsplit("@", 1)
            if re.fullmatch(r"[0-9a-f]{40}", revision) is None:
                raise SecurityToolingFailure(
                    f"{path.name}:{line_number} external action must use a full commit SHA"
                )
            expected = EXPECTED_ACTIONS.get(name)
            if expected is None:
                raise SecurityToolingFailure(
                    f"{path.name}:{line_number} {name} is not in the reviewed Action registry"
                )
            if revision != expected:
                raise SecurityToolingFailure(
                    f"{path.name}:{line_number} {name} drifted from its reviewed commit"
                )
            count += 1

    workflow = read_text(WORKFLOW)
    for required in (
        "  secret-scan:\n",
        "  haxe-format:\n",
        "fetch-depth: 0",
        "bash scripts/ci/install-gitleaks.sh --install-dir",
        "bash scripts/security/run-gitleaks.sh",
        "npx --no-install haxelib install formatter 1.18.0 --quiet",
        "npm run format:haxe:check",
    ):
        if required not in workflow:
            raise SecurityToolingFailure(
                f"governance workflow lost public-repository guard: {required.strip()}"
            )
    return count


def validate_config_and_hooks() -> None:
    try:
        config = tomllib.loads(read_text(GITLEAKS_CONFIG))
    except tomllib.TOMLDecodeError as error:
        raise SecurityToolingFailure(f"invalid .gitleaks.toml: {error}") from error
    if config.get("minVersion") != "8.30.0":
        raise SecurityToolingFailure("Gitleaks config must reject older scanner semantics")
    if config.get("extend") != {"useDefault": True}:
        raise SecurityToolingFailure("Gitleaks must retain the complete default rule set")
    allowlist = config.get("allowlist")
    if not isinstance(allowlist, dict) or allowlist.get("condition") != "AND":
        raise SecurityToolingFailure("Gitleaks exceptions must combine path and match constraints")
    if allowlist.get("regexTarget") != "match":
        raise SecurityToolingFailure("Gitleaks deterministic-hash exception must inspect the full match")
    if allowlist.get("paths") != [r"^docs/specs/stdlib-ledger\.json$"]:
        raise SecurityToolingFailure(
            "Gitleaks path exception broadened beyond the generated ledger"
        )
    regexes = allowlist.get("regexes")
    if regexes != [EXPECTED_GITLEAKS_REGEX]:
        raise SecurityToolingFailure("Gitleaks must retain one narrow deterministic-hash exception")

    pre_commit = read_text(ROOT / "scripts/hooks/pre-commit")
    if "scripts/security/run-gitleaks.sh\" --staged" not in pre_commit:
        raise SecurityToolingFailure("pre-commit must scan staged content with Gitleaks")
    if "scripts/lint/hx_format_guard.sh\" --tool-only" not in pre_commit:
        raise SecurityToolingFailure("pre-commit must require the pinned Haxe formatter")
    pre_push = read_text(ROOT / "scripts/hooks/pre-push")
    if "scripts/security/run-gitleaks.sh\"" not in pre_push or "--staged" in pre_push:
        raise SecurityToolingFailure("pre-push must scan full reachable Git history")
    beads_pre_commit = read_text(ROOT / ".beads/hooks/pre-commit")
    beads_pre_push = read_text(ROOT / ".beads/hooks/pre-push")
    if "scripts/hooks/pre-commit" not in beads_pre_commit:
        raise SecurityToolingFailure("active Beads pre-commit wrapper lost repository checks")
    if "scripts/hooks/pre-push" not in beads_pre_push:
        raise SecurityToolingFailure("active Beads pre-push wrapper lost repository checks")
    validate_hook_failure_propagation()

    git_scan = read_text(ROOT / "scripts/security/run-gitleaks.sh")
    for required in (
        'readonly DOLT_REMOTE_REF="refs/dolt/data"',
        "ls-remote",
        'DOLT_REMOTE_REF:$DOLT_LOCAL_REF',
        'gitleaks git . --redact --log-opts="--all"',
    ):
        if required not in git_scan:
            raise SecurityToolingFailure(
                f"full-history scan lost Beads/custom-ref coverage: {required}"
            )

    beads_scan = read_text(ROOT / "scripts/security/run-beads-gitleaks.sh")
    for required in (
        "export --all",
        'history "$issue_id" --json',
        "gitleaks stdin",
    ):
        if required not in beads_scan:
            raise SecurityToolingFailure(
                f"decoded Beads history scan lost required behavior: {required}"
            )
    safe_push = read_text(ROOT / "scripts/beads/push-safe.sh")
    if (
        "scripts/security/run-beads-gitleaks.sh" not in safe_push
        or 'dolt push "$@"' not in safe_push
    ):
        raise SecurityToolingFailure(
            "Beads publication must scan decoded history before Dolt push"
        )
    installer = read_text(ROOT / "scripts/hooks/install.sh")
    if "core.hooksPath .beads/hooks" not in installer:
        raise SecurityToolingFailure("hook installer must preserve the Beads hook chain")

    ignored = {
        line.strip()
        for line in read_text(ROOT / ".gitignore").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    }
    missing = sorted(REQUIRED_IGNORES - ignored)
    if missing:
        raise SecurityToolingFailure(
            "credential ignore baseline is incomplete: " + ", ".join(missing)
        )

    tracked_result = subprocess.run(
        ["git", "ls-files", "-z"],
        cwd=ROOT,
        check=True,
        capture_output=True,
        timeout=30,
    )
    tracked = tracked_result.stdout.decode("utf-8").split("\0")
    sensitive_patterns = (
        ".env",
        ".env.*",
        ".npmrc",
        ".pypirc",
        ".netrc",
        "credentials.json",
        "credentials.*.json",
        "service-account*.json",
        "id_rsa",
        "id_rsa.*",
        "id_ed25519",
        "id_ed25519.*",
        "*.pem",
        "*.key",
        "*.p12",
        "*.pfx",
        "*.jks",
        "*.keystore",
        "*.kdbx",
        "*.ovpn",
    )
    sensitive_tracked = []
    for path in tracked:
        if not path or path.endswith("/.env.example") or path == ".env.example":
            continue
        name = Path(path).name
        if any(fnmatch.fnmatchcase(name, pattern) for pattern in sensitive_patterns):
            sensitive_tracked.append(path)
    if sensitive_tracked:
        raise SecurityToolingFailure(
            "credential-shaped files are tracked: " + ", ".join(sensitive_tracked)
        )

    package = json.loads(read_text(PACKAGE))
    scripts = package.get("scripts", {})
    expected_scripts = {
        "format:haxe:check": "bash scripts/lint/hx_format_guard.sh",
        "security:gitleaks": "bash scripts/security/run-gitleaks.sh",
        "security:gitleaks:staged": "bash scripts/security/run-gitleaks.sh --staged",
        "security:beads-history": "bash scripts/security/run-beads-gitleaks.sh",
        "beads:push": "bash scripts/beads/push-safe.sh",
        "test:security-tooling": "python3 scripts/ci/check_security_tooling.py",
        "public:preflight": PUBLIC_PREFLIGHT_COMMAND,
    }
    for name, command in expected_scripts.items():
        if scripts.get(name) != command:
            raise SecurityToolingFailure(f"package.json lost {name}: {command}")

    for relative in (
        ".beads/hooks/pre-commit",
        ".beads/hooks/pre-push",
        "scripts/hooks/install.sh",
        "scripts/hooks/pre-commit",
        "scripts/hooks/pre-push",
        "scripts/beads/push-safe.sh",
        "scripts/lint/hx_format_guard.sh",
        "scripts/security/run-beads-gitleaks.sh",
        "scripts/security/run-gitleaks.sh",
        "scripts/ci/install-gitleaks.sh",
    ):
        require_executable(relative)


def main() -> int:
    try:
        version, digest = validate_installer()
        action_count = validate_workflows()
        validate_config_and_hooks()
        print(
            "security-tooling: OK: "
            f"Gitleaks {version} ({digest}), {action_count} commit-pinned Action uses, "
            "staged/Git/Dolt-history gates, fail-closed hook delegation, "
            "formatter 1.18.0, and credential ignores"
        )
        return 0
    except (OSError, UnicodeError, json.JSONDecodeError, SecurityToolingFailure) as error:
        print(f"security-tooling: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
