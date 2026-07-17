# Seeded primitive differential suite

E2.T11 turns the fixed primitive examples into a reproducible generated-program
corpus. It broadens confidence in the already admitted primitive lowering; it
does not expand the set of supported Haxe syntax or claim general Haxe-to-C
compilation.

## Reproducible corpus

[`test/primitive_differential/seed.json`](../test/primitive_differential/seed.json)
is the reviewed input. The runner implements SplitMix64 directly, fixes the
algorithm version and 64-bit seed, and generates 128 cases. Every admitted
operation receives at least three cases before the remaining cases and their
order are selected. Integer generation is biased toward zero, signed and
unsigned limits, shift boundaries, and nearby values; the remaining values use
all 32 bits. Floating inputs are finite binary-exact quarter values whose
results stay inside the common `Std.int` range.

The generated corpus and ordinary Haxe fixture are centrally owned snapshots.
The runner regenerates both twice, executes the pinned Haxe Eval oracle twice,
and compiles the same source into two unrelated production output roots. A
change in the seed, generator, operation inventory, trace, or generated C is
therefore reviewable instead of silently becoming a new random run.

## Oracle and target refinements

The common domain compares exact line-oriented `Int`, `UInt`, and `Bool`
results. Float arithmetic is observed through finite in-range `Std.int` results
or Boolean comparisons so the suite needs no target-dependent decimal
formatting normalization. Division and modulo denominators are nonzero in this
domain, and `INT32_MIN / -1` is excluded because its subsequent `Std.int`
conversion crosses the common range.

Target-specific behavior is not hidden by filtering a failed comparison.
[`primitive-divergences.json`](specs/primitive-divergences.json) is a closed,
schema-backed ledger that records the exact input, typed Eval expectation, hxc
value, ADR contract, rationale, owner, and executable test for every admitted
divergence. The suite currently replays integer modulo by zero and the positive
infinity/overflow sides of `Std.int`. An unknown, duplicate, unowned, stale, or
unexecuted ledger entry fails the suite.

Modulo by zero retains an exact `nan` Eval expectation, and finite positive
overflow retains its exact wrapping observation. Positive infinity does not:
the [pinned Eval implementation](https://github.com/HaxeFoundation/haxe/blob/2c1e544e0a2c7524ef4c8e103f1b0580362ea538/src/macro/eval/evalStdLib.ml#L2206-L2208)
delegates that non-finite conversion to OCaml's host conversion, whose result is
host-dependent. That entry therefore uses the only admitted normalization,
`host-dependent-int32`. The runner still executes the raw Eval call, requires
the observation to be a canonical signed 32-bit integer different from the
exact hxc saturation value, and only then records the stable
`<host-dependent-int32>` snapshot token. Common-domain cases, the other two
divergences, and all generated-C target values remain exact. The expectation
kind and canonical token are closed by the schema and negative self-tests, so
this normalization cannot expand into a general mismatch filter.

## Mismatch reduction

Traces carry stable case IDs. On a semantic mismatch, the runner reduces the
128-case trace to the first independently replayable case and includes its
typed operation, arguments, oracle line, and target line as canonical JSON in
the failure. The checked-in
[`minimizer-regression.json`](../test/primitive_differential/regressions/minimizer-regression.json)
locks that reduction behavior using surrounding passing cases, so changes to
the reducer cannot make a reproducer disappear or select a different case.

## Native and sanitizer evidence

Production compilation must retain an empty runtime feature plan and contain
no `hxrt` include, source, library, or symbol. The checked-in private header and
C source are consumed without Haxe in the required Linux GCC and Clang jobs.
Each compiler runs the whole corpus at `-O0` and `-O2`, then at `-O1` with
combined AddressSanitizer and UndefinedBehaviorSanitizer when the compiler and
host pass a compile-and-execute probe. A requested CI compiler may not skip a
missing sanitizer.

Native compilation uses the reusable argument-array fixture harness. Its
report retains compiler family/version, normalized compile/link/run arguments,
input hashes, exit status, stdout, and stderr, while an explicit check rejects
temporary or checkout paths.

Run the suite and its snapshot check with:

```sh
npm run test:primitive-differential
npm run snapshots:check -- --suite primitive-differential
```

Intentional baseline changes use the central updater:

```sh
npm run snapshots:update -- --suite primitive-differential
```

The fixed boundary matrix in [UB-safe primitive arithmetic](arithmetic-semantics.md)
remains the focused contract for NaN, infinity, signed zero, exact helper shape,
and optimization. This generated corpus complements that matrix; neither suite
proves objects, allocation, exceptions, arbitrary standard-library calls, or a
stable public ABI.
