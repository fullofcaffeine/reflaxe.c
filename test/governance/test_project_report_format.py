"""Protect report formatting without imposing it on optional inspection dumps."""

from __future__ import annotations

import importlib.util
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

spec = importlib.util.spec_from_file_location("project_report_format_emitter", ROOT / "test/project_emitter/run.py")
assert spec is not None and spec.loader is not None
emitter = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = emitter
spec.loader.exec_module(emitter)


class ProjectReportFormatTests(unittest.TestCase):
    def test_inspection_and_other_json_keep_their_format(self) -> None:
        paths = (
            "hxc.typed-inventory.json", "hxc.hxcir.json", "hxc.c-ast.json",
            "hxc.declarations.json", "_GeneratedFiles.json", "engine.json",
            "build/metadata.json", "nested/hxc.symbols.json",
        )
        emitter.check_compact_reports({path: b'{\n  "text": "inspection"\n}\n' for path in paths})

    def test_report_strings_keep_spaces_escapes_and_unicode(self) -> None:
        payload = '{"text":"space é 🙂 \\n \\t \\\" \\\\"}\n'.encode()
        emitter.check_compact_reports({"hxc.symbols.json": payload})

    def test_reports_require_compact_bytes_and_exact_final_lf(self) -> None:
        for path in (
            "hxc.manifest.json", "hxc.symbols.json", "hxc.runtime-plan.json",
            "hxc.abi.json", "hxc.stdlib-report.json", "hxc.initialization-plan.json",
            "hxc.specializations.json", "hxc.dispatch.json",
        ):
            for payload in (b'{ "x":1}\n', b'{\n"x":1}\n', b'{}', b'{}\n\n', b'{}\r\n'):
                with self.subTest(path=path, payload=payload):
                    with self.assertRaises(emitter.ProjectEmitterFailure):
                        emitter.check_compact_reports({path: payload})
            emitter.check_compact_reports({path: b'{"x":1}\n'})


if __name__ == "__main__":
    unittest.main()
