# ADR 0008: Primitive representations and conversions

- Status: Accepted
- Date: 2026-07-15
- Decision owners: project owner and compiler maintainers
- Related requirements: HXC-SEM-001, HXC-SEM-002, HXC-SEM-003, HXC-SEM-005

## Context

Primitive lowering is the first point where a compact C spelling can silently
change Haxe behavior. C leaves signed overflow undefined, permits target-sized
fundamental integer types, makes some out-of-range conversions
implementation-defined or undefined, and does not require `double` to be IEC
60559 binary64. Null also has no single representation that works for both
scalar values and references.

The two product profiles must not become two unrelated type systems. A profile
can admit a source-level native contract, but it must not silently change the
meaning of an ordinary Haxe primitive. Runtime selection also occurs after
representation and specialization decisions; a primitive must not request a
broad runtime merely because a direct C cast is unsafe.

The complete machine-readable form of this decision is
`docs/specs/primitive-semantics.json`, validated by
`docs/specs/primitive-semantics.schema.json`.

## Decision

### Primitive representations are exact target-owned identities

The compiler maps source types as follows in both `portable` and `metal`:

| Source type | Internal C representation | Required C header |
| --- | --- | --- |
| `Void` | `void` | none |
| `Bool` | `bool` | `<stdbool.h>` |
| `Int` | `int32_t` | `<stdint.h>` |
| `UInt` | `uint32_t` | `<stdint.h>` |
| `Float` | `double`, with binary64 required by the resolved target | none |
| `c.Int8/16/32/64` | `int8_t/int16_t/int32_t/int64_t` | `<stdint.h>` |
| `c.UInt8/16/32/64` | `uint8_t/uint16_t/uint32_t/uint64_t` | `<stdint.h>` |
| `c.Size` | `size_t` | `<stddef.h>` |
| `c.PtrDiff` | `ptrdiff_t` | `<stddef.h>` |
| `c.IntPtr` | `intptr_t` | `<stdint.h>` |
| `c.UIntPtr` | `uintptr_t` | `<stdint.h>` |

`c.Size`, `c.PtrDiff`, `c.IntPtr`, and `c.UIntPtr` remain distinct target-ABI
identities in HxcIR until native facts are resolved. The compiler never
rewrites one to `int`, `long`, `long long`, or a guessed bit width. A target
without the required exact-width typedefs, binary64 `double`, eight-bit bytes,
or pointer-round-trip integer typedefs is rejected for this contract instead
of receiving a host-derived fallback.

These are internal semantic representations. E7 still owns stable exported ABI
policy, where `Bool` or another internal scalar may require an explicitly
versioned fixed-width boundary representation.

### Ordinary Haxe meaning does not vary by profile

Portable and metal use the same mapping and conversion rules for the same
typed Haxe input. Metal differs only when the source uses an explicit typed C
contract such as `c.Size` or an eventual checked/wrapping native operation.
Selecting the profile alone never changes `Int`, `UInt`, `Float`, `Bool`, or
`Null<T>` storage.

### Integer conversions name their semantic operation

HxcIR distinguishes exact, wrapping, checked, and saturating numeric
conversions. Lowering follows these rules:

- every signed or unsigned 32-bit Haxe integer is represented exactly in
  binary64, so `Int`/`UInt` to `Float` is an exact direct conversion;
- `Int` to `UInt` uses modulo 2^32, which ISO C defines for conversion to the
  unsigned target type;
- `UInt` to `Int` and wrapping conversion to a signed exact-width type preserve
  the low bits and interpret them as the corresponding two's-complement value;
  a compiler-generated program-local helper performs the reconstruction rather
  than relying on an implementation-defined out-of-range unsigned-to-signed C
  cast;
- a wrapping conversion to an unsigned exact-width type uses the ISO C modulo
  rule;
- a widening conversion is direct only when the source range is a subset of
  the target range;
- a checked narrowing conversion performs an explicit range check and carries
  a typed HxcIR failure edge before the exact conversion.

Target-ABI integers are not narrowed, widened, or range-compared from a guessed
host width. Such a conversion remains rejected until the resolved native ABI
facts make the decision sound.

### Arithmetic operations have an explicit UB-safe contract

The compiler chooses a typed operation and implementation in HxcIR before it
chooses C syntax. Ordinary Haxe `Int` arithmetic has 32-bit two's-complement
wrapping behavior, but generated C never performs an overflowing signed
operation. Addition, subtraction, multiplication, negation, and bitwise
operations execute through explicitly unsigned 64-bit intermediates, retain
the low 32 bits, and use a small compiler-emitted helper to reconstruct
`int32_t` without an implementation-defined out-of-range cast. The widening is
required because a conforming C implementation may otherwise promote
`uint32_t` to a wider signed `int` before the operation.

The remaining integer edge rules are:

- every shift count is masked with `31`, including a negative Haxe count;
- left and unsigned-right shifts operate on `uint32_t` bits;
- signed right shift performs explicit sign extension rather than relying on
  implementation-defined C behavior;
- `Int` and `UInt` modulo by zero return `0`, an intentional deterministic
  target refinement for a Haxe edge whose behavior is target-specific;
- `INT32_MIN % -1` returns `0` without evaluating the overflowing C operation;
- Haxe `/` produces `Float`, so integer operands convert exactly to binary64
  before division and `INT32_MIN / -1` is exactly `2147483648.0`.

