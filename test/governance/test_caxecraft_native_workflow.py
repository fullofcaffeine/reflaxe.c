"""Protect one cold macOS generation and both independently retained native builds."""
from __future__ import annotations

import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / ".github/workflows/governance.yml"


def steps() -> list[tuple[str, str]]:
    text = WORKFLOW.read_text(encoding="utf-8")
    return re.findall(r"^      - name: ([^\n]+)\n(.*?)(?=^      - name:|\Z)", text, re.M | re.S)


class NativeWorkflowContract(unittest.TestCase):
    @unittest.skipIf(os.name == "nt", "executes the POSIX-only native workflow branches")
    def test_macos_reuses_generation_but_linux_keeps_pilot_inputs(self) -> None:
        selected = [(name, body) for name, body in steps() if name in (
            "Compile cold Caxecraft and link at O0",
            "Link Caxecraft at O2 and run the Linux desktop pilot",
        )]
        self.assertEqual(len(selected), 2)
        for platform in ("macos", "linux"):
            with self.subTest(platform=platform), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                log = root / "commands.jsonl"
                # Run the actual workflow shell with command observers. No compiler
                # or display server runs; independent assertions inspect its argv.
                python = root / "python3"
                python.write_text(
                    f'#!{sys.executable}\nimport json, os, sys\n'
                    'with open(os.environ["COMMAND_LOG"], "a") as stream:\n'
                    '    stream.write(json.dumps(sys.argv[1:]) + "\\n")\n'
                )
                python.chmod(0o755)
                xvfb = root / "xvfb-run"
                xvfb.write_text('#!/bin/sh\nshift\nshift\nexec "$@"\n')
                xvfb.chmod(0o755)
                env = {**os.environ, "PATH": str(root) + os.pathsep + os.environ["PATH"],
                       "RUNNER_TEMP": str(root), "COMMAND_LOG": str(log)}
                for _, body in selected:
                    script = body.split("        run: |\n", 1)[1]
                    script = "\n".join(line[10:] for line in script.splitlines() if line.startswith("          "))
                    for key, value in (("platform", platform), ("cc", "clang"), ("cxx", "clang++")):
                        script = script.replace("${{ matrix." + key + " }}", value)
                    subprocess.run(["bash", "-euo", "pipefail", "-c", script], env=env, check=True, timeout=15)
                commands = [json.loads(line) for line in log.read_text().splitlines()]
                self.assertEqual(len(commands), 3)
                self.assertIn("--compile-only", commands[0])
                self.assertIn("--build-only", commands[1])
                self.assertEqual(commands[1][commands[1].index("--optimization") + 1], "0")
                self.assertEqual(commands[2][commands[2].index("--optimization") + 1], "2")
                roots = [command[command.index("--output-root") + 1] for command in commands]
                self.assertEqual(roots[0], roots[1])
                if platform == "macos":
                    self.assertIn("--build-only", commands[2])
                    self.assertNotIn("--pilot", commands[2])
                    self.assertEqual(roots[0], roots[2])
                else:
                    self.assertEqual(commands[2][commands[2].index("--pilot") + 1], "resize-layout")
                    self.assertNotIn("--build-only", commands[2])
                    self.assertNotEqual(roots[0], roots[2])
                self.assertFalse(any("--haxe-timeout-seconds" in command for command in commands))

    def test_native_evidence_is_captured_before_each_overwrite(self) -> None:
        all_steps = steps()
        names = [name for name, _ in all_steps]
        order = [names.index(name) for name in (
            "Compile cold Caxecraft and link at O0",
            "Preserve macOS Caxecraft O0 native evidence",
            "Link Caxecraft at O2 and run the Linux desktop pilot",
            "Preserve macOS Caxecraft O2 native evidence",
        )]
        self.assertEqual(order, sorted(order))
        for optimization in (0, 2):
            body = dict(all_steps)[f"Preserve macOS Caxecraft O{optimization} native evidence"]
            self.assertIn("if: matrix.platform == 'macos'", body)
            self.assertIn(f"name: caxecraft-native-o{optimization}-${{{{ matrix.name }}}}", body)
            self.assertIn("if-no-files-found: error", body)
            for evidence in ("bin", "generations", "hxc-play-current-generation.json", "native-cache/**/metadata.json"):
                self.assertIn("/variants/macos-split-interactive/" + evidence + "\n", body)
            self.assertNotIn("hxc-play-build-state.json", body)


if __name__ == "__main__":
    unittest.main()
