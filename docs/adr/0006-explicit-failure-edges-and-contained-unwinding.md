# ADR 0006: Explicit failure edges and contained C unwinding

- Status: Accepted
- Date: 2026-07-14
- Decision owners: project owner and compiler/runtime maintainers
- Related requirements: HXC-SEM-019, HXC-SEM-020, HXC-RT-008, HXC-RT-009,
  HXC-RT-010

## Context

Haxe permits arbitrary thrown values, nested catch/rethrow, and `finally`.
Strict C has no native exception facility, and `setjmp`/`longjmp` does not run
cleanup or preserve every automatic local value by itself. Exported C code also
needs an ordinary, stable failure contract that can be consumed without sharing
the target runtime.

One representation does not need to serve both internal control flow and the C
ABI. The compiler can retain explicit exceptional edges in its semantic IR and
choose the narrowest legal lowering after cleanup and boundary analysis.

## Decision

### HxcIR always models failure and cleanup explicitly

Throwing operations, calls that may fail, catches, rethrows, and every exit from
a `finally` or ownership region have explicit normal and exceptional successors
in HxcIR. Cleanup actions are ordered, idempotence-checked, and associated with
region checkpoints before C lowering chooses a mechanism.

For a statically closed region, the compiler may lower exceptional edges to
ordinary result/status branches when it can prove that thrown-value matching,
rethrow, `finally`, return/break/continue behavior, cleanup order, and observable
stack behavior are unchanged. This is the preferred runtime-free lowering and
is reported per function/region. `c.Result<T,E>` and explicit status-returning
APIs always use this ordinary C control-flow contract.

### General portable exceptions use a contained runtime slice

Reachable general Haxe throw/catch/finally behavior that cannot be proven safe
for result lowering selects the separate `exception` runtime feature. Its
strict-C11 implementation uses an encapsulated exception-frame chain and
`setjmp`/`longjmp`:

- compiler-owned frames hold the jump buffer, previous frame, catch metadata,
  rooted thrown value, and cleanup-stack checkpoint;
- a throw finds the target frame, executes registered cleanups down to its
  checkpoint exactly once, keeps the payload live, and only then transfers;
- `finally` is compiled as an explicit cleanup/control-flow region so normal
  completion, return, loop exits, throw, and rethrow each execute it once;
- state live across `setjmp` is stored in explicit frame slots or otherwise
  emitted according to C's `volatile` rules; lowering never reads an
  indeterminate automatic local after `longjmp`;
- generated exception-capable regions do not rely on variable-length-array or
  foreign-frame lifetime after a non-local transfer;
- frames are thread-local and a transfer may target only an active frame on the
  same thread.

No C++ exception mechanism is used by generated C. A C++ shim catches C++
exceptions on its side and translates them into the declared C status contract
before calling or returning through generated C.

### Non-local transfer never crosses a native boundary

An exception frame, thrown Haxe value, or `longjmp` token is never part of a
public header or callable C ABI. Generated export wrappers catch every Haxe
exception before returning to C and translate it to the configured status/error
result. Callback trampolines do the same before returning into foreign code.
Imported C that may call back into Haxe must use such a trampoline and boundary
frame.

The compiler rejects a boundary for which it cannot generate a total
translation. A `longjmp` may not cross an arbitrary foreign C frame, public
export, callback return, thread boundary, signal handler, or a compiler-owned
frame that has already returned. Error details and owned payloads cross an ABI
only through the separately versioned, thread-safe error/ownership contract.

`portable + auto` may select the exception slice after result lowering is
ineligible. The default metal `minimal` policy rejects general exception
machinery; metal code uses typed results/statuses unless the user explicitly
chooses a policy that admits the reported slice. `hxc_runtime=none` accepts only
regions proven to lower without `hxrt`.

## Consequences

- Ordinary successful code and closed failure graphs can remain readable C
  branches with no exception runtime.
- Full portable Haxe exceptions remain available through one isolated,
  testable strict-C mechanism.
- Cleanup correctness is an IR property rather than an accidental side effect
  of `longjmp`.
- Exported libraries expose conventional status/error APIs and remain usable by
  runtimes that know nothing about Haxe exceptions.
- The compiler must test C automatic-local, thread, callback, nested-finally,
  rethrow, partially initialized value, and allocator-failure cases explicitly.

## Rejected alternatives

- Exposing `setjmp`/`longjmp` in generated public APIs: it couples consumers to
  target-private frames and permits undefined cross-boundary transfer.
- Using C++ exceptions: generated output and ABI are C, and arbitrary C++
  unwinding cannot be assumed.
- Lowering every throw to status returns unconditionally: open call graphs,
  arbitrary thrown values, callbacks, and nested cleanup can make that rewrite
  semantically incomplete or unreasonably invasive.
- Using `longjmp` without a compiler-owned cleanup stack: owned resources and
  `finally` blocks would leak or execute in the wrong order.
- Allowing exceptions to escape callbacks or exports: foreign frames cannot
  safely participate in the target's unwind protocol.
