#!/usr/bin/env python3
"""Prove typed C11 atomics, strict native semantics, and no pthread linkage."""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURES = Path(__file__).with_name("fixtures")
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
HAXE_TIMEOUT_SECONDS = 120


class AtomicLoweringFailure(RuntimeError):
    """Reports one failed atomic compiler or native contract."""


def development_tool(name: str) -> str:
    """Use the repository-pinned executable when it is installed."""
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def generate(main_class: str, output: Path) -> subprocess.CompletedProcess[str]:
    """Compile one fixture through this checkout without a shared Haxe server."""
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURES),
        "-lib",
        "reflaxe.c",
        "-main",
        main_class,
        "-D",
        "hxc_atomic_backend=c11",
        "-D",
        "hxc_project_layout=unity",
        "--custom-target",
        f"c={output}",
    ]
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=HAXE_TIMEOUT_SECONDS,
    )


def require_success(result: subprocess.CompletedProcess[str], label: str) -> None:
    """Fail with the compiler transcript when a positive fixture is rejected."""
    if result.returncode != 0:
        raise AtomicLoweringFailure(
            f"{label} failed with {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def require_failure(main_class: str, fragment: str, temporary: Path) -> None:
    """Require one exact fail-closed diagnostic and no published C source."""
    output = temporary / main_class
    result = generate(main_class, output)
    transcript = result.stdout + result.stderr
    if result.returncode == 0 or fragment not in transcript:
        raise AtomicLoweringFailure(
            f"{main_class} did not report {fragment!r}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if (output / "src/program.c").exists():
        raise AtomicLoweringFailure(f"{main_class} published C after its diagnostic")


def main() -> int:
    """Run positive generation, structural checks, strict C, and negative cases."""
    compiler = shutil.which("cc")
    if compiler is None:
        raise AtomicLoweringFailure("no C compiler is available for the native proof")
    with tempfile.TemporaryDirectory(prefix="hxc-atomic-lowering-") as raw:
        temporary = Path(raw)
        output = temporary / "positive"
        result = generate("AtomicFixture", output)
        require_success(result, "AtomicFixture generation")

        program = (output / "src/program.c").read_text(encoding="utf-8")
        header = (output / "include/hxc/program.h").read_text(encoding="utf-8")
        project_text = program + header + (output / "cmake/CMakeLists.txt").read_text(encoding="utf-8") + (output / "meson.build").read_text(encoding="utf-8")
        required = (
            "#include <stdatomic.h>",
            "_Atomic(int32_t)",
            "_Atomic(bool)",
            "HXC_TYPE_DESCRIPTOR_HAS_TRACE",
            "_trace_visit_atomic_",
            "atomic_init(",
            "atomic_load_explicit(",
            "atomic_store_explicit(",
            "atomic_exchange_explicit(",
            "atomic_compare_exchange_strong_explicit(",
            "atomic_fetch_add_explicit(",
            "atomic_fetch_sub_explicit(",
            "atomic_fetch_and_explicit(",
            "atomic_fetch_or_explicit(",
            "atomic_fetch_xor_explicit(",
        )
        missing = [fragment for fragment in required if fragment not in project_text]
        if missing:
            raise AtomicLoweringFailure("generated project lacks: " + ", ".join(missing))
        if "pthread" in project_text.lower():
            raise AtomicLoweringFailure("atomic-only output unexpectedly references pthread")

        executable = temporary / "atomic-probe"
        manifest = json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8"))
        sources = [str(output / path) for path in manifest["build"]["sources"]]
        include_flags = [
            flag
            for path in manifest["build"]["includeDirectories"]
            for flag in ("-I", str(output / path))
        ]
        native = subprocess.run(
            [compiler, *STRICT_FLAGS, *sources, *include_flags, "-o", str(executable)],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        require_success(native, "strict native compile")
        run = subprocess.run([str(executable)], check=False, capture_output=True, text=True, timeout=10)
        require_success(run, "atomic semantic executable")

        require_failure("InvalidLoadOrderFixture", "atomic load cannot use release", temporary)
        require_failure("InvalidAtomicCopyFixture", "atomic storage is non-copyable", temporary)

    print("atomic-lowering: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
