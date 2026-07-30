#!/usr/bin/env python3
"""Focused safety tests for Caxecraft's unchanged-build state."""

from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CASE))

from dev_build_state import (  # noqa: E402
    ExternalFile,
    InputPath,
    atomic_write_state,
    build_state,
    output_snapshot,
    request_snapshot,
    validate_reuse,
)
from play import (  # noqa: E402
    PlayFailure,
    haxe_module_path_inventory,
    hosted_content_haxe_defines,
    parse_args,
    play_build_inputs,
    validate_content_platform_output,
)


class CaxecraftBuildStateTests(unittest.TestCase):
    """Prove hits, precise misses, corruption rejection, and atomic publication."""

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-caxecraft-build-state-")
        self.root = Path(self.temporary.name)
        self.source = self.root / "source"
        self.source.mkdir()
        (self.source / "Main.hx").write_text("class Main {}\n", encoding="utf-8")

        self.output_root = self.root / "variant"
        generated = self.output_root / "generated"
        generated.mkdir(parents=True)
        (generated / "main.c").write_text("int main(void) { return 0; }\n", encoding="utf-8")
        self.executable = self.output_root / "bin/caxecraft"
        self.executable.parent.mkdir(parents=True)
        self.executable.write_bytes(b"native-executable")
        (self.executable.parent / "assets").mkdir()
        (self.executable.parent / "assets/hud.png").write_bytes(b"asset")
        (self.executable.parent / "content").mkdir()
        (self.executable.parent / "content/world.caxemap").write_text("world\n", encoding="utf-8")

        self.raylib = self.root / "libraylib.a"
        self.raygui = self.root / "libraygui.a"
        self.raylib.write_bytes(b"raylib")
        self.raygui.write_bytes(b"raygui")
        self.state_path = self.output_root / "hxc-play-build-state.json"
        self.publish()

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def request(
        self,
        *,
        layout: str = "split",
        tool_digest: str = "a" * 64,
    ) -> dict[str, object]:
        return request_snapshot(
            configuration={"layout": layout, "optimization": "2"},
            inputs=[InputPath("repo/source", self.source)],
            tools=[{"name": "c-compiler", "sha256": tool_digest}],
            environment={},
        )

    def publish(self) -> None:
        state = build_state(
            request=self.request(),
            outputs=output_snapshot(self.output_root / "generated", self.executable),
            external_native_files=[
                ExternalFile("raylib/library", self.raylib),
                ExternalFile("raygui/library", self.raygui),
            ],
        )
        atomic_write_state(self.state_path, state)

    def decision(self, request: dict[str, object] | None = None):
        return validate_reuse(
            state_path=self.state_path,
            current_request=request or self.request(),
            generated=self.output_root / "generated",
            executable=self.executable,
        )

    def test_exact_state_is_a_hit(self) -> None:
        decision = self.decision()
        self.assertTrue(decision.hit)
        self.assertEqual(decision.reason, "request and every reusable output match")
        self.assertFalse(any(self.state_path.parent.glob(f".{self.state_path.name}.*.tmp")))

    def test_source_edit_names_the_first_changed_input(self) -> None:
        (self.source / "Main.hx").write_text("class Main { static var changed = true; }\n", encoding="utf-8")
        decision = self.decision(self.request())
        self.assertFalse(decision.hit)
        self.assertEqual(decision.reason, "build input changed: repo/source/Main.hx")

    def test_added_module_cannot_hide_behind_an_old_request(self) -> None:
        (self.source / "Added.hx").write_text("class Added {}\n", encoding="utf-8")
        decision = self.decision(self.request())
        self.assertFalse(decision.hit)
        self.assertEqual(decision.reason, "build input added: repo/source/Added.hx")

    def test_haxe_server_identity_tracks_module_paths_not_body_bytes(self) -> None:
        nested = self.source / "gameplay"
        nested.mkdir()
        module = nested / "Actor.hx"
        module.write_text("class Actor {}\n", encoding="utf-8")
        baseline = haxe_module_path_inventory(self.source)

        module.write_text(
            "class Actor { public static function changed():Bool return true; }\n",
            encoding="utf-8",
        )
        self.assertEqual(haxe_module_path_inventory(self.source), baseline)

        renamed = nested / "RenamedActor.hx"
        module.rename(renamed)
        self.assertNotEqual(haxe_module_path_inventory(self.source), baseline)
        self.assertEqual(
            haxe_module_path_inventory(self.source),
            ["Main.hx", "gameplay/RenamedActor.hx"],
        )

    def test_configuration_and_tool_changes_are_misses(self) -> None:
        configuration = self.decision(self.request(layout="unity"))
        self.assertFalse(configuration.hit)
        self.assertEqual(configuration.reason, "configuration changed: layout")

        tool = self.decision(self.request(tool_digest="b" * 64))
        self.assertFalse(tool.hit)
        self.assertEqual(tool.reason, "native or launcher tool identity changed")

    def test_generated_or_executable_corruption_never_launches(self) -> None:
        (self.output_root / "generated/main.c").write_text("corrupt\n", encoding="utf-8")
        generated = self.decision()
        self.assertFalse(generated.hit)
        self.assertEqual(generated.reason, "build output changed: generated/main.c")

        self.publish()
        self.executable.write_bytes(b"corrupt")
        executable = self.decision()
        self.assertFalse(executable.hit)
        self.assertEqual(executable.reason, "build output changed: native/executable")

    def test_runtime_content_is_republished_instead_of_invalidating_the_build(self) -> None:
        staged = self.executable.parent / "content/world.caxemap"
        staged.write_text("new authored world\n", encoding="utf-8")
        decision = self.decision()
        self.assertTrue(decision.hit)

    def test_windows_generation_selects_typed_unsupported_package_capability(
        self,
    ) -> None:
        self.assertEqual(
            hosted_content_haxe_defines("linux"),
            ("caxecraft_posix_hosted",),
        )
        self.assertEqual(
            hosted_content_haxe_defines("macos"),
            ("caxecraft_posix_hosted", "caxecraft_posix_darwin"),
        )
        self.assertEqual(hosted_content_haxe_defines("windows"), ())

        generated = self.root / "windows-generated"
        source = generated / "src/program.c"
        source.parent.mkdir(parents=True)
        source.write_text(
            "/* typed ContentPackageError_UnsupportedCapability */\n",
            encoding="utf-8",
        )
        manifest: dict[str, object] = {
            "build": {"sources": ["src/program.c"]},
            "files": [{"path": "src/program.c"}],
        }
        validate_content_platform_output(generated, manifest, "windows")

        source.write_text(
            "#include <unistd.h>\nint bad(void) { return openat(0, \"x\", 0); }\n",
            encoding="utf-8",
        )
        with self.assertRaisesRegex(
            PlayFailure,
            "generated Windows Caxecraft sources contain POSIX package calls",
        ):
            validate_content_platform_output(generated, manifest, "windows")

    def test_generated_level_bridge_cannot_reenter_the_play_build(self) -> None:
        removed_paths = (
            CASE / "src/caxecraft/content/FirstPlayableLevel.hx",
            CASE / "src/caxecraft/content/FirstPlayableSessionLoader.hx",
            CASE / "toolsrc/caxecraft/tool/FirstPlayableLevelGenerator.hx",
            CASE / "level-adapter.hxml",
            CASE / "level_adapter.py",
        )
        self.assertFalse(any(path.exists() for path in removed_paths))
        self.assertNotIn("content_authority", vars(parse_args([])))

        scripts = json.loads((ROOT / "package.json").read_text(encoding="utf-8"))[
            "scripts"
        ]
        self.assertNotIn("test:caxecraft-level-adapter", scripts)

    def test_external_native_change_is_a_miss(self) -> None:
        self.raylib.write_bytes(b"changed-raylib")
        decision = self.decision()
        self.assertFalse(decision.hit)
        self.assertEqual(decision.reason, "external native input changed: raylib/library")

    def test_malformed_or_tampered_state_is_never_a_hit(self) -> None:
        self.state_path.write_text("{not json", encoding="utf-8")
        malformed = self.decision()
        self.assertFalse(malformed.hit)
        self.assertEqual(malformed.reason, "state file is unreadable or malformed")

        self.publish()
        value = json.loads(self.state_path.read_text(encoding="utf-8"))
        value["kind"] = "tampered"
        self.state_path.write_text(json.dumps(value), encoding="utf-8")
        tampered = self.decision()
        self.assertFalse(tampered.hit)
        self.assertEqual(tampered.reason, "state schema or kind is not supported")

    def test_missing_state_or_output_is_a_visible_miss(self) -> None:
        self.state_path.unlink()
        missing_state = self.decision()
        self.assertFalse(missing_state.hit)
        self.assertEqual(missing_state.reason, "state file is missing")

        self.publish()
        self.executable.unlink()
        missing_output = self.decision()
        self.assertFalse(missing_output.hit)
        self.assertEqual(missing_output.reason, "generated, staged, or executable output is missing")

    def test_playable_wires_every_semantic_input_family(self) -> None:
        inputs = play_build_inputs(parse_args([]))
        logical_names = {item.logical_name for item in inputs}
        required_roots = {
            "repo/extraParams.hxml",
            "repo/compiler/src",
            "repo/compiler/std",
            "repo/compiler/runtime",
            "repo/vendor/reflaxe",
            "repo/caxecraft/play.hxml",
            "repo/caxecraft/src",
            "repo/caxecraft/assets",
            "repo/caxecraft/locales",
            "repo/caxecraft/packs",
            "repo/caxecraft/tooling/play.py",
            "repo/caxecraft/tooling/dev_build_state.py",
            "repo/caxecraft/tooling/dev_generation.py",
            "repo/caxecraft/tooling/dev_haxe_server.py",
            "repo/caxecraft/tooling/dev_native_cache.py",
            "repo/raylib-tooling/provision.py",
            "repo/raylib-tooling/patches",
            "repo/raylib-tooling/provisioning-lock.json",
            "repo/raygui-tooling/provision.py",
            "repo/raygui-tooling/core-binding-lock.json",
            "tooling/haxeshim.js",
            "haxe/compiler",
            "haxe/std",
        }
        self.assertTrue(required_roots.issubset(logical_names))
        self.assertNotIn("repo/caxecraft/scenarios", logical_names)
        self.assertFalse(
            any("__pycache__" in name or name.endswith(".pyc") for name in logical_names)
        )


if __name__ == "__main__":
    unittest.main()
