# `haxe.io.Bytes`: fixed-length binary storage

## What this slice provides

`haxe.io.Bytes` stores arbitrary bytes. It is the right type for a saved map,
an image, a network packet, or any other data where every value from 0 through
255 matters. It is deliberately separate from `String`, which contains valid
UTF-8 text, and from `Array<Int>`, which is a resizable sequence of signed Haxe
integers.

The first haxe.c slice supports ordinary Haxe calls to:

- `Bytes.alloc`, with deterministic zero initialization as a haxe.c safety
  guarantee;
- `length`, `get`, and `set`;
- `sub`, `blit`, `fill`, and `compare`;
- `Bytes.ofString` when its input is a compile-time String literal and the
  optional encoding is absent or `null`.

Assignment shares the same mutable Bytes value. `sub` is different: it creates
an independent copy. `set` and `fill` keep the low eight bits of the supplied
Haxe `Int`. Every position and range is checked before C performs pointer
arithmetic, and `blit` preserves the source bytes when two ranges of the same
value overlap.

This is a deliberately bounded implementation, not a claim that all of
`haxe.io.Bytes`, `BytesBuffer`, `Input`, or `Output` is complete. Unsupported
methods fail at their Haxe source position with `HXC1001` and leave no plausible
generated project.

`compare` follows the pinned executable Haxe implementations: it compares the
shared prefix byte by byte, then compares lengths only when that prefix is
equal. For example, `[0xff]` sorts after `[0x00, 0x00]`. The prose comment in
the pinned portable source describes length-first ordering, but its actual
portable body and the JS and HashLink overrides use prefix-first ordering. The
Eval/C differential fixture keeps the executable cross-target behavior as the
oracle and records this otherwise easy-to-miss edge explicitly.

## Why the C target has a small `_std` declaration

Haxe normally compiles the portable implementation in
`haxe/io/Bytes.hx`. Its `get` and `set` methods are `inline`: before Reflaxe
receives the typed program, Haxe can replace a call with direct access to the
private target-neutral `BytesData` array. At that point the operation no longer
says “read one byte from this Bytes value,” so haxe.c would have to guess the
lost identity from an implementation detail.

[`std/c/_std/haxe/io/Bytes.hx`](../std/c/_std/haxe/io/Bytes.hx) instead declares
the pinned API without implementing it. Haxe therefore preserves nominal,
well-typed calls for the compiler. haxe.c admits the supported methods into
HxcIR and rejects the others. This follows normal Haxe target practice and is
more robust than recognizing the current inlined shape.

The declaration contains the complete pinned surface so ordinary Haxe code is
typed consistently. A declaration is not an implementation or support claim.
Its adapted upstream path, revision, license, and exact content hash are kept in
[`third-party-provenance.json`](specs/third-party-provenance.json).

## How it becomes C

HxcIR records Bytes as one managed reference type. “Managed” here means that
the generated program explicitly counts live owners; it does not mean that a
garbage collector is selected.

The lowering convention is:

1. a newly allocated or copied Bytes result begins with one owner;
2. binding that result to a local transfers that owner into the local;
3. assigning an existing Bytes value to another local retains the shared owner;
4. leaving the function releases owning locals in reverse construction order;
5. after the final release, the exact byte allocation and its small owner record
   are returned through the allocator that created them.

A function may return a fresh Bytes value or one of its own named owning
locals. That return transfers the existing owner to the caller, so the callee
deliberately omits only that value's normal end-of-function release. Returning
a borrowed parameter or field is different: the caller already owns that
value, and the callee would need to create a new owner with `retain`. That
borrowed-return path still fails closed until its ownership operation is
represented explicitly; it is not silently treated as a transfer.

An ordinary call may borrow a fresh result directly. For example,
`firstByte(makeText())`, `left.compare(right.sub(0, 4))`, and a fresh Bytes
receiver all receive a hidden caller-owned local. Haxe still evaluates nested
expressions from left to right. The outer call borrows each stable local, and
the caller releases the owners in reverse order on normal completion, early
return, or an admitted failure edge. Passing an existing local or parameter
does not add another retain because that value already has an owner.

When such an owner is created only inside an `if`, switch, or loop region,
HxcIR still gives it function-level identity. Generated C therefore declares
the cleanup-owned carrier at function scope with an inert zero value, then
assigns the real owner at the original expression. This is not a nullable Haxe
value or a second runtime object: it only gives every structured cleanup edge a
legal C identifier. Path-specific HxcIR cleanup still decides whether release
runs.

The selected C representation is private `hxc_bytes_ref *`. Generated public C
interfaces must not expose it as a stable application ABI. The runtime feature
depends on the checked allocator and status slices. String-literal support is
selected because the admitted `Bytes.ofString` boundary copies a validated
UTF-8 view; the Bytes storage itself is still untyped binary data.

Fresh Bytes results are admitted at compiler-known direct, indirect, instance,
constructor, super-constructor, and supported Bytes-operation borrow
boundaries. Unknown calls and APIs still fail closed rather than guessing
whether a callee borrows, retains, or consumes its argument.

## Why the suite has both Haxe and direct C

Run the focused gate with:

```bash
npm run test:bytes-runtime
```

The ordinary-Haxe fixture is executed by pinned Eval and then compiled through
the real Haxe front end, haxe.c lowering, split-project emitter, selective
runtime packager, strict C compiler, and native process. Matching termination
proves that the user-facing Haxe operations agree across those two paths.

The separate C fixture is intentionally not generated by haxe.c. It calls the
runtime boundary directly with a failure-injecting allocator, invalid ranges,
overlapping copies, embedded NUL bytes, and a forced reference-count overflow.
If that baseline were generated from Haxe by the compiler under test, the same
lowering mistake could affect both sides and make the comparison falsely pass.
The direct C is therefore independent runtime evidence, not application code or
a workaround for a missing compiler feature.

The runner also checks reversed typed-module discovery, HxcIR ownership markers,
the exact runtime feature closure, strict warnings, AddressSanitizer and
UndefinedBehaviorSanitizer where Clang is available, selective linked symbols,
negative diagnostics, and `hxc_runtime=none` rejection.
