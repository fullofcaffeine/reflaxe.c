# Retained C view research

## Outcome

haxe.c must continue to reject `Span<T>` and `ConstSpan<T>` class fields.
Those values contain a borrowed pointer and a count. They do not keep their
storage owner alive.

No current product needs a stored view. Therefore, this research does not add
a compiler feature or a new public type.

One narrow source shape is credible for future work. A holder can store its
exact owner and compute a read-only view for one immediate call. The holder
stores no pointer or count.

This shape is an **owner-relative view**. The name means that each use finds the
storage through the current owner reference. The compiler does not preserve a
view between uses.

## Why this question exists

Caxecraft first exposed the question through its water simulation. The
simulation shares world storage with rendering, collision, and level loading.

The natural design did not need a retained view. The simulation owns its private
queue and borrows the shared world for each operation.

Metal-facing Haxe libraries can have a different need. A small object can need
to name another object and expose part of that owner's fixed C storage.

A normal C implementation can store an interior pointer. That pointer points
inside another object instead of at the object's first byte.

This pattern becomes unsafe when the owner dies, moves, or gets copied. It also
becomes unsafe when an unknown native call keeps the pointer.

## Small mental model

The current safe path creates a short borrow from one direct owner:

```text
owner -> fixed storage -> temporary read-only view -> one direct consumer
```

A physical view field creates separate state:

```text
holder -> owner
holder -> cached pointer inside owner
```

The two arrows can stop referring to the same storage. A final field prevents
rebinding, but it does not preserve the relationship.

The proposed future shape keeps one path:

```text
holder -> owner -> fixed storage -> temporary read-only view -> one direct consumer
```

The holder stores only the owner reference. Each use computes a new view.

## Rejected stored field

This source shape remains invalid:

```haxe
import c.CArray;
import c.ConstSpan;
import c.UInt8;

private final class Length4 {}

private final class SampleBuffer {
	public final values:CArray<UInt8, Length4> = CArray.zero(4);

	public function new() {}
}

private final class UnsafeWindow {
	public final owner:SampleBuffer;
	public final view:ConstSpan<UInt8>;

	public function new(owner:SampleBuffer) {
		this.owner = owner;
		this.view = owner.values.constSpan();
	}
}
```

The `view` field stores an interior pointer and a count. The `owner` field proves
reachability only when the compiler gives both objects durable storage.

Reachability is not the complete contract. The pointer must still refer to the
same fixed field at the same address.

These operations can detach a cached pointer from its owner:

- The holder escapes while the owner remains on a finished stack frame.
- An inline owner or parent value gets copied.
- Scalar replacement changes the physical storage.
- A native call keeps the pointer after the call.
- The holder or view crosses an unsupported ABI boundary.
- A future moving collector relocates the owner without repairing interior pointers.

The current collector is non-moving. This fact removes one current failure mode,
but it does not make the other shapes safe.

## Credible future source shape

The following example is a proposal. Current haxe.c does not admit the two-hop
borrow in `view()`.

```haxe
import c.CArray;
import c.ConstSpan;
import c.UInt8;

private final class Length4 {}

private final class SampleBuffer {
	public final values:CArray<UInt8, Length4> = CArray.zero(4);

	public function new() {}
}

private final class SampleWindow {
	public final owner:SampleBuffer;

	public function new(owner:SampleBuffer) {
		this.owner = owner;
	}

	public function view():ConstSpan<UInt8> {
		return owner.values.constSpan();
	}
}

private function readAt(values:ConstSpan<UInt8>, index:Int):UInt8 {
	return values[index];
}

private function use(buffer:SampleBuffer):UInt8 {
	final window = new SampleWindow(buffer);
	return readAt(window.view(), 2);
}
```

`SampleWindow` retains `SampleBuffer`, not `ConstSpan<UInt8>`. The call to
`view()` computes the pointer and count from `owner.values`.

The caller consumes the result immediately. It cannot name, store, return, or
capture that result.

An ordinary method is sufficient. A computed property adds another source rule
without adding safety or user value.

## Exact future contract

A future implementation can start with this complete contract:

- The holder and owner are final, concrete, and non-generic classes.
- The holder has one final field with the exact owner class type.
- The constructor assigns that owner field once and does not publish `this`.
- The owner has one immediate final `CArray<T, N>` storage field.
- The view method has one return path.
- The method returns exactly `owner.storage.constSpan()`.
- The result type is `ConstSpan<T>`. Mutable `Span<T>` remains invalid.
- The caller uses the result in one existing compiler-known direct consumer.
- The view cannot enter a local, field, array, record, enum, closure, global, or return value.
- The holder, owner, and view cannot cross native, exported, reflective, dynamic, or interface boundaries.

