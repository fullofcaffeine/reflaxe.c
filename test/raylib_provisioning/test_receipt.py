"""Stale-cache controls for trusted Raylib source-build receipts; no native jobs."""
from __future__ import annotations

import copy
import hashlib
import json
import sys
from unittest import mock
import tempfile
import unittest
from pathlib import Path

from scripts.raylib.provision import (
    ProvisionFailure, TreeIdentity, configuration_definitions, load_lock,
    patch_lock_identity, verify_source_build_receipt, verify_compile_commands_receipt,
)


class CompileCommandReceiptTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory(prefix='raylib-recipe-')
        self.addCleanup(temporary.cleanup)
        self.path = Path(temporary.name) / 'compile_commands.json'
        self.raw = b'[{"command":"/usr/bin/clang -O3 -c /original/src/rcore.c"}]\n'
        self.path.write_bytes(self.raw)
        self.digest = hashlib.sha256(self.raw).hexdigest()

    def verify(self, outputs, replacements=None):
        return verify_compile_commands_receipt(
            report={'outputs': outputs}, compile_commands=self.path,
            original_replacements=replacements,
        )

    def test_raw_receipt_and_changed_flags_or_source_fail(self) -> None:
        outputs = {'compileCommandsSha256': self.digest}
        self.assertEqual(self.verify(outputs), self.digest)
        for original, changed in ((b'-O3', b'-O0'), (b'rcore.c', b'other.c')):
            self.path.write_bytes(self.raw.replace(original, changed))
            with self.subTest(changed=changed), self.assertRaises(ProvisionFailure):
                self.verify(outputs)

    def test_legacy_requires_exact_original_substitutions(self) -> None:
        # Independent expected bytes: a bare original compiler argument did not
        # replace CMake's later resolved compiler path.
        expected = b'[{"command":"/usr/bin/clang -O3 -c ${SOURCE}/src/rcore.c"}]\n'
        outputs = {'normalizedCompileCommandsSha256': hashlib.sha256(expected).hexdigest()}
        original = {'/original': '${SOURCE}'}
        self.assertEqual(self.verify(outputs, original), self.digest)
        for replacements in (None, {}, {**original, '/usr/bin/clang': '${C_COMPILER}'}):
            with self.subTest(replacements=replacements), self.assertRaises(ProvisionFailure):
                self.verify(outputs, replacements)
        # A raw mismatch must never fall back to an otherwise matching legacy hash.
        outputs['compileCommandsSha256'] = '0' * 64
        with self.assertRaises(ProvisionFailure):
            self.verify(outputs, original)

    def test_missing_digest_file_and_symlink_fail(self) -> None:
        with self.assertRaises(ProvisionFailure):
            self.verify({}, {})
        self.path.unlink()
        with self.assertRaises(ProvisionFailure):
            self.verify({'compileCommandsSha256': self.digest})
        target = self.path.with_name('target.json')
        target.write_bytes(self.raw)
        self.path.symlink_to(target)
        with self.assertRaises(ProvisionFailure):
            self.verify({'compileCommandsSha256': self.digest})


class SourceBuildReceiptTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="raylib-receipt-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.source = self.root / "effective"
        (self.source / "src").mkdir(parents=True)
        self.file = self.source / "src/example.c"
        self.file.write_bytes(b"effective\n")
        self.library = self.root / "libraylib.a"
        self.library.write_bytes(b"archive")
        self.lock = load_lock()
        upstream = self.lock['upstream']
        # Independent one-file vector for hxc-path-size-content-sha256-v1.
        # The helper must inspect actual bytes; no mocked tree/hash routine.
        self.tree = {
            'algorithm': 'hxc-path-size-content-sha256-v1',
            'sha256': "d128c008d19a23686ffa59b4ca55b2c392f341ff08fecb544f0a1858791e8b4f",
            'fileCount': 1, 'sizeBytes': 10,
        }
        self.report = {
            'schemaVersion': 1, 'authority': 'pinned-source',
            'target': {'platform': 'macos'},
            'configuration': {'id': 'memory-software', 'cmakeDefinitions':
                list(configuration_definitions(self.lock, 'macos', 'memory-software'))},
            'upstream': {key: upstream[key] for key in
                ('name', 'release', 'packageVersion', 'apiVersion', 'commit', 'sourceTree')},
            'patches': patch_lock_identity(self.lock, 'macos', 'memory-software'),
            'effectiveSourceTree': dict(self.tree),
            'claims': {'archiveVerified': True, 'sourceTreeVerified': True, 'raylibBuilt': True},
            'commands': {'configure': {'exitCode': 0}, 'build': {'exitCode': 0}},
            'outputs': {'library': {'sha256': hashlib.sha256(b'archive').hexdigest(), 'sizeBytes': 7}},
        }
        self.report['upstream']['archiveSha256'] = upstream['archive']['sha256']

    def verify(self, report=None, authority='pinned-source') -> TreeIdentity:
        return verify_source_build_receipt(
            lock=self.lock, report=self.report if report is None else report,
            authority=authority, platform_name='macos', configuration='memory-software',
            effective_source_root=self.source, library_file=self.library,
        )

    def test_unchanged_trusted_receipt_and_explicit_offline_authority(self) -> None:
        before = copy.deepcopy(self.report)
        self.assertEqual(self.verify(), TreeIdentity(self.tree['sha256'], 1, 10))
        self.assertEqual(self.report, before)
        offline = copy.deepcopy(self.report)
        offline['authority'] = 'offline-source'
        offline['claims']['archiveVerified'] = False
        self.assertEqual(self.verify(offline, 'offline-source').file_count, 1)
        with self.assertRaises(ProvisionFailure):
            self.verify(offline)

    def test_changed_added_deleted_and_linked_source_fail(self) -> None:
        self.file.write_bytes(b'EFFECTIVE\n')  # Same length, changed bytes.
        with self.assertRaises(ProvisionFailure): self.verify()
        self.file.write_bytes(b'effective\n')
        extra = self.source / 'added.c'
        extra.write_bytes(b'extra')
        with self.assertRaises(ProvisionFailure): self.verify()
        extra.unlink()
        self.file.unlink()
        with self.assertRaises(ProvisionFailure): self.verify()
        self.file.symlink_to(self.library)
        with self.assertRaisesRegex(ProvisionFailure, 'symlink'): self.verify()

    def test_source_root_and_library_symlinks_fail(self) -> None:
        alias = self.root / 'alias'
        alias.symlink_to(self.source, target_is_directory=True)
        self.source = alias
        with self.assertRaises(ProvisionFailure): self.verify()
        self.source = self.root / 'effective'
        alias.unlink()
        alias.symlink_to(self.library)
        self.library = alias
        with self.assertRaises(ProvisionFailure): self.verify()

    def test_library_changed_bytes_size_or_missing_fails(self) -> None:
        for data in (b'ARCHIVE', b'longer archive'):
            self.library.write_bytes(data)
            with self.subTest(data=data), self.assertRaises(ProvisionFailure): self.verify()
        self.library.unlink()
        with self.assertRaises(ProvisionFailure): self.verify()

    def test_receipt_identity_and_success_claim_drift_fail(self) -> None:
        changes = [
            (('schemaVersion',), True), (('schemaVersion',), 2),
            (('authority',), 'system-pkg-config'), (('target', 'platform'), 'linux'),
            (('configuration', 'id'), 'desktop'),
            (('configuration', 'cmakeDefinitions'), ['PLATFORM=Desktop']),
            (('upstream', 'commit'), 'a' * 40),
            (('upstream', 'archiveSha256'), 'a' * 64),
            (('upstream', 'sourceTree', 'sha256'), 'a' * 64),
            (('patches',), []), (('patches', 0, 'id'), 'different-patch'),
            (('patches', 0, 'sha256'), 'a' * 64),
            (('effectiveSourceTree', 'algorithm'), 'sha256'),
            (('effectiveSourceTree', 'sha256'), 'a' * 64),
            (('effectiveSourceTree', 'fileCount'), True),
            (('effectiveSourceTree', 'fileCount'), 2),
            (('effectiveSourceTree', 'sizeBytes'), 9),
            (('claims', 'sourceTreeVerified'), False), (('claims', 'raylibBuilt'), 1),
            (('claims', 'archiveVerified'), False),
            (('commands', 'configure', 'exitCode'), 1),
            (('commands', 'build', 'exitCode'), False),
            (('outputs', 'library', 'sha256'), 'a' * 64),
            (('outputs', 'library', 'sizeBytes'), 8),
        ]
        for path, value in changes:
            report = copy.deepcopy(self.report)
            owner = report
            for key in path[:-1]: owner = owner[key]
            owner[path[-1]] = value
            with self.subTest(path=path), self.assertRaises(ProvisionFailure):
                self.verify(report)
        for key in ('target', 'upstream', 'configuration', 'patches', 'claims',
                    'commands', 'effectiveSourceTree', 'outputs'):
            report = copy.deepcopy(self.report)
            del report[key]
            with self.subTest(missing=key), self.assertRaises(ProvisionFailure): self.verify(report)


    def test_both_play_reuse_paths_reject_current_source_drift(self) -> None:
        sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'examples/caxecraft'))
        import play
        cache = self.root / 'cache'
        build = cache / 'build' / 'fixture'
        build.mkdir(parents=True)
        effective = build / 'hxc-patched-source'
        self.source.rename(effective)
        self.source = effective
        self.file = effective / 'src/example.c'
        state = {
            'schemaVersion': 1, 'raylibCommit': play.provision.PINNED_COMMIT,
            'librarySha256': hashlib.sha256(self.library.read_bytes()).hexdigest(),
            'provisioningReport': self.report,
        }
        (build / 'hxc-caxecraft-raylib.json').write_text(json.dumps(state))
        report_path = self.root / 'integration.json'
        report_path.write_text(json.dumps({'provision': self.report}))
        with mock.patch.object(play, 'raylib_cache_key', return_value='fixture'), \
                mock.patch.object(play.provision, 'pinned_source', return_value=self.root / 'verified-upstream'), \
                mock.patch.object(play.provision, 'locate_raylib_library', return_value=self.library):
            def cached():
                return play.provision_raylib(
                    authority='pinned-source', platform_name='macos',
                    raylib_configuration='memory-software', source=None, cache_root=cache,
                    cc='clang', cxx='clang++', cmake='cmake', generator='Unix Makefiles',
                    allow_network=False, rebuild=False,
                )
            def prebuilt():
                return play.resolve_prebuilt_raylib(
                    cache_root=cache, build_root=build, report_path=report_path,
                    platform_name='macos', raylib_configuration='memory-software',
                )
            for reuse in (cached, prebuilt):
                actual_include, actual_library = reuse()
                self.assertEqual(actual_include.resolve(), (effective / 'src').resolve())
                self.assertEqual(actual_library.resolve(), self.library.resolve())
            self.file.write_bytes(b'changed native source\n')
            for reuse in (cached, prebuilt):
                with self.subTest(reuse=reuse.__name__), self.assertRaisesRegex(
                        play.provision.ProvisionFailure, 'effective source tree identity mismatch'):
                    reuse()


if __name__ == '__main__':
    unittest.main()
