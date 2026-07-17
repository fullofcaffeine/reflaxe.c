# UTF-8 scalar string runtime contract

This document records the bounded E4.T03 native `hxrt` string slice. It
implements the storage and operation contract from
[ADR 0004](adr/0004-utf8-scalar-string-contract.md), but it does not make the
slice compiler-selectable, expose the private layout as a public ABI, or claim
general Haxe `String`/standard-library lowering. E2.T07 and E5.T02 own those
later compiler and library connections; E4.T11 owns runtime ABI stabilization.

The incompatible provisional native-seed revision advances the runtime marker
from 0.2.0 to 0.3.0. That marker remains an inspection aid, not a compatibility
promise.

## Representation and invariants

`hxc_string` is the private immutable runtime value. It stores a byte pointer,
logical byte length, and a trusted fact describing whether stable storage has a
NUL immediately after the logical bytes. The stored logical bytes are always
shortest-form UTF-8 for Unicode scalar values:

- embedded NUL is ordinary content and is included in length, equality, hash,
  slicing, and comparison;
- surrogate code points, overlong encodings, truncated sequences, lone
  continuation bytes, and values above U+10FFFF are not valid internal state;
- a trailing NUL is a storage convenience outside the logical length;
- canonical composition is never changed, so `é` and `e` plus U+0301 remain
  different strings; and
- scalar positions, never byte or UTF-16-unit positions, drive length, access,
  and slicing.

`HXC_STRING_LITERAL` is a compiler-owned direct initializer for an actual valid
UTF-8 C literal array. It uses static storage and `sizeof`, so it allocates
nothing and preserves embedded NUL. The macro is not an unchecked user-facing
raw-string API; later compiler emission owns its valid input.

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

The `string` feature remains `native-seed-only` and depends only on
`alloc -> status -> runtime-abi`. It has no object, tracing collector, dynamic,
reflection, exception, thread, or Unicode-table dependency. A generated Haxe
program still cannot request it; current admitted generated programs retain an
empty runtime plan.

[`test/differential/string-runtime`](../test/differential/string-runtime)
compares BMP/non-BMP/embedded-NUL/composed/decomposed scalar behavior with the
pinned Haxe Eval oracle. Its independent strict-C fixture covers checked and
maximal-subpart lossy decoding, slicing, comparison, stable hashing, builder
aliasing and failure atomicity, allocator identity, borrowed/owned CString
lifetime, and exact allocation counts. Required GCC and Clang lanes run at
`-O0`, `-O2`, and with AddressSanitizer plus UndefinedBehaviorSanitizer, then
inspect the link for the string symbols and absence of object/GC/reflection/
dynamic families.

Run the focused evidence with:

```sh
python3 test/differential/string-runtime/run.py
python3 test/differential/string-runtime/run.py --native-only --toolchain clang
npm run test:runtime-features
npm run test:native
```
