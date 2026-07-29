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
- `Bytes.ofString` when its input is an admitted immutable Haxe String view and
  the optional encoding is absent or `null`. The expression may be a source
  literal, local, alias, parameter, or an admitted operation that creates a
  fresh String, including concatenation and `Array<String>.join`;
- `getString(position, length)` and `toString()`, using UTF-8. The selected
  range must be in bounds and contain complete, well-formed UTF-8. The returned
  immutable String owns its own copy, so later mutation of the Bytes value
  cannot alter text that has already been published.

Assignment shares the same mutable Bytes value. `sub` is different: it creates
an independent copy. `set` and `fill` keep the low eight bits of the supplied
Haxe `Int`. Every position and range is checked before C performs pointer
arithmetic, and `blit` preserves the source bytes when two ranges of the same
value overlap.

This is a deliberately bounded implementation, not a claim that all of
`haxe.io.Bytes`, `BytesBuffer`, `Input`, or `Output` is complete. Unsupported
methods fail at their Haxe source position with `HXC1001` and leave no plausible
generated project.

The optional `getString` encoding may be omitted, `null`, or exactly
`Encoding.UTF8`. `Encoding.RawNative` is intentionally rejected rather than
given a misleading cross-platform meaning: Haxe defines it in terms of the
host's native encoding, while haxe.c does not yet have a reviewed platform
encoding contract.

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
interfaces must not expose it as a stable application ABI. The Bytes runtime
feature depends on the checked allocator, status, and String-view carrier
slices. Operations that build a runtime String select their own narrow String
dependencies; `Bytes.ofString` does not select a generic text runtime merely
because it receives a String.

One deliberately narrow interop operation can lend that private allocation to
a synchronous C text function. `c.CStringBufferRef.to(bytes)` lowers to
`hxc_bytes_ref_borrow_mutable_cstring`, which accepts only a live, non-empty
Bytes value whose allocation already contains a NUL byte. It publishes a
`char *` only after those checks pass. The pointer has no owner of its own:
schema-21 HxcIR requires the checked borrow as its producer and exactly one
direct native-call argument as its consumer, in the same basic block. Storage,
return, forwarding, indirect calls, and a second consumer are validation
errors before C emission.

This check prevents an unbounded C string read, but it does not know how a
particular library interprets a separate capacity integer. The typed wrapper
still must pass the real allocation length, reserve the final NUL, and validate
the edited bytes as UTF-8 where its user-facing API promises text. That
division is intentional: hxrt owns memory validity, HxcIR owns the one-call
lifetime, and the library binding owns its API-specific pointer/capacity
relationship.

`Bytes.ofString` accepts the length-delimited String view by value and copies
its logical bytes immediately. It therefore preserves non-ASCII scalars, empty
text, and embedded NUL without needing a C-string terminator. The resulting
Bytes storage is independent mutable binary data and never retains or points
into the String.

That copy and the source lifetime are separate decisions. A literal, local,
alias, or parameter already has storage that outlives the call, so the compiler
adds no String retain or temporary owner. A fresh expression such as
`Bytes.ofString(left + right)` already owns its newly allocated String but has
no source local to hold that owner. HxcIR moves it into one hidden
cleanup-capable local, lets the Bytes call borrow the local, and releases the
String exactly once after a successful copy. The generated profile's
allocation-failure policy is a checked terminal abort; the independent C
contract additionally forces a recoverable Bytes allocation failure and proves
that the caller's managed String remains valid and independently releasable.
This admission does not imply support for an unrelated String producer:
`value.toUpperCase()` still fails at its own unimplemented String boundary.

The reverse conversion selects a separate `bytes-string` runtime feature.
`getString` passes its byte position and byte length to one checked runtime
operation; `toString` uses the same operation with position zero and the
buffer's complete length. The operation validates bounds before pointer
arithmetic, validates UTF-8 before publishing a result, and moves the fresh
String owner into the caller's normal HxcIR cleanup plan. A malformed range
therefore cannot become plausible text, and a successful result remains valid
after the source Bytes owner changes or is released.

That ownership also crosses an ordinary Haxe function boundary. For example,
`function snapshot(bytes:Bytes):String return bytes.toString()` returns a new
immutable String owner rather than a pointer into the function's temporary
storage. Before haxe.c fixes function signatures, it scans reachable bodies for
the exact Bytes decoding operations. The helper and its callers therefore
agree on the managed String return convention from the start: the helper moves
the fresh owner out, the caller receives it into a cleanup-capable local, and
the caller releases it after its last use. This early decision matters because
C cannot safely change a function's return representation after call sites
have already been lowered.

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

The runner also checks reversed typed-module discovery independently for split
and unity projects, HxcIR ownership markers for borrowed and fresh String
sources and fresh decoded String results, including helper returns whose caller
mutates the source Bytes afterward. It checks valid ASCII and multibyte UTF-8
snapshots, malformed UTF-8 and range failures, the exact runtime feature
closure, strict C11 execution at `-O0` and `-O2`, C++17 header consumption,
AddressSanitizer and UndefinedBehaviorSanitizer on both the direct runtime
contract and generated split projects where Clang is available, selective
linked symbols, forced allocation rollback, negative diagnostics, and
`hxc_runtime=none` rejection.
