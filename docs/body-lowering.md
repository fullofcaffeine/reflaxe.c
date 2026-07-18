# Primitive function-body lowering

E2.T02 established the first compile-backed path from real pinned-Haxe
`TypedExpr` bodies through validated HxcIR to structural strict C11. E2.T03
reuses this layer for typed parameters and direct calls and packages the
admitted primitive graph as a production C project. E2.T04 adds explicit
multi-block sequencing for assignments, primitive static fields, lazy Boolean
operators, value conditionals, and unsigned increments. E2.T05 adds typed
UB-safe primitive operators, compound updates, signed updates, and `Std.int`.
E2.T06 adds statement conditionals, loops, primitive switches, and explicit
loop jumps without changing the runtime-free compiler-first boundary. E2.T08
and its bounded storage extension add nonempty literal-backed and compile-time-
sized zero-initialized `c.CArray` locals, borrowed `Span`/`ConstSpan` views,
checked indexing, and direct exact-width span iteration.
E3.T01 extends the same typed pipeline with closed anonymous value records,
including nested records, explicit copies, and typed field addressing and
projection. Its exact boundary is documented in [closed anonymous-record
lowering](aggregate-lowering.md).
E3.T02 adds bounded algebraic enum values, checked payload projection, and
exhaustive tag control flow. Its representation and lifetime boundary is
documented in [Haxe enum lowering](enum-lowering.md).
This document remains the contract for individual body semantics; [static function
lowering](function-lowering.md) owns graph, prototype, call, and
executable-entry behavior, while [evaluation order](evaluation-order.md) owns
the stable-value and control-flow proof.

## Admitted subset

`CBodyLowering` accepts typed functions containing:

- `Void`, `Bool`, `Int`, `UInt`, and `Float` return/local/value types;
- compiler-typed `TInt`, `TFloat`, and `TBool` constants plus numeric negation,
  bitwise complement, and Boolean negation;
- initialized automatic locals and local reads;
- primitive parameters and direct static calls whose typed signatures are in
  the same admitted graph;
- implicit direct primitive conversions recorded before their calls;
- local assignment and referenced primitive static-field load/store with
  captured typed initializers; the focused body unit admits constants directly,
  while production E2.T09 lowers every explicit field through a deferred
  initializer function;
- short-circuit `&&`/`||` and value-form ternary expressions through explicit
  HxcIR blocks;
- statement `if`/`else`, pre-test `while`, post-test `do-while`, and range
  `for` forms that the pinned Reflaxe preprocessing exposes as typed primitive
  loop graphs;
- nested loop `break`/`continue` through explicit target-owned jump edges;
- `Int` statement/value switches with typed integer cases and a value-form
  `default`;
- `Int`/`UInt`/`Float` arithmetic and comparisons, Haxe division, integer and
  floating modulo, masked integer shifts, and integer bit operations;
- arithmetic compound assignment plus prefix/postfix numeric increment and
  decrement through explicit load/operation/store;
- `Std.int(Float)` through the defined saturating/truncating primitive
  conversion;
- nonempty primitive array literals assigned directly to `c.CArray<T, N>` or
  `CArray.zero` with a positive compiler-known product within the exact element-
  size and 65,536-byte per-array storage policy, local mutable/const span
  borrows, checked element reads/writes, and `for` iteration over those views
  through explicit guarded HxcIR blocks;
- closed anonymous records containing admitted primitive or nested closed-record
  fields, including object literals, local copies, direct parameters/calls/
  returns, and read-only field access;
- fieldless Haxe enums, payload enums whose fields are admitted primitive or
  enum values, distinct concrete primitive generic instances, exhaustive
  source `switch` patterns, and recursive enum values whose indirect backing
  storage remains within the constructing function;
- nested primitive blocks and parentheses; and
- explicit value/void returns or an implicit final void return.

The lowering preserves each typed instruction in Haxe evaluation order. Nested
lexical blocks may share a semantic block only while they introduce no edge;
lazy, conditional, loop, and switch forms create explicit branch/jump/switch
blocks and typed flow locals. Each Haxe switch arm has its own HxcIR target and
the structural C switch immediately jumps to that target, so one arm can never
fall through into another. Locals and globals remain addressable HxcIR places,
while constants, loads, calls, and pure operators produce immutable values.

The pinned Reflaxe pass may expose a value switch as an uninitialized temporary
followed by a switch that assigns it. The frontend admits that carrier only
after structurally proving that every case and `default` assigns the same typed
local. It emits a defensive typed initialization before those stores, so no C
path can read indeterminate storage and general uninitialized Haxe locals remain
fail-closed.

Return validity is checked twice. Frontend lowering maps the typed function and
return expression independently, then `HxcIRValidator` rejects missing values,
values on `Void` returns, and value/type mismatches as internal invariants. The
C body emitter consumes only validated IR.