"One owner" means one exact field path. Other Haxe references to the owner can
exist. The view always observes the current element values in the owner's fixed
storage.

## Current compiler facts

The current collector uses precise, non-moving managed storage. A live managed
payload keeps one address until the collector reclaims it.

The compiler records exact roots at allocations and calls. These points are
safepoints, where collection can occur after live managed references become
visible to the collector.

A safepoint does not invalidate a pointer into a rooted owner in the current
non-moving collector. The compiler must still keep the exact owner live and
must prevent the callee from retaining the pointer.

Class representation is conservative and uses one representation for each
concrete class. A retained concrete class-reference field already puts both
endpoint classes in the managed graph.

Generated type descriptors trace such managed fields. This graph gives a
possible lifetime proof for a future `SampleWindow.owner` field.

The current borrowed-return contract proves only an immediate field on the
method receiver. It does not prove the two-hop `this.owner.storage` path.

Therefore, current source must continue to fail before C output. The compiler
must not infer the new relationship in the C emitter.

## Rejected alternatives

| Alternative | Result | Reason |
| --- | --- | --- |
| Own or copy the needed storage | Keep | This path is safe and clear when copy cost and value semantics match the use. |
| Borrow the owner for each operation | Keep | This path fits shared storage, including the Caxecraft water world. |
| Store a physical span beside its owner | Reject | Reachability does not prove pointer identity, copying safety, or ABI safety. |
| Store a holder and compute a view per use | Candidate | This path stores only an ordinary owner reference and preserves immediate consumption. |
| Add owner metadata to a span field | Reject | Metadata can request a proof, but application text cannot create a lifetime proof. |
| Add a copyable owner token | Reject | Ordinary Haxe values cannot create an unforgeable lifetime for one object instance. |
| Add general affine lifetimes | Defer | Independently storable or mutable views need a broader source-language contract. |

## Trigger for implementation

Do not implement the candidate for anticipated convenience. Start implementation
only when a concrete, Caxecraft-independent source shape needs this exact path.

The motivating case must show why these simpler choices do not fit:

- The owner lends a view for each operation.
- The holder owns its storage.
- The holder copies a bounded snapshot.
- A typed native handle owns the external resource.

The new case must use the natural safe Haxe API. It must not start from a desired
C layout and work backward into Haxe.

## Required implementation proof

If a real use activates this work, use these stages:

1. Add one focused positive Haxe fixture and the nearest unsafe counterexamples.
2. Prove exact holder, owner, storage, element, length, and call identities during typed lowering.
3. Keep the two-hop relationship explicit in HxcIR.
4. Reprove the relationship in `HxcIRValidator` before C emission.
5. Use the current class-representation graph when it already proves durable ownership.
6. Add a new representation constraint only when the existing graph is insufficient.
7. Materialize the pointer and count only at the direct consumer.
8. Keep all physical span fields and other escape paths on exact negative tests.

The positive proof must include:

- Eval behavior for the portable meaning.
- Deterministic HxcIR with source-linked identities.
- Strict C11 compilation and execution at O0 and O2.
- AddressSanitizer and UndefinedBehaviorSanitizer execution.
- Mutation through another owner alias and a later view read.
- Holder return after the factory scope ends.
- Collector pressure between view uses.
- Generated C with no cached pointer, view allocation, pin, or view runtime.
- Exact runtime and allocation reports.
- Byte-stable split output with readable owner-field traversal.

The negative proof must reject:

- A physical `Span` or `ConstSpan` field.
- A named, returned, captured, or aggregated view.
- Mutable, nullable, generic, inherited, virtual, interface, or dynamic variants.
- Constructor publication before owner initialization.
- Owner or storage rebinding.
- Native, exported, reflective, and unknown-call retention.
- Malformed HxcIR that changes any holder, owner, storage, type, or length identity.

Stop the implementation when any proof depends on source names, unchecked
metadata, emitter pattern matching, or a Caxecraft-specific rule.

## Decision boundary

This research does not admit retained views. It preserves the current
fail-closed behavior and records one bounded design for a future real need.

Independently storable, mutable, offset, multi-owner, native-visible, or
reflective views remain a different problem. They need an owned-view type or a
broader lifetime model.

See [fixed arrays and span-based iteration](span-lowering.md) for the current
implemented contract. See [the precise non-moving collector](gc-runtime.md)
for managed address and root behavior.
