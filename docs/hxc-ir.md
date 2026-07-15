# HxcIR semantic contract

`HxcIR` is the target-owned semantic layer between normalized Haxe input and
the structural C AST. Its schema is internal to the compiler: schema version 2
is deterministic and validation-backed, but it is not a public file format or
ABI promise. E2.T02 connects real primitive bodies to this layer; E2.T03 adds
typed parameters, ordered direct calls, explicit argument conversions, and a
narrow production C consumer. E2.T04 adds primitive global places plus explicit
branch/jump graphs for lazy and expression-valued control flow. All other
frontend and C lowering remains explicitly gated.

The IR exists because C syntax cannot safely carry several Haxe decisions by
itself. It records evaluation order, immutable values, mutable places,
initialization and lifetime transitions, call dispatch, conversion strategy,
allocation intent, failure successors, and cleanup execution before a C
expression or statement is selected.

## Structural model

A program contains modules. Modules own semantic type declarations, concrete
type instances, global/static storage, and functions. Functions contain typed
parameters, addressable locals, cleanup regions, and basic blocks. A block owns
an ordered instruction array and exactly one terminator.

An `HxcIRTypeRef` describes Haxe-level semantic types and selected type
instances, not C declarator spelling. `IRTDynamic` is the explicit Haxe
`Dynamic` type; it must never be used as a placeholder for an unsupported typed
AST node. Exact-width integers retain width and signedness, while `size_t`,
`ptrdiff_t`, `intptr_t`, and `uintptr_t` retain distinct unresolved target-ABI
identities. Nullability records either a tagged scalar payload or a pointer
representation rather than being inferred from the eventual C spelling.

Calls have one of seven exhaustive dispatch forms: direct, virtual, interface,
closure, native, runtime, or intrinsic. Direct and native calls therefore
cannot be confused with runtime fallback, and later devirtualization does not
need to recover intent from target syntax.

## Required invariants

`HxcIRValidator` rejects a program unless all of these conditions hold:

- program schema, module/type/instance/global/function IDs, function-local
  value/place/block/instruction/cleanup IDs, and every reference are valid and
  unique in their declared namespace;
- source spans use normalized repository-relative `/` paths, one-based
  line/column coordinates, and an end point that does not precede the start;
  invalid absolute spellings are redacted from diagnostics;
- immutable values and mutable addressable places remain different types;
  loads, stores, addresses, dereferences, fields, and indices cannot silently
  change value category;
- instruction array order is semantic order. An instruction can use only a
  function parameter, a parameter of its own block, or an earlier result in
  that block;
- values never leak directly between blocks. A control or failure edge passes
  them through typed target-block parameters, with arity and type validation;
- direct and closure calls match known parameter and return signatures;
- every value-producing instruction has exactly one typed result, every
  effect-only instruction has none, every block terminates, and every branch,
  switch, failure, and `finally` target exists;
- constant families match their result types; for structurally resolvable
  places, loads match the place type, initialization/store values match their
  destination, and address results are pointers to the addressed type;
- return terminators carry no value for `Void`, carry exactly one value for a
  non-`Void` function, and match the declared return type;
- calls, conversions, allocation, deallocation, retain, and trace operations
  identify their implementation as static/direct, program-local specialized,
  or a named runtime feature;
- primitive conversions identify exact, wrapping, checked, saturating,
  nullable-inject, or nullable-unwrap intent. Checked numeric and nullable
  unwrap forms require an explicit failure edge; the remaining primitive forms
  reject one;
- exact, wrapping, checked, saturating, and nullable primitive operations may
  use only direct/static or program-local implementation. A named runtime
  feature is an internal invariant failure, not a fallback;
- local entry states and every explicit lifetime transition are legal. An
  initialization ends in `initialized`; a destroy cleanup ends in `destroyed`;
- cleanup actions are listed in registration order. An edge lists actual
  execution order: reverse registration order within a region, then the direct
  parent region moving outward. Steps are unique and refer to stable
  local/global places;