Default/optional/rest parameters, indirect calls, arbitrary collection/iterator
lowering, general Haxe arrays, escaping spans, string/Float switches, guarded
patterns outside the admitted enum form, recursive enum parameters/returns,
reference or aggregate enum payloads, mutable/open/reflective records, record
or enum identity/equality, strings, closures, general allocation, exceptions,
and cleanup remain outside this primitive slice. Concrete class
references/fields and bounded nonescaping constructors are separately admitted
by E3.T04/E3.T05. The first
unsupported typed node fails with `HXC1001` at its exact Haxe range; lowering
never substitutes `Dynamic`, `Any`, reflection, raw C, or an invented value.
Source that remains after a terminating return or loop jump receives the same
stable source-positioned `HXC1001` family.

## Names, source mapping, and C shape

Every function, global, local, stable-value temporary, generated label, and
selected program-local helper is registered in the per-compilation
`CSymbolRegistry` before names are finalized. A local request includes its
lexical source ordinal and function namespace, so two shadowed Haxe variables
with the same spelling receive distinct deterministic C identifiers. Input
order reversal and repeated renders must produce identical HxcIR, C, and symbol
records.

Haxe compiler positions become normalized, repository-relative, one-based
HxcIR spans. The C body emitter has two structural modes: ordinary output and
output with typed `#line` statement/declaration nodes. It never interpolates a
raw directive. Both forms are strict C11 and use only the required standard
headers. Selected arithmetic helpers may add `<math.h>` or `<stdint.h>`, and
floating modulo adds the exact `m` build fact.

Fixed arrays use structural C array declarators; zero storage uses the C11
aggregate initializer `{ 0 }` without allocation or `memset`. Span borrows use
typed pointers plus `size_t` element counts, and array access uses structural subscripting.
Static and loop-dominance proofs remove only redundant checks; all other
admitted indexes retain an explicit negative/upper-bound fail-stop check. See
[fixed arrays and span-based iteration](span-lowering.md) for the complete
profile/build matrix and overflow/lifetime proof.

Fieldless enums use structural C enum definitions. Payload enums use a
structural discriminant enum, per-constructor payload structs, a payload union,
and an outer value struct. Payload reads compare the tag before accessing the
union member and call the registered C `abort` symbol on mismatch. Exhaustive
HxcIR tag switches become structural C switches, and recursive local values use
typed pointer declarators to function-local backing storage.

Bounded constructor sites use structural automatic class storage, explicit
default initialization, and ordinary private C constructor calls. HxcIR keeps
the `initializing`/`initialized` transitions, status failure convention, and
ordered cleanup edges visible before C emission. See [bounded constructor
lowering](constructor-lowering.md); escaping references and general object
allocation remain fail-closed.

Every local/global load first becomes a typed stable-value temporary; an unused
read then becomes an explicit `(void)temporary` statement so source order is
retained without native warnings. The emitted body slice selects no runtime
feature, header, source, define, or symbol. Compiler-owned helpers remain
private `static inline` declarations; the C math library fact is a build
requirement, not an `hxrt` feature. Portable and metal use the same
representation and operation semantics.

## Production boundary

`CCompiler` locates the real typed static `main`, including the Haxe case where
the normalized module array omits its class but retains the typed entry-point
call. It then collects the reachable direct-static graph before lowering any
body. A wholly admitted direct-value graph now emits an owned private header,
source, `int main(void)` wrapper, and analyzed runtime/ABI/stdlib sidecars.

Production remains fail-closed outside that exact slice. An unsupported body,
signature, or call emits source-positioned `HXC1001` and no artifact. The broad
`HXC1000` scaffold diagnostic remains a distinct capability family but is no
longer the successful primitive-body terminus. The focused body fixture's
translation-unit render is still useful snapshot evidence; the production
project and entry point are independently checked by both this suite and the
function-lowering suite.

## Evidence and compatibility

Run:

```sh
npm run test:body-lowering
npm run test:evaluation-order
npm run test:arithmetic-semantics
npm run test:span-lowering
npm run test:aggregate-lowering
npm run test:constructor-lowering
npm run test:enum-lowering
npm run snapshots:check
```

The focused body and evaluation suites render twice, reverse function discovery
order, compare portable with metal, check exact HxcIR/C/symbol snapshots, prove
shadow-safe names, exact unsupported/unreachable diagnostic ranges, nested loop jumps,
non-fallthrough switches, and exact-once short-circuit/switch evaluation. They
compile/run mapped and unmapped C with
available GCC and Clang at `-O0` and `-O2` under
`-std=c11 -Wall -Wextra -Werror -Wconversion -Wsign-conversion` and
`-pedantic-errors`. Required CI lanes pin each compiler family and verify its
command identity before compiling the checked-in generated snapshots. The enum
suite additionally pairs each required C compiler with its C++17 companion to
verify private tagged-union and recursive layout.

This slice has no public C ABI, broad standard-library support, general object
allocation, or `hxrt` dependency. The one admitted `Std.int` intrinsic is compiler lowering,
not a general stdlib claim. Standard-library parity remains the E5 feature
graph: direct idiomatic C first, program-local specialization next, and only
the narrowest justified optimized runtime slice last.
