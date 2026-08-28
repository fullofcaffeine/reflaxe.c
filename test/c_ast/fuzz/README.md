# C AST printer fuzz corpus

This corpus checks that structured C data cannot break the generated C file.
It uses the real `CASTPrinter` and the normal Reflaxe output owner.

`seeds.tsv` owns the stable numeric seeds. `dictionary.tsv` owns text that is
difficult to print safely, such as quotes, comment markers, newlines, Unicode,
and text that looks like a C directive. Keep both files small and reviewable.

Run `npm run test:c-ast-fuzz`. The runner applies these limits:

- at most 16 seeds and 64 cases for each seed;
- at most 512 total generated cases;
- at most 1 MiB of generated C;
- 120 seconds for Haxe and 30 seconds for each native process;
- no Haxe compiler server and no network access.

The generator is prefix-stable. Case 1 through case N do not change when more
cases are added. If a seed fails, the runner finds the smallest failing prefix
and prints a JSON record. Add that record to `regressions/`, add the seed to
`seeds.tsv` if it is new, and keep the test after the defect is fixed.

`regressions/minimizer.json` is a synthetic failure. It checks the minimizer
without requiring a real printer defect to remain unfixed.