`UInt` addition, subtraction, multiplication, bit operations, and masked shifts
remain direct C, using an explicit `uint64_t` intermediate and `uint32_t`
narrowing so C integer promotions cannot change the operation. Only zero-safe
modulo needs a program-local helper. The pinned Haxe `UInt`
abstract expands several operations through signed and floating expressions;
the frontend recognizes only that exact typed standard-library expansion and
recovers the underlying unsigned operation. A Haxe pin change must re-audit
this structural recognition instead of accepting an arbitrary source pattern.

### Floating behavior is binary64 and fast-math is not semantic lowering

Haxe `Float` storage and ordinary operations preserve binary64 NaN, infinity,
and signed-zero behavior. Ordered comparisons follow IEC 60559 behavior: NaN
is unordered and unequal to itself, infinities retain their ordering, and
negative zero compares equal to positive zero while its stored sign remains
available to applicable floating operations.

The target forbids fast-math modes for Haxe semantic lowering when they may
erase NaN, infinity, or signed-zero behavior. A build adapter may not silently
enable such a mode as an optimization preset.

For a finite, in-range `Float`, `Std.int` truncates toward zero. The Haxe
contract leaves NaN, infinities, and out-of-range results unspecified; this C
target deliberately defines them so the generated program never executes an
undefined floating-to-integer C conversion:

- NaN and either sign of zero become `0`;
- positive infinity and positive finite overflow become `INT32_MAX`;
- negative infinity and negative finite overflow become `INT32_MIN`.

This refinement is identical in both profiles. It is emitted as a
program-local specialized operation, with comparisons performed before the
final in-range C conversion.

Floating addition, subtraction, multiplication, negation, and comparisons use
direct `double` operations. Division uses a zero-safe program-local helper so
zero yields the appropriate NaN or signed infinity without asking a sanitizer
to tolerate a native divide-by-zero operation. Floating modulo uses `fmod`;
zero yields NaN through the helper, and the compiler records the exact `m` link
requirement with source-module provenance. That build fact is not a runtime
feature.

### Nullability is representation-explicit

`Null<T>` for a non-null scalar uses a tagged optional containing a presence
bit and `T` payload. The payload is read only when present. A proven non-null
value may erase the wrapper; absence of such a proof must preserve it.

Reference-like values and explicitly nullable native pointers use the null
pointer representation. Ordinary `c.Ptr`/`c.Ref` contracts remain non-null.
Ownership and lifetime are separate facts and cannot be inferred from
nullability.

Nullable injection and unwrap are distinct HxcIR conversions. Injection is
non-failing. Unwrap requires an explicit failure edge unless prior analysis
has proved presence and erased the wrapper. The later failure-strategy pass
chooses result, exception, or policy behavior; no unchecked null dereference is
introduced here.

### Primitive operations are compiler-first and runtime-free

Primitive representation, operations, direct conversions, and specialized
helpers select no `hxrt` feature. The implementation preference is:

1. direct, idiomatic, strictly defined C;
2. the smallest program-local helper specialized by the compiler;
3. a runtime feature only if a later semantic operation demonstrates that the
   first two choices are infeasible.

Selected helpers form a deterministic dependency closure. They are emitted as
private `static inline` structural C with registry-finalized names and only for
the requesting program. Optimized builds are expected to erase the call
boundary. HxcIR validation rejects a primitive operation or an exact,
wrapping, checked, saturating, or nullable primitive conversion that names a
runtime feature. The checked and nullable unwrap forms also reject a missing
failure edge.

## Evidence boundary

The primitive-contract fixture compiles real Haxe fields through the pinned
typed-AST macro API, compares portable and metal decisions, renders the schema-2
machine contract twice, and exercises representation and conversion algorithms
in independent strict C11 at `-O0` and `-O2`.

The arithmetic suite is separate generated-Haxe evidence. It lowers real
`Int`/`UInt`/`Float` operators and `Std.int` through HxcIR, checks the selected
helper closure and exact `m` build fact, compares defined behavior with Eval,
and executes boundary inputs under strict GCC and Clang at `-O0`, `-O2`, and
UBSan where supported. It verifies that optimized assembly retains no
out-of-line specialization symbol and that safe unsigned paths remain direct
C. Portable, metal, and explicit `hxc_runtime=none` projects remain runtime-
free. E2.T11 still owns broader generated-program differential and sanitizer
coverage beyond this primitive slice.

## Consequences

- Primitive layout and conversions have one reviewable authority rather than
  being reconstructed from C spellings.
- C implementation-defined and undefined conversions cannot become accidental
  semantics.
- Signed overflow, invalid shifts, division overflow, and unchecked
  floating-to-integer conversion cannot become accidental generated-C behavior.
- Profile selection stays orthogonal to explicit source contracts.
- Runtime-free primitive programs remain eligible for a build with no `hxrt`
  include, source, define, library, or symbol.
- Native ABI width facts remain the responsibility of Clang/probes and later
  generated static assertions, not the Haxe compiler host.

## Rejected alternatives

- Mapping Haxe `Int` to C `int` or target `long`: neither spelling guarantees
  the required width across supported ABIs.
- Emitting ordinary C casts for every conversion: out-of-range signed and
  floating conversions do not provide a portable semantic contract.
- Emitting signed C arithmetic and relying on wrapping compiler flags: that
  would make source correctness depend on a native build option and would not
  solve invalid shifts or division overflow.
- Using a tagged union for every nullable value: references and native pointers
  already have an exact null pointer representation.
- Using a null pointer for nullable scalars: valid scalar zero values would be
  conflated with absence.
- Letting metal silently inherit host C widths or unchecked conversions: that
  would make source meaning depend on the build machine rather than an explicit
  typed contract.
- Putting primitive conversion helpers in unconditional `hxrt`: it would add a
  hidden runtime dependency to operations the compiler can specialize safely.
