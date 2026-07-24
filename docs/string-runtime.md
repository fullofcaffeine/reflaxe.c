# UTF-8 scalar string runtime contract

This document records the bounded UTF-8 String runtime and its current
compiler-selected E5.T02 operations. It implements the storage and operation
contract from [ADR 0004](adr/0004-utf8-scalar-string-contract.md), but it does
not expose the private layout as a public ABI or claim complete Haxe
`String`/standard-library support.

Literal-backed Strings remain allocation-free. Runtime-created values from
`String.fromCharCode`, ordinary concatenation, and the upstream
`StringBuf.addChar` path use a small reference-counted owner. Those values may
cross calls and returns, aliases, branches, closed records and enums, fixed
Array literals, `Null<String>`, and class fields without dangling bytes.
`length`, `charAt`, `charCodeAt`, and `substring` use the shared Unicode-scalar
rules. Other String methods still fail closed. E4.T11 established the internal
same-major runtime contract, and E7 owns any future public ABI.

E4.T03 advanced the incompatible native-seed marker from 0.2.0 to 0.3.0.
E2.T07 added the provisional hosted output API and advanced it to 0.4.0;
E4.T04's additive array slice advances the internal same-major contract to
0.5.0, E5.T03's shared Array identity layer advanced it to 0.6.0, E5.T04's
fixed-length Bytes owner advanced it to 0.7.0, and the separate selected
collector/root API advanced it to 0.8.0. Preserving Haxe's legacy-nullable
String identity without changing the three-field layout advanced the internal
semantic contract to 0.9.0. Adding the optional owner pointer needed by
runtime-created ordinary Haxe values changes that private carrier and advances
the marker to 0.10.0. That marker does not stabilize the private string layout
or application ABI.

## Representation and invariants

`hxc_string` is the private immutable runtime value. It stores a byte pointer,
logical byte length, a trusted fact describing whether stable storage has a
NUL immediately after the logical bytes, and an optional opaque owner. A null
owner means the bytes have another proven lifetime, normally static literal
storage. A non-null owner keeps one allocator-backed immutable byte allocation
alive through explicit retain/release operations. The stored logical bytes are
always shortest-form UTF-8 for Unicode scalar values:

- embedded NUL is ordinary content and is included in length, equality, hash,
  slicing, and comparison;
- surrogate code points, overlong encodings, truncated sequences, lone
  continuation bytes, and values above U+10FFFF are not valid internal state;
- a trailing NUL is a storage convenience outside the logical length;
- canonical composition is never changed, so `é` and `e` plus U+0301 remain
  different strings; and
- scalar positions, never byte or UTF-16-unit positions, drive length, access,
  and slicing.

The byte pointer also carries Haxe reference-null identity. A null pointer is
Haxe `null`; every actual String, including `""`, has a non-null address.
Runtime validators reject the null carrier as String content, and generated
operations that require a value perform a checked null access first. This
keeps null distinct without adding a second tagged wrapper to every String.

`HXC_STRING_LITERAL` and the private `hxc_string` carrier live in the narrow
`string_literal.h` header. The initializer accepts an actual valid UTF-8 C
literal array and uses static storage plus `sizeof`, so it allocates nothing and
preserves embedded NUL. It is not an unchecked user-facing raw-string API;
compiler emission owns its valid input.

### Literal-backed values in generated programs

A *literal-backed String* is a normal typed Haxe `String` whose bytes are known
at compile time because they originate in source such as `"village:west"`.
The compiler validates those bytes as canonical UTF-8, places them in C storage
that lives for the whole program, and represents the Haxe value as an
`hxc_string` view containing a pointer and byte length. A *view* means the value
describes existing bytes; it does not own an allocation that must be freed.

That stable lifetime makes ordinary value operations cheap and safe. Copying
the value copies only the view, not its bytes. The value can therefore cross
parameters and returns and can be stored in closed records, tagged enums,
`Null<T>`, and managed Arrays without adding retain/release work. A nominal Haxe
abstract such as `ScenarioId(String)` keeps its Haxe identity in plans and
diagnostics even though its generated C carrier is the same String view.

Under Haxe's default legacy null safety, both `String` and `Null<String>` are
the same nullable reference value; `Null` is documentary for a reference type.
`Null<ScenarioId>` follows the same rule for an abstract over String. The
four-field view therefore uses its null data pointer for absence, while the
empty literal points at non-null static storage. Equality first handles null
identity, then compares non-null values by byte length and canonical UTF-8
content with `memcmp`. It never treats different non-null storage pointers as
unequal.

