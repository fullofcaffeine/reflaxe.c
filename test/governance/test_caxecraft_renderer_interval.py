"""Accept complete measured evidence while retaining warmup and exact counters."""
from __future__ import annotations

import copy
import json
import sys
import tempfile
from unittest import mock
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / 'examples/caxecraft'))
from renderer_interval import IntervalEvidenceFailure, parse_interval_evidence

PREFIX = 'CAXECRAFT_TERRAIN_INTERVAL='
WARMUP = {
    'schemaVersion': 1, 'phase': 'warmup', 'valid': True, 'frames': 2,
    'portalFrames': 1, 'completed': 2, 'allocationAttempts': 3,
    'reallocationAttempts': 1, 'collectionAttempts': 2,
    'rootPushAttempts': 4294967297, 'rootPopAttempts': 4294967296,
}
COMPLETE = {
    'schemaVersion': 1, 'phase': 'complete', 'valid': True, 'frames': 5,
    'portalFrames': 3, 'completed': 5, 'allocationAttempts': 3,
    'reallocationAttempts': 1, 'collectionAttempts': 2,
    'rootPushAttempts': 4294967314, 'rootPopAttempts': 4294967312,
}


def transcript(*records: dict[str, object]) -> str:
    return 'ordinary game log\n' + '\n'.join(PREFIX + json.dumps(row) for row in records)


