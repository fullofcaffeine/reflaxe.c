#!/usr/bin/env python3
"""Host-only contracts for the manifest-owned native compilation recipe."""
from __future__ import annotations

import copy
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "examples/caxecraft"))
import play  # noqa: E402


class NativeCompilePlanTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory(prefix="native-compile-plan-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.generated = self.root / "generated"
        for directory in ("include", "runtime/include", "src", "runtime/src"):
            (self.generated / directory).mkdir(parents=True, exist_ok=True)
        self.names = ["src/z.c", "runtime/src/gc.c", "src/a.c"]
        for name in self.names:
            (self.generated / name).write_text("/* fixture input; no compiler required */\n")
        self.manifest = {"build": {
            "sources": list(self.names),
            "includeDirectories": ["runtime/include", "include"],
            "libraries": [{"name": name, "ownerModulePaths": ["Fixture"]}
                          for name in ("raylib", "raygui", "m")],
            "frameworks": [],
        }}
        self.options = dict(include_directory=self.root / "raylib", raygui_include_directory=self.root / "raygui",
                            platform_name="linux", optimization="2", native_sanitizer_flags=())

    def plan(self, **changes) -> play.NativeCompilePlan:
        return play.native_compile_plan(self.generated, self.manifest, **{**self.options, **changes})

    def test_manifest_order_and_exact_ordinary_recipe(self) -> None:
        plan = self.plan()
        self.assertEqual(plan.sources, tuple((name, self.generated / name) for name in self.names))
        self.assertEqual([(root.logical_name, root.path) for root in plan.include_roots], [
            ("generated-include-0", self.generated / "runtime/include"),
            ("generated-include-1", self.generated / "include"),
            ("raylib-include", self.root / "raylib"),
            ("raygui-include", self.root / "raygui"),
            ("caxecraft-native-include", ROOT / "examples/caxecraft/native/include"),
        ])
        self.assertEqual([(root.logical_name, root.path) for root in plan.dependency_roots], [
            ("generated-project", self.generated), ("raylib-include", self.root / "raylib"),
            ("raygui-include", self.root / "raygui"),
            ("caxecraft-native-include", ROOT / "examples/caxecraft/native/include"),
        ])
        self.assertEqual(plan.compile_flags, (
            "-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic-errors", "-Wshadow",
            "-Wconversion", "-Wsign-conversion", "-Wstrict-prototypes", "-Wmissing-prototypes",
            "-Wundef", "-Wformat=2", "-Wimplicit-fallthrough", "-Wcast-align", "-Wcast-qual",
            "-D_POSIX_C_SOURCE=200809L", "-O2",
        ))

    def test_observer_and_sanitizer_append_without_reordering(self) -> None:
        ordinary = self.plan(platform_name="macos", optimization="0")
        sanitizer = ("-fsanitize=address,undefined", "-fno-sanitize-recover=all", "-fno-omit-frame-pointer")
        observed = self.plan(platform_name="macos", optimization="0", native_sanitizer_flags=sanitizer,
                             observe_renderer_interval=True)
        observer = ROOT / "runtime/hxrt/test"
        self.assertEqual(observed.sources, ordinary.sources + (("terrain-observer/interval_observer.c", observer / "interval_observer.c"),))
        self.assertEqual(observed.include_roots, ordinary.include_roots + (play.IncludeRoot("terrain-observer", observer),))
        self.assertEqual(observed.dependency_roots, ordinary.dependency_roots + (play.DependencyRoot("terrain-observer", observer),))
        self.assertEqual(ordinary.compile_flags[-3:], ("-D_POSIX_C_SOURCE=200809L", "-D_DARWIN_C_SOURCE=1", "-O0"))
        self.assertEqual(observed.compile_flags, ordinary.compile_flags + sanitizer + ("-include", str(observer / "interval_observer_hooks.h")))
        self.assertNotIn("-include", ordinary.compile_flags)

    def test_missing_manifest_or_files_fail_before_compilation(self) -> None:
        invalid = [{}, {"build": {}}, {"build": {"sources": [], "includeDirectories": None}}]
        for key, value in (("sources", ["src/missing.c"]), ("includeDirectories", ["missing"]),
                           ("sources", ["../escape.c"])):
            manifest = copy.deepcopy(self.manifest)
            manifest["build"][key] = value
            invalid.append(manifest)
        for manifest in invalid:
            with self.subTest(manifest=manifest), self.assertRaises(play.PlayFailure):
                play.native_compile_plan(self.generated, manifest, **self.options)

    def test_native_consumer_uses_plan_and_preserves_link_order(self) -> None:
        plan = self.plan()
        with mock.patch.object(play, "native_compile_plan", return_value=plan) as planner, \
                mock.patch.object(play, "NativeCache") as cache_type, \
                mock.patch.object(play.provision, "load_lock", return_value={}), \
                mock.patch.object(play.provision, "link_facts", return_value=(["raylib"], [])):
            cache = cache_type.return_value
            cache.objects.return_value = ()
            cache.link.return_value = False
            play.compile_native(self.generated, self.manifest, output=self.root / "app",
                                library=self.root / "raylib.a", raygui_library=self.root / "raygui.a",
                                raylib_configuration="memory-software", cc="clang", cache_root=self.root / "cache",
                                cache_enabled=True, jobs=2, **self.options)
            planner.assert_called_once_with(self.generated, self.manifest, observe_renderer_interval=False, **self.options)
            self.assertEqual(cache_type.call_args.kwargs["compile_flags"], plan.compile_flags)
            self.assertEqual(cache_type.call_args.kwargs["include_roots"], plan.include_roots)
            self.assertEqual(cache_type.call_args.kwargs["dependency_roots"], plan.dependency_roots)
            cache.objects.assert_called_once_with(plan.sources)
            self.assertEqual(cache.link.call_args.kwargs["arguments_after_objects"],
                             [str(self.root / "raygui.a"), str(self.root / "raylib.a"), "-lm"])

    def test_coverage_uses_same_plan_with_native_preprocessor_and_clang_parser(self) -> None:
        args = SimpleNamespace(cc='gcc', optimization='2')
        with mock.patch('renderer_build_coverage.build_terrain_coverage', return_value='proof') as build, \
                mock.patch('dev_native_cache.verify_native_objects', return_value=mock.sentinel.pins) as verify, \
                mock.patch('scripts.test.compiler_discovery.resolve_compiler', return_value='/tools/clang'), \
                mock.patch.object(play.shutil, 'which', return_value='/tools/gcc'):
            result = play.terrain_build_coverage(
                args, generated=self.generated, manifest=self.manifest,
                executable=self.root / 'game', include_directory=self.root / 'raylib',
                raygui_include_directory=self.root / 'raygui', platform_name='linux',
                native_sanitizer_flags=(), native_contract=mock.sentinel.contract,
                native_result=play.NativeBuildResult((), 0, 0, False),
                output_root=self.root,
            )
        self.assertEqual(result, 'proof')
        plan = self.plan(observe_renderer_interval=True)
        passed = build.call_args.kwargs
        self.assertEqual(passed['sources'], plan.sources)
        self.assertEqual(passed['flags'], plan.compile_flags + tuple(
            item for root in plan.include_roots for item in ('-I', str(root.path))))
        self.assertEqual(passed['compiler'], Path('/tools/clang'))
        self.assertEqual(passed['preprocessor'], Path('/tools/gcc'))
        self.assertEqual(passed['native_build_pins'], mock.sentinel.pins)
        self.assertEqual(verify.call_args.kwargs['sources'], plan.sources)

    def test_prebuilt_legacy_recipe_preserves_original_bare_tool_arguments(self) -> None:
        args = SimpleNamespace(
            prebuilt_raylib_build=self.root / 'build', prebuilt_raylib_cache=self.root / 'cache',
            prebuilt_raylib_report=self.root / 'report.json', raylib_configuration='memory-software',
            cc='/current/clang', cxx='/current/clang++', cmake='/current/cmake', generator='Unix Makefiles',
        )
        report = {'outputs': {'normalizedCompileCommandsSha256': 'receipt'}, 'target': {
            'compiler': {'command': 'clang'}, 'cxxCompiler': {'command': 'clang++'},
            'cmake': {'command': 'cmake'}, 'buildTool': {'command': 'make'},
        }}
        with mock.patch.object(play, 'load_object', return_value={'provision': report}), \
                mock.patch('renderer_native_contract.verify_native_contract', return_value='contract') as verify:
            self.assertEqual(play.terrain_native_contract(
                args, platform_name='linux', source=self.root / 'source',
                include_directory=self.root / 'effective/src', library=self.root / 'library.a'), 'contract')
        passed = verify.call_args.kwargs
        replacements = passed['original_replacements']
        self.assertEqual(set(replacements.values()), {'${CACHE_ROOT}', '${RAYLIB_SOURCE}', '${RAYLIB_BUILD}'})
        self.assertNotIn('/current/clang', replacements)
        self.assertEqual(passed['authority'], 'pinned-source')


if __name__ == "__main__":
    unittest.main()
