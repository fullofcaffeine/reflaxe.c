/** Alias used to make explicit Dynamic boundaries readable in the fixture. */
private typedef ReferenceDynamic = Dynamic;

/** One mutable inline record whose Dynamic wrapper owns stable identity. */
private typedef MutablePoint = {
	var x:Int;
}

/** One mutable managed field used to test Dynamic reads and assignments. */
private typedef ManagedHolder = {
	var text:String;
}

/** A fieldless enum exercises exact tag equality without payload reflection. */
private enum Tone {
	Red;
	Blue;
}

/** An exact identity-bearing class used by Dynamic field and method adapters. */
private final class Counter {
	/** Mutable state observed through the same collector-owned identity. */
	public var value:Int;

	/** Create one counter with the requested starting value. */
	public function new(value:Int) {
		this.value = value;
	}

	/** Add an amount and return the updated state. */
	public function bump(amount:Int):Int {
		value += amount;
		return value;
	}
}

/**
	Exercises the first closed-world Dynamic lowering slice.

	Each value enters Dynamic only at an explicit typed boundary. Reads, writes,
	calls, casts, equality, and type values then use ordinary Haxe syntax so the
	reference target and generated C can be compared without a compiler-shaped
	test API.
**/
class Main {
	/** Return one more than the supplied value through a bare function pointer. */
	static function addOne(value:Int):Int
		return value + 1;

	/**
		Force collection while a Dynamic call argument is owned by its caller.

		The result then crosses a second managed-wrapper boundary, so the caller can
		force collection again before unboxing it.
	**/
	static function echoAfterPressure(value:String):String {
		forceCollectionPressure();
		return value;
	}

	/** Exceed the generated collector's deterministic one-mebibyte threshold. */
	static function forceCollectionPressure():Void {
		for (index in 0...50000)
			new Counter(index);
	}

	/** Exercise exact adapters, ownership, source order, and observable results. */
	static function main():Void {
		final integer:ReferenceDynamic = 7;
		final floating:ReferenceDynamic = 2.5;
		final boolean:ReferenceDynamic = true;
		final text:ReferenceDynamic = "caxe";
		final absent:ReferenceDynamic = null;
		final values:ReferenceDynamic = ([3, 5, 8] : Array<Int>);
		final point:ReferenceDynamic = ({x: 4} : MutablePoint);
		final counter:ReferenceDynamic = new Counter(10);
		final tone:ReferenceDynamic = Tone.Blue;
		final callable:ReferenceDynamic = addOne;
		final textCallable:ReferenceDynamic = echoAfterPressure;
		final holder:ReferenceDynamic = ({text: "held"} : ManagedHolder);
		final opaqueType:ReferenceDynamic = Counter;

		// Every managed box above must stay live while temporary allocations force
		// a collection before any value is read.
		forceCollectionPressure();

		point.x = 6;
		counter.value = 11;
		final bumped:Int = counter.bump(4);
		final called:Int = callable(9);
		final callResult:ReferenceDynamic = textCallable(text);
		forceCollectionPressure();
		final fieldRead:ReferenceDynamic = holder.text;
		forceCollectionPressure();
		final replacement:ReferenceDynamic = "changed";
		final assignmentResult:ReferenceDynamic = (holder.text = replacement);
		forceCollectionPressure();
		final restoredValues:Array<Int> = values;
		final first:Int = restoredValues[0];
		final restoredInteger:Int = integer;
		final restoredFloating:Float = floating;
		final restoredBoolean:Bool = boolean;
		final restoredText:String = text;
		final pointX:Int = point.x;
		final counterValue:Int = counter.value;
		final calledText:String = callResult;
		final fieldText:String = fieldRead;
		final assignedText:String = assignmentResult;
		final holderText:String = holder.text;
		final sameInteger:Bool = integer == 7;
		final sameTone:Bool = tone == Tone.Blue;
		final nullEqual:Bool = absent == null;
		final sameType:Bool = opaqueType == Counter;

		Sys.println('${restoredInteger}:${restoredFloating}:${restoredBoolean}:${restoredText}:${first}:${pointX}:${counterValue}:${bumped}:${called}:${calledText}:${fieldText}:${assignedText}:${holderText}:${sameInteger}:${sameTone}:${nullEqual}:${sameType}');
	}
}
