# ADR 0004: UTF-8 Unicode-scalar string contract

- Status: Accepted
- Date: 2026-07-14
- Decision owners: project owner and compiler maintainers
- Related requirements: HXC-SEM-006, HXC-SEM-008, HXC-RT-004, HXC-RT-008

## Context

Haxe `String` storage and non-BMP indexing differ by target. The portable C
target needs one deliberate observable contract rather than inheriting byte
offsets from C or surrogate-unit offsets from a convenient host. It must also
interoperate with byte buffers and NUL-terminated C APIs without confusing
their ownership, encoding, or length rules.

The original Haxe 4.3.7 Eval experiment provided an appropriate oracle for a
non-UTF-16 custom target. A local `--interp` probe observed:

- `"😀".length == 1`, `charCodeAt(0) == 128512`, and
  `new UnicodeString("😀").length == 1`;
- composed `"é"` has length one while decomposed `"e\u0301"` has length two;
- `"a\x00b"` has length three.

This agrees with Haxe's documented target-specific representation model and
with the project's choice to enable `target.unicode` without `target.utf16`.

The later carrier experiment found that Haxe 4.3.7's default Reflaxe `Cross`
configuration predefines `target.utf16`/`utf16` and cannot remove them through
the public initialization-macro API. ADR 0007 therefore selects the exact Haxe
5.0.0-preview.1 custom-target carrier. Its production platform snapshot proves
`usesUtf16=false`, `supportsUnicode=true`, `target.unicode`, no
`target.utf16`/`utf16`, and the scalar branches of upstream `String`,
`StringTools`, and `UnicodeString`. Eval remains an independent oracle and CLI
bootstrap host only; neither Eval nor legacy Cross types production C programs.

## Decision

### Portable `String` is immutable, valid UTF-8

The internal portable representation is a target-owned immutable value carrying
at least a byte pointer and byte length. It may also carry ownership flags,
cached scalar length, an ASCII flag, or indexing metadata, but its layout is
private and is never the stable exported C ABI.

With Haxe's default legacy null safety, `String` is a nullable reference type;
writing `Null<String>` documents that fact but does not introduce another
source-level value representation. The private C carrier therefore represents
Haxe `null` directly. In the current layout a null data pointer is the null
discriminant, while every real String--including `""`--has a non-null byte
address. This keeps `null != ""`, lets plain `String` parameters receive the
null values that Haxe permits, and avoids a redundant presence wrapper.

The stored bytes are well-formed, shortest-form UTF-8 for Unicode scalar values:

- embedded NUL is ordinary string content;
- a trailing NUL is neither required nor part of the logical length;
- surrogate code points and ill-formed UTF-8 are not valid internal `String`
  state;
- no Unicode normalization is performed, so canonically equivalent scalar
  sequences remain distinct;
- literals may use static deduplicated storage, and owned or borrowed storage
  need not select the object or collector runtime features.

Ordinary UTF-8 decoding of external bytes is total and deterministic: each
maximal subpart of an ill-formed byte sequence becomes U+FFFD. Code-point
constructors given a surrogate or out-of-range integer use the same replacement.
Checked target APIs expose validation as an explicit result for callers that
must reject malformed input. Arbitrary or losslessly preserved bytes remain
`haxe.io.Bytes` or a typed byte span, never an invalid `String`.

On this target `haxe.io.Encoding.RawNative` is UTF-8 because UTF-8 is the native
string encoding. It still produces a valid `String`; it is not a binary-data
escape hatch.

### Every Haxe string index is a scalar index

`String.length`, `charAt`, `charCodeAt`, substring/slice bounds, search results,
and Unicode string iterators use zero-based Unicode scalar positions. No API
exposes UTF-8 byte offsets or UTF-16 surrogate halves as ordinary Haxe string
indices. Byte-oriented target APIs name their units explicitly.

`charCodeAt` returns the scalar value at a valid position. String iteration and
`UnicodeString` reuse the same scalar model rather than building a second
representation. Comparison, equality, and hashing operate on the exact scalar
sequence; UTF-8's unique encoding of a valid scalar sequence permits byte-wise
fast paths. ASCII operations use direct byte indexing. Non-ASCII random access
may scan from a known boundary or use planner-justified cached metadata, but no
index table or allocation is unconditional.

### `CString` and ABI views are separate contracts

`c.CString` is a NUL-terminated byte-string abstraction, not an alias for Haxe
`String`. Safe conversion from `String` rejects embedded NUL. A borrowed
conversion is available only when stable trailing-NUL storage exists and its
lifetime is statically tied to the source owner. Otherwise conversion allocates
an explicitly owned C string through the selected allocator. Truncating at an
embedded NUL is never the default safe behavior.

Exported C APIs use explicit UTF-8 views such as pointer plus byte length, or an
owned buffer contract. They do not expose the private `hxc_string` layout. A
NUL-terminated export is opt-in and documents ownership, allocator, embedded-NUL
failure, and lifetime.

## Consequences

- Haxe source, macros running on Eval, and generated C share logical non-BMP
  indexing instead of leaking C byte offsets.
- UTF-8 is efficient for source literals, C libraries, files, protocols, and
  generated headers while remaining independent of NUL termination.
- Non-ASCII random indexing is not promised constant-time; optional caches are
  reported costs rather than hidden baseline allocation.
- Invalid external text has a stable lossy conversion and a separate checked
  conversion; binary round trips stay on `Bytes`.
- Legacy-nullable `String` values preserve null identity across parameters,
  returns, control-flow joins, and nominal abstracts without confusing null
  with the empty String.
- The string runtime can link independently of object graphs, reflection, and
  the collector.

## Rejected alternatives

- Raw `char *` as Haxe `String`: it loses embedded NUL, length, encoding,
  ownership, and lifetime information.
- UTF-8 byte indexing: it contradicts the selected Haxe Unicode target contract
  and makes non-ASCII behavior accidentally C-shaped.
- UTF-16 storage/indexing: it complicates C interoperability and would expose
  surrogate units despite selecting a non-UTF-16 Haxe target.
- Implicit normalization: it changes equality, length, hashes, and round trips.
- Allowing malformed UTF-8 inside `String`: every consumer would need a second
  error model and ordinary Haxe operations could cease to be total.
- Encoding null as the empty String: Haxe observes them as different values.
- Wrapping only explicit `Null<String>` in a tagged optional: under default
  null safety plain `String` is nullable too, so the wrapper would make
  equivalent Haxe types use incompatible call representations.
