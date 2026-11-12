"""Keep sanitizer runtime selection local to the selected compiler and child."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from scripts.test.compiler_discovery import sanitizer_execution_environment

FLAGS = ("-fsanitize=address,undefined",)


class SanitizerEnvironmentTests(unittest.TestCase):
    def test_unaffected_runs_do_not_query_or_mutate(self):
        original = {"PATH": "/tools", "NEKOPATH": "/neko", "ASAN_OPTIONS": "detect_leaks=0"}
        for platform, family, flags in (("linux", "clang", FLAGS), ("darwin", "gcc", FLAGS), ("darwin", "clang", ())):
            with self.subTest(platform=platform, family=family, flags=flags), patch("scripts.test.compiler_discovery.sys.platform", platform), patch("scripts.test.compiler_discovery.subprocess.run") as query:
                result = sanitizer_execution_environment("/selected/clang", family, flags, environment=original)
                self.assertEqual(result, original)
                self.assertIsNot(result, original)
                query.assert_not_called()

    def test_selected_runtime_and_existing_environment(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary).resolve()
            (directory / "libclang_rt.asan_osx_dynamic.dylib").touch()
            original = {"DYLD_LIBRARY_PATH": f"/previous:{directory}", "NEKOPATH": "/neko", "ASAN_OPTIONS": "detect_leaks=0"}
            response = subprocess.CompletedProcess([], 0, str(directory) + "\n", "")
            with patch("scripts.test.compiler_discovery.sys.platform", "darwin"), patch("scripts.test.compiler_discovery.subprocess.run", return_value=response) as query:
                result = sanitizer_execution_environment("/selected/clang", "clang", FLAGS, environment=original)
                query.assert_called_once_with(["/selected/clang", "--print-runtime-dir"], capture_output=True, text=True, check=False, timeout=10)
            self.assertEqual(result, {**original, "DYLD_LIBRARY_PATH": f"{directory}:/previous"})
            self.assertEqual(original["DYLD_LIBRARY_PATH"], f"/previous:{directory}")

    def test_invalid_or_missing_runtime_fails_closed(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary).resolve()
            for output, status, stderr in (("", 0, ""), ("relative", 0, ""), (f"{directory}\n/other", 0, ""), (str(directory), 1, ""), (str(directory), 0, "warning"), (str(directory / "missing"), 0, ""), (str(directory), 0, "")):
                with self.subTest(output=output, status=status, stderr=stderr), patch("scripts.test.compiler_discovery.sys.platform", "darwin"), patch("scripts.test.compiler_discovery.subprocess.run", return_value=subprocess.CompletedProcess([], status, output, stderr)):
                    with self.assertRaises(RuntimeError):
                        sanitizer_execution_environment("/selected/clang", "clang", FLAGS, environment={})

    @unittest.skipUnless(sys.platform == "darwin", "Darwin loader contract")
    def test_npm_shell_loads_runtime_and_keeps_address_checks(self):
        """The shell must not supply the runtime path that the helper restores."""
        npm = shutil.which("npm")
        self.assertIsNotNone(npm, "npm is required by the development toolchain")
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            command = shlex.join([sys.executable, str(Path(__file__).resolve()), "--native-child"])
            (directory / "package.json").write_text(json.dumps({"private": True, "scripts": {"verify": command}}))
            environment = dict(os.environ)
            environment.pop("DYLD_LIBRARY_PATH", None)
            result = subprocess.run([npm, "--silent", "run", "verify"], cwd=directory, env=environment, capture_output=True, text=True, timeout=90)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            report = json.loads(result.stdout)
            self.assertEqual(report["validExit"], 0)
            self.assertTrue(report["addressViolationDetected"])
            print(json.dumps(report, sort_keys=True))


def native_child():
    """Run after npm's shell, proving both runtime loading and active checks."""
    assert "DYLD_LIBRARY_PATH" not in os.environ
    compiler = shutil.which("clang")
    assert compiler is not None
    source = Path("probe.c")
    source.write_text("#include <stdlib.h>\nint main(int argc, char **argv) { (void)argv; volatile int index = argc > 1; int *p = malloc(sizeof(int)); if (!p) return 2; p[index] = 7; free(p); return 0; }\n")
    executable = Path("probe").resolve()
    compiled = subprocess.run([compiler, "-O0", *FLAGS, str(source), "-o", str(executable)], capture_output=True, text=True, timeout=30)
    assert compiled.returncode == 0 and not compiled.stdout and not compiled.stderr, compiled
    environment = sanitizer_execution_environment(compiler, "clang", FLAGS)
    environment["ASAN_OPTIONS"] = "detect_leaks=0"
    valid = subprocess.run([str(executable)], env=environment, capture_output=True, text=True, timeout=10)
    assert valid.returncode == 0 and not valid.stdout and not valid.stderr, valid
    invalid = subprocess.run([str(executable), "overflow"], env=environment, capture_output=True, text=True, timeout=10)
    assert invalid.returncode != 0 and "AddressSanitizer: heap-buffer-overflow" in invalid.stderr, invalid
    runtime = Path(environment["DYLD_LIBRARY_PATH"].split(":")[0]) / "libclang_rt.asan_osx_dynamic.dylib"
    print(json.dumps({"compiler": compiler, "compilerSha256": hashlib.sha256(Path(compiler).read_bytes()).hexdigest(), "runtime": str(runtime), "runtimeSha256": hashlib.sha256(runtime.read_bytes()).hexdigest(), "validExit": valid.returncode, "invalidExit": invalid.returncode, "addressViolationDetected": True, "transport": "npm -> shell -> Python -> native"}))


if __name__ == "__main__":
    if sys.argv[1:] == ["--native-child"]:
        native_child()
    else:
        unittest.main()
