/**
	Defines the call-bounded class and interface contracts exercised by this fixture.

	The source deliberately passes fresh class instances directly to known
	synchronous calls. haxe.c must provide the unnamed object storage and lifetime;
	the Haxe program should not need a temporary local merely because C requires an
	addressable struct.
**/
interface ScoreResolver {
	/** Read the implementation's score while its caller-owned object is alive. */
	function score(seed:Int):Int;
}

/**
	Stores one managed Array so successful and failed construction both need cleanup.

	The Boolean branch makes the constructor genuinely fallible even though the
	successful runtime oracle passes `false`. This gives the generated HxcIR an
	explicit partial-construction edge to validate.
**/
final class OffsetResolver implements ScoreResolver {
	final offsets:Array<Int>;

	/** Retain the offset before optionally throwing from the constructor. */
	public function new(offset:Int, shouldFail:Bool) {
		offsets = [offset];
		if (shouldFail)
			throw 99;
	}

	/** Combine the retained offset with one caller-supplied seed. */
	public function score(seed:Int):Int
		return offsets[0] + seed;
}

/**
	Owns an instance method used to prove the same argument rule on direct methods.

	The class is final, so haxe.c knows the exact method body and may prove that its
	interface parameter remains borrowed only for this call. An overridable or
	indirect call would not provide that ownership proof.
**/
final class ScoreSink {
	/** Create the stateless direct-call owner without allocating managed state. */
	public function new() {}

	/** Invoke a fresh resolver without storing its interface value. */
	public function read(resolver:ScoreResolver, seed:Int):Int
		return resolver.score(seed);
}

/**
	Forwards one borrowed object through two exact instance-method bodies.

	Both methods are known because this class is final. Neither stores, returns,
	throws, captures, or forwards the reference to an unknown target, so the
	caller's automatic object remains alive for the complete synchronous chain.
**/
final class ForwardingSink {
	/** Create the stateless forwarding owner. */
	public function new() {}

	/** Forward the caller-owned object to the exact private helper below. */
	public function read(resolver:OffsetResolver, seed:Int):Int
		return readDirect(resolver, seed);

	/** Read the borrow without retaining it beyond this method call. */
	function readDirect(resolver:OffsetResolver, seed:Int):Int
		return resolver.score(seed);
}

/** Executes class, interface, managed-neighbor, and direct-instance call shapes. */
final class Main {
	/**
		Accept a concrete class as a call-only borrow.

		The Arrays on either side make evaluation order observable in generated C:
		the left owner exists before the resolver is constructed, while the right
		owner is created afterward. All three temporary owners end after this call.
	**/
	static function readConcrete(left:Array<Int>, resolver:OffsetResolver, right:Array<Int>):Int
		return left[0] + resolver.score(0) + right[0];

	/** Accept the same object through its by-value interface object/table pair. */
	static function readInterface(left:Array<Int>, resolver:ScoreResolver, right:Array<Int>):Int
		return left[0] + resolver.score(0) + right[0];

	/** Keep running only if native C differs from the pinned Haxe Eval oracle. */
	static function main():Void {
		final concrete = readConcrete([1], new OffsetResolver(40, false), [1]);
		final throughInterface = readInterface([1], new OffsetResolver(40, false), true ? [1] : [100]);
		final sink = new ScoreSink();
		final throughMethod = sink.read(new OffsetResolver(40, false), 2);
		final forwardedValue = new OffsetResolver(40, false);
		final forwardingSink = new ForwardingSink();
		final throughForwarding = forwardingSink.read(forwardedValue, 2);
		while (concrete != 42 || throughInterface != 42 || throughMethod != 42 || throughForwarding != 42) {}
	}
}
