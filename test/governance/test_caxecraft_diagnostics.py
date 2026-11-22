"""Prove occurrence-specific attribute admission without compiler subprocesses."""
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'examples/caxecraft'))
import renderer_diagnostics as diagnostics


class DiagnosticAdmissionTests(unittest.TestCase):
    def record(self, source, entry):
        first = source.index(entry)
        return {'compilerVersion': 'clang version 21.1.8', 'libraryVersion': 'clang version 21.1.8', 'diagnostics': [
            {'severity': 2, 'option': '-Wunknown-attributes', 'offset': first,
             'ranges': [[first, first + len(entry) - 1]], 'children': []}]}

    def test_literal_forms_and_byte_locations(self):
        entry = b'__access__(\n __read_only__, 2, 3)'
        source = '/* café */\n'.encode() + b'__attribute__((' + entry + b'))'
        result = diagnostics.admit_diagnostics(self.record(source, entry), source)
        self.assertEqual(result[0]['start'], source.index(entry))
        self.assertEqual(result[0]['attribute'], '__access__(__read_only__,2,3)')

    def test_namespace_expression_suffix_and_invalid_modes_are_closed(self):
        for entry in [b'access(read_only, extent())', b'access(read_only, 1U)',
                      b'access(read_ only, 1)', b'access(read_only, 1 2)',
                      b'access(read_only, 0)', b'access(other, 1)', b'access(read_only, 01)']:
            source = b'__attribute__((' + entry + b'))'
            with self.subTest(entry=entry), self.assertRaises(ValueError):
                diagnostics.admit_diagnostics(self.record(source, entry), source)
        entry = b'access(read_only,1)'
        source = b'[[vendor::' + entry + b']]'
        with self.assertRaises(ValueError):
            diagnostics.admit_diagnostics(self.record(source, entry), source)

    def test_incomplete_gnu_outer_list_is_rejected(self):
        entry = b'access(read_only,1)'
        for tail in [b'', b')', b'), other']:
            source = b'__attribute__((' + entry + tail
            with self.subTest(tail=tail), self.assertRaises(ValueError):
                diagnostics.admit_diagnostics(self.record(source, entry), source)

    def test_unknown_sibling_duplicate_or_partial_ranges_fail(self):
        entry = b'access(read_only,1)'
        source = b'__attribute__((' + entry + b', dangerous(1)))'
        record = self.record(source, entry)
        unknown = self.record(source, b'dangerous(1)')['diagnostics'][0]
        record['diagnostics'].append(unknown)
        with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)
        record = self.record(source, entry)
        record['diagnostics'] *= 2
        with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)
        record = self.record(source, entry)
        record['diagnostics'][0]['ranges'][0][1] -= 1
        with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)

    def test_context_notes_are_bounded_to_admitted_parents(self):
        import copy
        entry = b'access(read_only,1)'
        source = b'__attribute__((' + entry + b'))'
        note = {'severity': 1, 'option': '', 'category': 0,
                'location': {'offset': 0, 'line': 1, 'column': 1},
                'ranges': [], 'fixits': 0, 'children': []}
        record = self.record(source, entry)
        record['diagnostics'][0]['children'] = [note]
        self.assertEqual(diagnostics.admit_diagnostics(record, source)[0]['contextNotes'], [note])
        for key, value in [('severity', 2), ('severity', 3), ('option', '-Wother'),
                           ('category', 1), ('ranges', [[0, 1]]), ('fixits', 1), ('children', [note])]:
            changed = copy.deepcopy(record)
            changed['diagnostics'][0]['children'][0][key] = value
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                diagnostics.admit_diagnostics(changed, source)
        record = self.record(source, entry)
        record['diagnostics'][0]['severity'] = 1
        with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)

    def test_strings_comments_and_all_other_diagnostics_fail(self):
        entry = b'access(read_only,1)'
        for source in [b'"__attribute__((' + entry + b'))"', b'/* __attribute__((' + entry + b')) */']:
            with self.assertRaises(ValueError): diagnostics.admit_diagnostics(self.record(source, entry), source)
        source = b'__attribute__((' + entry + b'))'
        for key, value in [('severity', 3), ('option', '-Wignored-attributes'), ('children', 1)]:
            record = self.record(source, entry); record['diagnostics'][0][key] = value
            with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)
        record = self.record(source, entry); record['libraryVersion'] = 'different'
        with self.assertRaises(ValueError): diagnostics.admit_diagnostics(record, source)