class RendererIntervalTests(unittest.TestCase):
    def test_exact_difference_preserves_warmup_and_attempt_meanings(self) -> None:
        result = parse_interval_evidence(transcript(WARMUP, COMPLETE), measured_frames=3)
        self.assertEqual(result['warmup'], WARMUP)
        self.assertEqual(result['total'], COMPLETE)
        self.assertEqual(result['measured'], {
            'frames': 3, 'portalFrames': 2, 'completed': 3,
            'allocationAttempts': 0, 'reallocationAttempts': 0,
            'collectionAttempts': 0, 'rootPushAttempts': 17, 'rootPopAttempts': 16,
        })

    def test_missing_duplicate_or_reordered_records_fail(self) -> None:
        for rows in [(), (WARMUP,), (COMPLETE,), (COMPLETE, WARMUP), (WARMUP, WARMUP, COMPLETE)]:
            with self.subTest(rows=rows), self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(transcript(*rows), measured_frames=3)

    def test_forbidden_runtime_attempts_in_measured_interval_fail(self) -> None:
        for key in ['allocationAttempts', 'reallocationAttempts', 'collectionAttempts']:
            changed = copy.deepcopy(COMPLETE)
            changed[key] += 1
            with self.subTest(key=key), self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(transcript(WARMUP, changed), measured_frames=3)

    def test_bad_protocol_counts_and_lossy_values_fail(self) -> None:
        changes = [('valid', False), ('schemaVersion', True), ('frames', True),
                   ('completed', 4), ('portalFrames', 6), ('portalFrames', 0),
                   ('rootPushAttempts', 1), ('rootPushAttempts', 1.0),
                   ('rootPushAttempts', -1), ('rootPushAttempts', 2**64),
                   ('extra', 0)]
        for key, value in changes:
            changed = copy.deepcopy(COMPLETE)
            changed[key] = value
            with self.subTest(key=key, value=value), self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(transcript(WARMUP, changed), measured_frames=3)

    def test_bad_warmup_and_frame_coverage_fail(self) -> None:
        bad = dict(WARMUP, frames=1, completed=1)
        with self.assertRaises(IntervalEvidenceFailure):
            parse_interval_evidence(transcript(bad, COMPLETE), measured_frames=4)
        for expected in [0, 2, 4, True]:
            with self.subTest(expected=expected), self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(transcript(WARMUP, COMPLETE), measured_frames=expected)

    def test_portal_proof_requires_activity_after_warmup(self) -> None:
        for warmup_portals in (0, 1):
            warmup = dict(WARMUP, portalFrames=warmup_portals)
            complete = dict(COMPLETE, portalFrames=warmup_portals)
            text = transcript(warmup, complete)
            self.assertEqual(parse_interval_evidence(text, measured_frames=3)['measured']['portalFrames'], 0)
            with self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(text, measured_frames=3, require_portal=True)
        parse_interval_evidence(transcript(WARMUP, COMPLETE), measured_frames=3, require_portal=True)

    def test_instrumented_samples_cannot_become_timing_evidence(self) -> None:
        from benchmark_renderer import BenchmarkFailure, build_comparison
        for kind in ('instrumented-correctness', None):
            report = {'benchmark': {'measurementKind': kind}}
            with self.assertRaisesRegex(BenchmarkFailure, 'uninstrumented timing'):
                build_comparison(report, report, Path('.'), Path('.'), {})

    def test_native_hooks_and_observer_source_only_enter_correctness_build(self) -> None:
        import play
        class PlanCaptured(Exception):
            pass
        with tempfile.TemporaryDirectory() as raw:
            root = Path(raw)
            (root / 'include').mkdir()
            (root / 'program.c').write_text('int main(void) { return 0; }\n')
            manifest = {'build': {'sources': ['program.c'], 'includeDirectories': ['include']}}
            for observed in (False, True):
                with self.subTest(observed=observed), mock.patch.object(play, 'NativeCache') as cache:
                    cache.return_value.objects.side_effect = PlanCaptured
                    with self.assertRaises(PlanCaptured):
                        play.compile_native(
                            root, manifest, output=root / 'game', include_directory=root,
                            library=root / 'raylib.a', raygui_include_directory=root,
                            raygui_library=root / 'raygui.a', platform_name='macos',
                            raylib_configuration='memory-software', cc='clang', optimization='2',
                            native_sanitizer_flags=(), cache_root=root / 'cache',
                            cache_enabled=True, jobs=1, observe_renderer_interval=observed,
                        )
                    flags = cache.call_args.kwargs['compile_flags']
                    sources = cache.return_value.objects.call_args.args[0]
                    self.assertEqual('-include' in flags, observed)
                    self.assertEqual(any(path.name == 'interval_observer.c' for _, path in sources), observed)
                    if observed:
                        self.assertEqual(Path(flags[-1]).name, 'interval_observer_hooks.h')
                    self.assertEqual(sources[0], ('program.c', root / 'program.c'))

    def test_haxe_observer_define_only_enters_correctness_request(self) -> None:
        import play
        class RequestCaptured(Exception):
            pass
        for observed in (False, True):
            with self.subTest(observed=observed), mock.patch.object(play, 'run', side_effect=RequestCaptured) as run:
                with self.assertRaises(RequestCaptured):
                    play.compile_haxe(
                        Path('/unused-generated'), layout='split', platform_name='macos',
                        raylib_configuration='memory-software', pilot='move-jump-edit',
                        benchmark_renderer=True, observe_renderer_interval=observed,
                    )
                self.assertEqual('caxecraft_observe_terrain' in run.call_args.args[0], observed)

    def test_external_frame_expectation_must_be_explicit(self) -> None:
        with self.assertRaises(TypeError):
            parse_interval_evidence(transcript(WARMUP, COMPLETE))

    def test_journey_derives_positive_terrain_count_without_timing(self) -> None:
        result = parse_interval_evidence(transcript(WARMUP, COMPLETE), measured_frames=None,
                                         require_portal=True, timing_enabled=False)
        self.assertEqual(result['measured']['frames'], 3)
        self.assertEqual(result['measured']['completed'], 3)
        self.assertEqual(result['coverage'], 'terrain draw arguments and rendering; no timing claim')
        for complete in (dict(WARMUP, phase='complete'),
                         dict(COMPLETE, completed=4),
                         dict(COMPLETE, portalFrames=WARMUP['portalFrames'])):
            with self.subTest(complete=complete), self.assertRaises(IntervalEvidenceFailure):
                parse_interval_evidence(transcript(WARMUP, complete), measured_frames=None,
                                        require_portal=True, timing_enabled=False)

    def test_observer_admits_only_bounded_journey_or_fixed_benchmark(self) -> None:
        import play
        for pilot, benchmark, backend, agent, accepted in [
            ('adventure-journey', False, 'memory-software', False, True),
            ('move-jump-edit', True, 'memory-software', False, True),
            ('adventure-journey', True, 'memory-software', False, False),
            ('move-jump-edit', False, 'memory-software', False, False),
            ('adventure-journey', False, 'desktop', False, False),
            ('adventure-journey', False, 'memory-software', True, False),
            (None, False, 'memory-software', False, False),
        ]:
            with self.subTest(pilot=pilot, benchmark=benchmark, backend=backend, agent=agent):
                arguments = dict(pilot=pilot, benchmark_renderer=benchmark,
                                 raylib_configuration=backend, agent_session=agent)
                if accepted:
                    play.validate_terrain_observation(**arguments)
                else:
                    with self.assertRaises(play.PlayFailure):
                        play.validate_terrain_observation(**arguments)

    def test_journey_compile_request_observes_without_enabling_timers(self) -> None:
        import play
        class RequestCaptured(Exception):
            pass
        with mock.patch.object(play, 'run', side_effect=RequestCaptured) as run:
            with self.assertRaises(RequestCaptured):
                play.compile_haxe(Path('/unused-generated'), layout='split', platform_name='macos',
                                  raylib_configuration='memory-software', pilot='adventure-journey',
                                  observe_renderer_interval=True)
            command = run.call_args.args[0]
            self.assertIn('caxecraft_observe_terrain', command)
            self.assertNotIn('caxecraft_render_benchmark', command)

    def test_duplicate_json_keys_fail(self) -> None:
        text = transcript(WARMUP, COMPLETE).replace('"valid": true', '"valid": false, "valid": true')
        with self.assertRaises(IntervalEvidenceFailure):
            parse_interval_evidence(text, measured_frames=3)


if __name__ == '__main__':
    unittest.main()
