/**
 * Groups two ordinary managed Haxe objects without copying their identities.
 *
 * The record itself remains a direct C value. Its fields are typed pointers to
 * collector-managed objects, so HxcIR can keep both objects rooted for as long
 * as the record is live.
 */
typedef ManagedReferenceRecord = {
	final first:ManagedReferenceValue;
	final second:ManagedReferenceValue;
}

/** A small managed class used to prove class-reference record fields. */
final class ManagedReferenceValue {
	public final value:Int;

	/** Store one observable value so the native executable can check both refs. */
	public function new(value:Int) {
		this.value = value;
	}
}

/** Executes the class-reference record contract as a generated C program. */
final class Main {
	/** Return a class reference so whole-program analysis selects managed storage. */
	static function managedReference(value:Int):ManagedReferenceValue
		return new ManagedReferenceValue(value);

	/** Preserve the record as one typed value across a direct call boundary. */
	static function groupReferences(first:ManagedReferenceValue, second:ManagedReferenceValue):ManagedReferenceRecord
		return {first: first, second: second};

	/** Read two non-null managed class references through one local record. */
	static function managedReferenceRecordSum():Int {
		final references = groupReferences(managedReference(19), managedReference(23));
		return references.first.value + references.second.value;
	}

	/** Keep running only if record construction or either reference drifts. */
	static function main():Void {
		while (managedReferenceRecordSum() != 42) {}
	}
}
