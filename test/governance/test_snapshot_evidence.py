#!/usr/bin/env python3
"""Exercise audit retention without invoking Haxe or changing expected output."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path, PureWindowsPath
from unittest.mock import patch
from types import SimpleNamespace
from contextlib import redirect_stderr
import io

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from scripts.test import snapshots


class SnapshotEvidenceTests(unittest.TestCase):
    def test_full_project_and_unselected_dependency_survive_cleanup(self):
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            with snapshots.snapshot_project_root("snapshot-test-", evidence) as generated:
                (generated / "include").mkdir()
                (generated / "include/hidden.h").write_bytes(b"struct Hidden { int value; };\n")
                (generated / "selected.c").write_bytes(b"#include \"include/hidden.h\"\n")
                (generated / "hxc.manifest.json").write_text('{"artifacts":[{"path":"include/hidden.h"}]}')
            self.assertFalse(generated.exists())
            self.assertEqual((evidence / "project/include/hidden.h").read_bytes(), b"struct Hidden { int value; };\n")
            receipt = json.loads((evidence / "capture.json").read_text())
            self.assertEqual(receipt["captureStatus"], "complete")
            self.assertEqual(set(receipt["files"]), {"selected.c", "include/hidden.h", "hxc.manifest.json"})

    def test_render_failure_retains_partial_tree_and_original_exception(self):
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            with self.assertRaisesRegex(RuntimeError, "original render"):
                with snapshots.snapshot_project_root("snapshot-test-", evidence) as generated:
                    (generated / "partial.c").write_text("partial output")
                    raise RuntimeError("original render")
            self.assertFalse(generated.exists())
            receipt = json.loads((evidence / "capture.json").read_text())
            self.assertEqual(receipt["renderStatus"], "failed")
            self.assertEqual(receipt["errorType"], "RuntimeError")
            self.assertEqual((evidence / "project/partial.c").read_text(), "partial output")

    def test_existing_destination_is_never_overwritten(self):
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            evidence.mkdir()
            sentinel = evidence / "capture.json"
            sentinel.write_text("owned elsewhere")
            with self.assertRaisesRegex(RuntimeError, "primary"):
                with snapshots.snapshot_project_root("snapshot-test-", evidence):
                    raise RuntimeError("primary")
            self.assertEqual(sentinel.read_text(), "owned elsewhere")

    def test_symlink_and_local_path_output_are_not_published(self):
        for mode in ("symlink", "local-path"):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temporary:
                evidence = Path(temporary).resolve() / "evidence"
                with self.assertRaises(snapshots.SnapshotFailure):
                    with snapshots.snapshot_project_root("snapshot-test-", evidence) as generated:
                        if mode == "symlink":
                            (generated / "escape").symlink_to(ROOT / "package.json")
                        else:
                            (generated / "leak.c").write_text(str(ROOT))
                self.assertFalse((evidence / "project").exists())
                self.assertEqual(json.loads((evidence / "capture.json").read_text())["captureStatus"], "incomplete")

    def test_escaped_windows_path_cannot_escape_project_privacy_check(self):
        home = PureWindowsPath("C:/fixture-home")
        for spelling in (str(home / "private/source.hx"), (home / "private/source.hx").as_posix()):
            with self.subTest(spelling=spelling), tempfile.TemporaryDirectory() as temporary:
                evidence = Path(temporary).resolve() / "evidence"
                with patch.object(snapshots.Path, "home", return_value=home):
                    with self.assertRaises(snapshots.SnapshotFailure):
                        with snapshots.snapshot_project_root("snapshot-test-", evidence) as generated:
                            (generated / "metadata.json").write_text(json.dumps({"source": spelling}))
                self.assertFalse((evidence / "project").exists())
                self.assertEqual(json.loads((evidence / "capture.json").read_text())["captureStatus"], "incomplete")

    def test_new_root_rejects_checkout_existing_and_symlink_paths(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            checkout = root / "checkout"
            checkout.mkdir()
            with patch.object(snapshots, "ROOT", checkout):
                with self.assertRaises(snapshots.SnapshotFailure):
                    snapshots.new_evidence_root(checkout / "expected")
                with self.assertRaises(FileExistsError):
                    snapshots.new_evidence_root(root)
                (root / "link").symlink_to(checkout, target_is_directory=True)
                with self.assertRaises(snapshots.SnapshotFailure):
                    snapshots.new_evidence_root(root / "link/evidence")
                self.assertEqual(snapshots.new_evidence_root(root / "fresh"), root / "fresh")

    def test_playable_owner_passes_complete_project_to_retention(self):
        sys.path.insert(0, str(ROOT / "examples/caxecraft"))
        import play
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            def compile_fixture(generated, **options):
                self.assertEqual(options, {"layout": "split", "platform_name": play.SNAPSHOT_PLATFORM,
                                           "raylib_configuration": "desktop", "runtime_report": "full"})
                generated.mkdir()
                (generated / "selected.c").write_text("selected")
                (generated / "unselected.c").write_text("full dependency")
            with (patch.object(play, "compile_haxe", side_effect=compile_fixture),
                  patch.object(play, "PLAYABLE_SNAPSHOT_FORMATS", {"playable/selected.c": "c"})):
                values = play.snapshot_values(evidence_dir=evidence)
            self.assertEqual(values, {"playable/selected.c": "selected"})
            self.assertEqual((evidence / "playable/project/generated/unselected.c").read_text(), "full dependency")

    def test_unchanged_generators_keep_zero_argument_render_contract(self):
        class Values(dict):
            def get(self, name, default=None):
                return {} if name.endswith(".json") else "fixture text"
        for generate in (snapshots.raylib_provisioning_artifacts,
                         snapshots.static_initialization_artifacts,
                         snapshots.primitive_differential_artifacts,
                         snapshots.string_output_artifacts, snapshots.hello_artifacts):
            with self.subTest(generator=generate.__name__):
                calls = []
                def render():  # Deliberately does not accept evidence keywords.
                    calls.append(True)
                    return Values()
                module = SimpleNamespace(snapshot_values=render, load_lock=lambda: {},
                                         render_files=lambda lock: {}, render_abi_probe=lambda lock: "probe",
                                         render_raw=lambda lock: "raw")
                with patch.object(snapshots, "load_module", return_value=module):
                    self.assertTrue(generate())
                self.assertEqual(len(calls), 2)

    def test_final_report_write_failure_does_not_replace_render_failure(self):
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            real_write = snapshots.evidence_json
            def write(path, value):
                if isinstance(value, dict) and value.get("status") == "failed":
                    raise PermissionError("report destination unavailable")
                real_write(path, value)
            def fail():
                raise snapshots.SnapshotFailure("original render failure")
            diagnostics = io.StringIO()
            with (patch.object(snapshots, "read_catalog", return_value={}),
                  patch.object(snapshots, "managed_entries", return_value={"fixture": {}}),
                  patch.object(snapshots, "evidence_provenance", return_value={}),
                  patch.dict(snapshots.GENERATORS, {"fixture": fail}),
                  patch.object(snapshots, "evidence_json", side_effect=write), redirect_stderr(diagnostics)):
                self.assertEqual(snapshots.main(["--check", "--evidence-dir", str(evidence)]), 1)
            self.assertIn("original render failure", diagnostics.getvalue())
            self.assertIn("primary failure preserved", diagnostics.getvalue())

    def test_cli_failure_report_keeps_prior_completed_suite(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            evidence = root / "evidence"
            def fail():
                raise snapshots.SnapshotFailure("render failed")
            with (patch.object(snapshots, "read_catalog", return_value={}),
                  patch.object(snapshots, "managed_entries", return_value={"first": {}, "second": {}}),
                  patch.object(snapshots, "evidence_provenance", return_value={"revision": "fixture"}),
                  patch.dict(snapshots.GENERATORS, {"first": lambda: [], "second": fail}),
                  patch.object(snapshots, "collect_changes", return_value=[])):
                self.assertEqual(snapshots.main(["--check", "--evidence-dir", str(evidence)]), 1)
            report = json.loads((evidence / "report.json").read_text())
            self.assertEqual(report["suites"], ["first"])
            self.assertEqual(report["status"], "failed")
            self.assertEqual(report["errorType"], "SnapshotFailure")
            self.assertTrue((evidence / "first/artifacts.json").exists())

    def test_unhandled_generator_failure_is_reported_without_replacing_exception(self):
        class GeneratorFailure(RuntimeError):
            pass

        for fail_report_write in (False, True):
            with self.subTest(fail_report_write=fail_report_write), tempfile.TemporaryDirectory() as temporary:
                evidence = Path(temporary).resolve() / "evidence"
                failure = GeneratorFailure("original generator failure")
                real_write = snapshots.evidence_json

                def fail():
                    with snapshots.snapshot_project_root("snapshot-test-", evidence / "second") as generated:
                        (generated / "partial.c").write_text("partial output")
                        raise failure

                def write(path, value):
                    if (fail_report_write and path == evidence / "report.json"
                            and isinstance(value, dict) and value.get("status") == "failed"):
                        raise PermissionError("report destination unavailable")
                    real_write(path, value)

                diagnostics = io.StringIO()
                with (patch.object(snapshots, "read_catalog", return_value={}),
                      patch.object(snapshots, "managed_entries", return_value={"first": {}, "second": {}}),
                      patch.object(snapshots, "evidence_provenance", return_value={}),
                      patch.dict(snapshots.GENERATORS, {"first": lambda: [], "second": fail}),
                      patch.object(snapshots, "collect_changes", return_value=[]),
                      patch.object(snapshots, "evidence_json", side_effect=write),
                      patch.object(snapshots, "write_artifact", side_effect=AssertionError("unexpected snapshot write")),
                      redirect_stderr(diagnostics)):
                    try:
                        snapshots.main(["--check", "--evidence-dir", str(evidence)])
                    except GeneratorFailure as observed:
                        self.assertIs(observed, failure)
                        frames = []
                        traceback = observed.__traceback__
                        while traceback is not None:
                            frames.append(traceback.tb_frame.f_code)
                            traceback = traceback.tb_next
                        self.assertIn(fail.__code__, frames)
                    else:
                        self.fail("the original generator exception must propagate")

                self.assertTrue((evidence / "first/artifacts.json").exists())
                self.assertEqual((evidence / "second/project/partial.c").read_text(), "partial output")
                capture = json.loads((evidence / "second/capture.json").read_text())
                self.assertEqual(capture["renderStatus"], "failed")
                self.assertEqual(capture["captureStatus"], "complete")
                self.assertEqual(capture["errorType"], "GeneratorFailure")
                report = json.loads((evidence / "report.json").read_text())
                self.assertEqual(report["suites"], ["first"])
                self.assertEqual(report["activeSuite"], "second")
                self.assertNotIn("original generator failure", json.dumps(report))
                if fail_report_write:
                    self.assertIn("primary failure preserved", diagnostics.getvalue())
                else:
                    self.assertEqual(report["status"], "failed")
                    self.assertEqual(report["errorType"], "GeneratorFailure")

    def test_success_report_ignores_callers_handled_exception(self):
        with tempfile.TemporaryDirectory() as temporary:
            evidence = Path(temporary).resolve() / "evidence"
            with (patch.object(snapshots, "read_catalog", return_value={}),
                  patch.object(snapshots, "managed_entries", return_value={"fixture": {}}),
                  patch.object(snapshots, "evidence_provenance", return_value={}),
                  patch.dict(snapshots.GENERATORS, {"fixture": lambda: []}),
                  patch.object(snapshots, "collect_changes", return_value=[])):
                try:
                    raise RuntimeError("unrelated handled caller error")
                except RuntimeError:
                    self.assertEqual(snapshots.main(["--check", "--evidence-dir", str(evidence)]), 0)
            report = json.loads((evidence / "report.json").read_text())
            self.assertEqual(report["status"], "matched")
            self.assertNotIn("errorType", report)


if __name__ == "__main__":
    unittest.main()
