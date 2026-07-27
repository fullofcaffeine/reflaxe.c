#!/usr/bin/env python3
"""Focused dependency and publication tests for Caxecraft's native cache."""

from __future__ import annotations

import os
import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CASE))

from dev_native_cache import (  # noqa: E402
    DependencyRoot,
    IncludeRoot,
    NativeCache,
    NativeCacheFailure,
)


class CaxecraftNativeCacheTests(unittest.TestCase):
    """Prove exact depfile hits/misses, corruption recovery, and link reuse."""

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-native-cache-")
        self.root = Path(self.temporary.name)
        self.generated = self.root / "generated"
        self.include = self.generated / "include"
        self.source = self.generated / "src"
        self.shadow = self.root / "shadow"
        self.cache_root = self.root / "cache"
        self.output = self.root / "bin/program"
        self.include.mkdir(parents=True)
        self.source.mkdir()
        self.shadow.mkdir()
        (self.include / "shared.h").write_text(
            "#ifndef SHARED_H\n#define SHARED_H\nint a(void); int b(void);\n#endif\n",
            encoding="utf-8",
        )
        (self.include / "private_a.h").write_text(
            "#define A_VALUE 1\n", encoding="utf-8"
        )
        (self.source / "main.c").write_text(
            '#include "shared.h"\nint main(void) { return a() + b() == 3 ? 0 : 1; }\n',
            encoding="utf-8",
        )
        (self.source / "a.c").write_text(
            '#include "shared.h"\n#include "private_a.h"\nint a(void) { return A_VALUE; }\n',
            encoding="utf-8",
        )
        (self.source / "b.c").write_text(
            '#include "shared.h"\nint b(void) { return 2; }\n',
            encoding="utf-8",
        )
        self.compiler = shutil.which("clang") or shutil.which("gcc")
        if self.compiler is None:
            self.skipTest("a C compiler is required")

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def native_cache(
        self,
        *,
        flags: tuple[str, ...] = ("-std=c11", "-Wall", "-Werror"),
        enabled: bool = True,
    ) -> NativeCache:
        return NativeCache(
            self.cache_root,
            compiler=str(self.compiler),
            compile_flags=flags,
            include_roots=(
                IncludeRoot("shadow", self.shadow),
                IncludeRoot("generated-include", self.include),
            ),
            dependency_roots=(DependencyRoot("generated", self.generated),),
            jobs=3,
            enabled=enabled,
        )

    def sources(self):
        return tuple(
            (f"src/{name}", self.source / name)
            for name in ("main.c", "a.c", "b.c")
        )

    def build(self, cache: NativeCache | None = None, *, link_flag: str | None = None):
        selected = cache or self.native_cache()
        objects = selected.objects(self.sources())
        link_arguments = () if link_flag is None else (link_flag,)
        link_hit = selected.link(
            objects,
            output=self.output,
            arguments_after_objects=link_arguments,
            libraries=(),
        )
        return objects, link_hit

    def test_exact_second_build_hits_every_object_and_link(self) -> None:
        first, first_link = self.build()
        second, second_link = self.build()

        self.assertFalse(first_link)
        self.assertTrue(all(not item.cache_hit for item in first))
        self.assertTrue(second_link)
        self.assertTrue(all(item.cache_hit for item in second))
        result = os.spawnv(os.P_WAIT, str(self.output), [str(self.output)])
        self.assertEqual(result, 0)

    def test_source_and_private_header_changes_invalidate_only_dependents(self) -> None:
        self.build()
        (self.source / "a.c").write_text(
            '#include "shared.h"\n#include "private_a.h"\nint a(void) { return A_VALUE + 0; }\n',
            encoding="utf-8",
        )
        source_edit, link_hit = self.build()
        self.assertEqual(
            [item.source for item in source_edit if not item.cache_hit],
            ["src/a.c"],
        )
        self.assertFalse(link_hit)

        (self.include / "private_a.h").write_text(
            "#define A_VALUE (1)\n", encoding="utf-8"
        )
        header_edit, _ = self.build()
        self.assertEqual(
            [item.source for item in header_edit if not item.cache_hit],
            ["src/a.c"],
        )

    def test_public_header_and_new_shadowing_path_fail_closed(self) -> None:
        self.build()
        (self.include / "shared.h").write_text(
            "#ifndef SHARED_H\n#define SHARED_H\nint b(void); int a(void);\n#endif\n",
            encoding="utf-8",
        )
        public_edit, _ = self.build()
        self.assertTrue(all(not item.cache_hit for item in public_edit))

        (self.shadow / "shared.h").write_text(
            "#ifndef SHARED_H\n#define SHARED_H\nint a(void); int b(void);\n#endif\n",
            encoding="utf-8",
        )
        shadow_edit, _ = self.build()
        self.assertTrue(all(not item.cache_hit for item in shadow_edit))

    def test_flag_and_link_only_changes_have_separate_owners(self) -> None:
        first, _ = self.build()
        changed_flags, _ = self.build(
            self.native_cache(
                flags=("-std=c11", "-Wall", "-Werror", "-fno-common")
            )
        )
        self.assertTrue(all(not item.cache_hit for item in changed_flags))

        same_flags = self.native_cache(
            flags=("-std=c11", "-Wall", "-Werror", "-fno-common")
        )
        objects = same_flags.objects(self.sources())
        self.assertTrue(all(item.cache_hit for item in objects))
        link_hit = same_flags.link(
            objects,
            output=self.output,
            arguments_after_objects=("-Wl,-dead_strip",)
            if sys.platform == "darwin"
            else ("-Wl,--gc-sections",),
            libraries=(),
        )
        self.assertFalse(link_hit)
        self.assertNotEqual(
            [item.key for item in first],
            [item.key for item in changed_flags],
        )

    def test_corrupt_object_and_missing_index_are_misses(self) -> None:
        first, _ = self.build()
        first[1].path.write_bytes(b"corrupt")
        repaired = self.native_cache().objects(self.sources())
        self.assertFalse(repaired[1].cache_hit)
        self.assertTrue(repaired[0].cache_hit)
        self.assertTrue(repaired[2].cache_hit)

        indexes = sorted((self.cache_root / "object-indexes").glob("*.json"))
        indexes[0].unlink()
        missed = self.native_cache().objects(self.sources())
        self.assertEqual(sum(not item.cache_hit for item in missed), 1)

    def test_malformed_dependency_metadata_and_link_corruption_are_misses(self) -> None:
        first, _ = self.build()
        metadata_path = first[0].path.parent / "metadata.json"
        metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
        metadata["dependencies"] = "malformed"
        from dev_build_state import canonical_digest

        body = {key: value for key, value in metadata.items() if key != "sha256"}
        metadata["sha256"] = canonical_digest(body)
        metadata_path.write_text(json.dumps(metadata), encoding="utf-8")

        dependency_miss = self.native_cache().objects(self.sources())
        self.assertFalse(dependency_miss[0].cache_hit)

        self.native_cache().link(
            dependency_miss,
            output=self.output,
            arguments_after_objects=(),
            libraries=(),
        )
        cached_executables = list((self.cache_root / "links").glob("*/caxecraft"))
        self.assertTrue(cached_executables)
        cached_executables[-1].write_bytes(b"corrupt")
        link_hit = self.native_cache().link(
            dependency_miss,
            output=self.output,
            arguments_after_objects=(),
            libraries=(),
        )
        self.assertFalse(link_hit)

    def test_failed_compile_never_publishes_an_index(self) -> None:
        (self.source / "a.c").write_text("this is not C\n", encoding="utf-8")
        with self.assertRaisesRegex(NativeCacheFailure, "native compile"):
            self.native_cache().objects(self.sources())
        published_sources = []
        for metadata_path in (self.cache_root / "objects").glob("*/metadata.json"):
            metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
            published_sources.append(metadata.get("base", {}).get("source"))
        self.assertNotIn("src/a.c", published_sources)

    def test_cache_off_publishes_no_object_or_link_entries(self) -> None:
        cache = self.native_cache(enabled=False)
        objects, link_hit = self.build(cache)
        self.assertFalse(link_hit)
        self.assertTrue(all(not item.cache_hit for item in objects))
        self.assertEqual(list((self.cache_root / "objects").iterdir()), [])
        self.assertEqual(list((self.cache_root / "links").iterdir()), [])


if __name__ == "__main__":
    unittest.main()