- call/allocation failure edges and throw terminators identify exception,
  result-error, allocation-failure, or native-status behavior explicitly. The
  normal continuation of a successful instruction is the next instruction (or
  its block terminator); the non-normal successor is never implicit.

These checks make a malformed internal IR an `HXC9000` compiler invariant
failure. They do not replace later semantic passes such as representation
selection, ownership proofs, runtime-feature finalization, exception-strategy
selection, or C undefined-behavior checks.

## Runtime and profile policy

The IR has no unconditional runtime concept. `IRIStatic` is a direct semantic
operation, `IRIProgramLocal` names a compilation-local specialized helper, and
`IRIRuntime` names the exact requested feature. Runtime call dispatch likewise
records a feature and operation. Merely constructing a type, calling a direct
function, or using cleanup selects nothing from `hxrt`.

Portable and metal share this IR. Later analyses decide whether a representation
is legal under the resolved profile and runtime policy. The IR must preserve the
reason and source span needed for those decisions; it must not preselect a broad
runtime slice to make validation pass.

## Unsupported typed AST nodes

The body frontend calls
`HxcIRDiagnostic.unsupportedTypedAstNode(profile, nodeKind, context, span)` and
stops at the first unsupported typed node with stable diagnostic `HXC1001`. It
must not insert a `Dynamic`, null, raw C string, or invented constant in place
of an unsupported node. A fully admitted primitive static graph reaches
validated HxcIR, structural C, and an owned runtime-free executable project;
unimplemented signatures or expressions still stop without output. See
[primitive function-body lowering](body-lowering.md) and [static function
lowering](function-lowering.md).

## Canonical dump

`HxcIRDumper` emits UTF-8 text with LF line endings. It sorts semantically
unordered modules, declarations, instances, globals, functions, locals,
cleanup regions, and blocks by UTF-8 ID (with the entry block first). It
preserves every semantically ordered list: parameters, fields, tag payloads,
instructions, switch cases, cleanup registration, edge arguments, and cleanup
execution steps. Every source-bearing module, declaration, storage, function,
block, instruction, terminator, and cleanup node includes its span; nested edge
data inherits the owning instruction or terminator span. Dumps contain no
timestamps, random IDs, locale-dependent formatting, or normalized-away checkout
paths.

The executable contract is:

```sh
npm run test:hxc-ir
```

`test/hxc_ir/expected/semantic.hxcir` demonstrates the ordered lowering intent
for `arr[nextIndex()] += produce()` and its nested cleanup path without a
runtime request. The checked-in Haxe fixture runs independently under Eval and
asserts the oracle trace `nextIndex,produce:8`. `coverage.hxcir` exercises all
dispatch forms plus ABI integers, tagged/pointer nullability, explicit primitive
conversion/failure forms, aggregate/tag, allocation, retain/trace, and lifetime
forms. Its named runtime requests are explicit non-primitive coverage.
`diagnostics.json` covers missing termination, use-before-definition, invalid
cleanup order, path redaction, constant/load/address/store/initializer type
mismatches, primitive runtime rejection, missing nullable-unwrap failure,
void/value return mismatches, return-type mismatch, and `HXC1001`.
The runner renders twice and reverses unordered inputs before comparing the
canonical bytes.

The direct HxcIR suite itself emits no C. The body-lowering suite generates a
test translation unit from real typed Haxe. The function-lowering suite extends
that evidence to parameters, conversions, calls, recursive prototypes, and the
production private header/source-set/entry project. The evaluation-order suite
adds source-backed calls, assignments, static fields, short circuit, ternary,
and increments, and deliberately reuses the representation-neutral indexed
compound-assignment IR until E2.T08 owns source array lowering. All select no
runtime files or public C ABI and compile/run as strict C11 with available GCC
and Clang at `-O0` and `-O2`.