This is deliberately smaller than general String support. `String.charAt` and
`substring` return borrowed views into the receiver's bytes. When such a view
escapes its immediate expression, generated code retains the same optional
owner; it does not copy the slice. `String.fromCharCode` and concatenation
produce fresh owners, while aliases and aggregate/container copies retain them.
The last cleanup releases the allocation. `Sys.println(value)` remains
unsupported when the argument is not a literal at the call site: broadening
that output API is a separate standard-library decision.

`hxc_owned_string` pairs one immutable value with `hxc_allocation`. The complete
allocator callback/context identity therefore follows owned bytes and disposal
never consults an ambient allocator. A canonical owned empty string retains a
valid zero-size allocator owner and uses shared static trailing-NUL bytes without
calling the allocation callback. Live owners are move-only by convention.

`hxc_string_buffer` is the mutable construction boundary. Every completed
operation leaves its initialized prefix valid UTF-8 and trailing-NUL storage.
It grows through the retained allocator with checked sizes, supports appending a
borrowed view of its own current content, and transfers its allocation when
finished. Malformed append input and allocation failure leave the logical bytes
and length unchanged. Appending an invalid scalar integer emits one U+FFFD, as
required for code-point construction by ADR 0004.

These structures are runtime-private. Exported APIs later use explicit UTF-8
views or owned buffers rather than freezing these layouts.

## External UTF-8 policy

`hxc_byte_view` carries untrusted or binary bytes and asserts no encoding.
Checked conversion first validates the whole view and returns
`HXC_STATUS_INVALID_UTF8` without allocating or changing the output owner.
Lossy conversion is total: it replaces each maximal ill-formed subpart with one
U+FFFD, computes the exact output size with checked arithmetic, and performs at
most one allocation before publishing a valid immutable result.

The decoder consumes malformed prefixes deterministically. Representative
cases are:

| Input bytes | Lossy scalar sequence |
| --- | --- |
| lone `80` | U+FFFD |
| overlong `C0 AF` | U+FFFD, U+FFFD |
| truncated `E2 82` | U+FFFD |
| surrogate encoding `ED A0 80` | U+FFFD, U+FFFD, U+FFFD |
| out-of-range `F4 90 80 80` | four U+FFFD values |
| `E1 80 41` | U+FFFD, `A` |

Arbitrary bytes that must round-trip losslessly remain a byte view or future
`haxe.io.Bytes`; they are never installed as an invalid `String`.

## Scalar operations and allocation

The core operations have these costs and ownership effects:

| Operation | Allocation contract |
| --- | --- |
| Direct literal | None; static compiler-owned storage |
| Scalar length/access | None; scans UTF-8 from known boundaries |
| Borrowed scalar slice | None; lifetime remains tied to the source |
| Compare/equality basis | None; lexicographic canonical UTF-8 bytes |
| Stable hash | None; 32-bit FNV-1a over all logical bytes |
| Checked or lossy owned decode | Zero callbacks for empty, otherwise one exact allocation |
| Owned copy | Zero callbacks for empty, otherwise one exact allocation |
| Owned concatenation | Zero callbacks when both inputs are empty, otherwise one exact allocation |
| Mutable append | Geometric checked growth through the retained allocator |
| Owned CString conversion | One exact `byte_length + 1` allocation |

Canonical UTF-8 preserves scalar ordering under byte-wise lexicographic
comparison, so compare does not decode or allocate after invariant validation.
No scalar index table or cache is unconditional. Later compiler analysis may
emit direct C or a program-local specialization instead of selecting this slice
when lifetime and representation are statically known.

## CString boundary

`hxc_borrowed_cstring` and `hxc_owned_cstring` are deliberately different
records.

A borrowed conversion succeeds only when:

1. the String contains no embedded NUL;
2. its storage has a stable trailing NUL; and
3. the caller accepts a lifetime tied to the recorded source owner.

An interior slice therefore returns `HXC_STATUS_BORROW_UNAVAILABLE` rather than
pretending the next byte is a terminator. Copying the borrowed record does not
extend the owner lifetime. The pointer becomes invalid when owned source storage
is disposed or when a mutable builder view is changed.

Owned conversion also rejects embedded NUL, then allocates exact terminated
storage and retains the selected allocator identity until explicit disposal.
Neither path truncates at embedded NUL. The typed `c.Borrowed<c.CString>` and
`c.Owned<c.CString>` compiler surface will carry these distinctions when its
lowering owner is implemented.

## Feature and evidence boundary

