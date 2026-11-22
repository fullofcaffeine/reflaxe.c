"""Exercise the native observer with a tiny ABI stand-in, without compiling Haxe."""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from test_caxecraft_compiler_progress import CASE, ROOT, load_module


class NativeLoaderProgressTests(unittest.TestCase):
    def test_opt_in_progress_keeps_stdout_and_collector_call_order(self) -> None:
        compiler = shutil.which("cc")
        if compiler is None:
            self.skipTest("a C compiler is required for the native observer fixture")
        with mock.patch.object(sys, "path", [str(CASE), *sys.path]):
            runner = load_module("native_observer_fixture_runner", CASE / "run_haxe_c_test.py")
        fields = (
            "observed", "traceInputHash", "traceByteLength", "traceGenerationId",
            "traceWorldState", "traceAuthored", "traceActorMechanics", "traceAuthority",
        )
        prefix = "hxc_caxecraft_qa_RuntimeLevelLoaderProbe_"
        with tempfile.TemporaryDirectory(prefix="hxc-native-observer-") as directory:
            root = Path(directory)
            (root / "hxc").mkdir()
            (root / "hxc/program.h").write_text(
                '#include "hxrt/gc.h"\n'
                "extern hxc_gc hxc_program_gc;\n"
                "extern hxc_gc_thread hxc_program_gc_thread;\n"
                f"void {prefix}main(void);\n"
                + "".join(f"extern int32_t {prefix}{field};\n" for field in fields),
                encoding="utf-8",
            )
            stand_in = root / "stand_in.c"
            stand_in.write_text(
                '#include "hxc/program.h"\n'
                "hxc_gc hxc_program_gc = HXC_GC_INITIALIZER;\n"
                "hxc_gc_thread hxc_program_gc_thread = {0};\n"
                + "".join(
                    f"int32_t {prefix}{field} = {0 if index == 0 else index + 10};\n"
                    for index, field in enumerate(fields)
                )
                + r'''
static int stage = 0;
static hxc_status advance(int expected) {
    if (stage != expected) return HXC_STATUS_INVALID_ARGUMENT;
    ++stage;
    return HXC_STATUS_OK;
}
hxc_allocator hxc_default_allocator(void) {
    const hxc_allocator allocator = {NULL, NULL, NULL, NULL};
    return allocator;
}
hxc_status hxc_gc_init(const hxc_gc_config *config, hxc_gc *gc) {
    (void)config; (void)gc;
    return advance(0);
}
hxc_status hxc_gc_thread_register(hxc_gc *gc, hxc_gc_thread *thread) {
    (void)gc; (void)thread;
    return advance(1);
}
void hxc_caxecraft_qa_RuntimeLevelLoaderProbe_main(void) {
    if (advance(2) != HXC_STATUS_OK)
        hxc_caxecraft_qa_RuntimeLevelLoaderProbe_observed = 9;
}
hxc_status hxc_gc_collect(hxc_gc *gc) {
    (void)gc;
    return advance(3);
}
hxc_status hxc_gc_get_stats(const hxc_gc *gc, hxc_gc_stats *stats) {
    (void)gc;
    stats->allocation_count = 3U;
    stats->collection_count = 1U;
    stats->reclaimed_object_count = 3U;
    stats->peak_object_count = 2U;
    stats->current_object_count = 0U;
    return advance(4);
}
hxc_status hxc_gc_thread_unregister(hxc_gc_thread *thread) {
    (void)thread;
    return advance(5);
}
hxc_status hxc_gc_dispose(hxc_gc *gc) {
    (void)gc;
    return advance(6);
}
''', encoding="utf-8",
            )
            executable = root / "observer"
            subprocess.run(
                [compiler, *runner.STRICT_FLAGS, "-I", str(root),
                 "-I", str(ROOT / "runtime/hxrt/include"),
                 str(CASE / "test/native/runtime_level_loader_harness.c"),
                 str(stand_in), "-o", str(executable)],
                check=True, capture_output=True, text=True, timeout=15,
            )
            expected = "0\n11\n12\n13\n14\n15\n16\n17\n"
            environment = dict(os.environ)
            environment.pop("HXC_TEST_NATIVE_PROGRESS", None)
            for setting in (None, "0", "1"):
                selected = dict(environment)
                if setting is not None:
                    selected["HXC_TEST_NATIVE_PROGRESS"] = setting
                with self.subTest(setting=setting):
                    result = subprocess.run(
                        [str(executable)], env=selected, check=True,
                        capture_output=True, text=True, timeout=5,
                    )
                    self.assertEqual(result.stdout, expected)
                    runner.validate_oracle(runner.CASES["runtime-level-loader"], result.stdout)
                    if setting == "1":
                        self.assertEqual(result.stderr.splitlines(), [
                            "native test phase: initialization",
                            "native test phase: probe main",
                            "native test phase: final collection",
                            "native test phase: collector statistics",
                            "native test phase: teardown",
                            "native test phase: complete",
                        ])
                    else:
                        self.assertEqual(result.stderr, "")


if __name__ == "__main__":
    unittest.main()