The allocation-free `string-scalar` feature is compiler-selectable and depends
only on `status` plus the `string-literal` carrier. Ordinary Haxe `length`,
`charAt`, `charCodeAt`, and `substring` select this slice when their inputs
remain dynamic. Scalar views borrow the source bytes; literal-only programs
still avoid `alloc` and the broader `string` source.

The `string` feature is compiler-selectable and depends on `alloc` plus
`string-scalar`. A generated program selects it only for a reachable owned
operation or lifetime action: `from-scalar`, `concat`, `retain`, or
`cleanup-release`. It has no object, tracing collector, dynamic, reflection,
exception, thread, or Unicode-table dependency.

The compiler admits literal-backed String values as the direct
`string-literal/static-value` capability. A program using only those values
selects `runtime-base + string-literal`; this packages the private carrier
definition but no allocator or full String operations. E2.T07 additionally
admits compiler-known literals passed directly to hosted `Sys.println` or
default `trace`. Those output programs select
`runtime-base + status + string-literal + io`; they package no allocator or
`string.c` operation symbols. Generated C
stores exact validated UTF-8 bytes and byte length, including embedded NUL, and
the output helper writes by length, adds a newline, flushes, and returns
`HXC_STATUS_IO_ERROR` on write or flush failure. The generated caller follows
the admitted fail-stop policy by aborting on any non-OK status.

`Bytes.ofString(value)` may also consume an admitted String parameter, local,
or alias. “Runtime” here describes when the expression is selected, not who
owns its bytes: the value remains the same immutable length-delimited view
backed by already-admitted storage. The Bytes operation copies those bytes
synchronously into its own managed allocation, so it does not add another
String owner. Runtime-created Strings already carry their own owner when
required.

[`test/differential/string-runtime`](../test/differential/string-runtime)
has two complementary halves. Its independent strict-C fixture covers checked
and maximal-subpart lossy decoding, slicing, comparison, stable hashing,
builder failure atomicity, allocator identity, borrowed/owned CString lifetime,
reference counts, and exact allocations. Its ordinary-Haxe fixture compares
Eval with generated C for `String.fromCharCode`, upstream `StringBuf.addChar`,
concatenation, aliases, branches, records, enums, arrays, reassignment,
nullable values, calls, returns, and borrowed scalar slices. Generated projects
are checked in split, package, and unity layouts under cold, reversed, and warm
compiler-server discovery. Strict C11 `-O0`/`-O2`, C++17 headers,
AddressSanitizer, UndefinedBehaviorSanitizer, runtime-none rejection, and
malformed-HxcIR diagnostics keep both behavior and ownership plan honest.

[`test/differential/string-char-at`](../test/differential/string-char-at)
compares ordinary Haxe `String.charAt` with Eval for ASCII, non-Basic
Multilingual Plane scalars, embedded NUL, empty, negative, and out-of-range
indices. It also proves the exact allocation-free runtime plan and generated
strict C across split, package, and unity layouts. This is evidence for
`charAt`, not for neighboring String methods.

[`test/differential/string-null`](../test/differential/string-null) compares
the pinned Eval and generated-C behavior for plain and explicit nullable
Strings. It covers calls, returns, early control flow, aliases, a nominal
String abstract, null-versus-empty equality, and a record-carried,
String-backed enum-abstract switch. C cannot use a String as the subject of its
native `switch` statement, so haxe.c evaluates the Haxe subject once and lowers
the cases to ordered String-content comparisons in HxcIR. The generated C is
then ordinary structured `if`/`else` control flow; it never compares String
data pointers as if pointer identity meant equal text. The suite checks that
semantic shape, default and exhaustive behavior, HxcIR null identity, strict
C/C++ consumption, deterministic layouts, and sanitizers. Its negative fixture
keeps runtime-created String operations outside this bounded capability.

[`test/enum_lowering`](../test/enum_lowering) proves that a nominal
abstract-over-String literal can be constructed, copied, passed, returned,
stored in an enum, projected, and compared across split, package, and unity
layouts. [`test/string_output`](../test/string_output) adds generated-Haxe
evidence for the output edge. It compares exact bytes with Eval, checks UTF-8
scalars and embedded NUL, exercises portable and metal policy, rejects
nonliteral/general output forms, and closes stdout to prove error handling.
Together these tests prove the bounded literal-backed value model, not general
String or I/O support.

Run the focused evidence with:

```sh
python3 test/differential/string-runtime/run.py
python3 test/differential/string-runtime/run.py --native-only --toolchain clang
npm run test:string-char-at
npm run test:string-null
npm run test:runtime-features
npm run test:string-output
npm run test:native
```
