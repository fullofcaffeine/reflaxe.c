package;

import haxe.io.Bytes;

/**
 * Exercises one private Haxe enum whose active case owns a `Bytes` value.
 *
 * `Bytes` has shared mutable identity: copying the enum must retain the same
 * byte buffer, projecting the payload must keep it alive for the branch, and
 * destroying either enum copy must release exactly one owner. The silent
 * program stops only when construction, return, copying, projection, mutation,
 * and cleanup agree with Haxe Eval.
 */
enum ValidationResult {
	ValidationFailed;
	ValidationPassed(canonical:Bytes);
}

/**
 * Drives the ordinary-Haxe behavior that the generated C must preserve.
 *
 * This is intentionally application-shaped rather than a compiler intrinsic:
 * it is the same private result-enum pattern used by Caxecraft's editor
 * validation boundary.
 */
final class Main {
	/** Small observable trace used only to prove conditional evaluation order. */
	static var order:Int = 0;

	/** Build a successful result and transfer the fresh buffer into its payload. */
	static function validate():ValidationResult {
		final canonical = Bytes.alloc(3);
		canonical.set(0, 3);
		canonical.set(1, 5);
		canonical.set(2, 7);
		return ValidationPassed(canonical);
	}

	/** Return another owning enum value while preserving shared `Bytes` identity. */
	static function copy(value:ValidationResult):ValidationResult
		return value;

	/**
	 * Join a fresh result and a caller-owned result through one local.
	 *
	 * The generated C must move the fresh branch but retain the borrowed branch;
	 * treating both branches alike would either leak or free shared bytes early.
	 */
	static function choose(useFresh:Bool, borrowed:ValidationResult):ValidationResult {
		final selected = useFresh ? validate() : borrowed;
		return selected;
	}

	/** Exercise the same ownership join when the conditional is a call argument. */
	static function chooseArgument(useFresh:Bool, borrowed:ValidationResult):ValidationResult
		return copy(useFresh ? validate() : borrowed);

	/** Prove nested joins preserve one owner without flattening branch order. */
	static function chooseNested(outer:Bool, inner:Bool, borrowed:ValidationResult):ValidationResult {
		final selected = outer ? (inner ? validate() : borrowed) : ValidationFailed;
		return selected;
	}

	/** Record condition evaluation before returning its Boolean result. */
	static function conditionWithOrder(value:Bool):Bool {
		order = 9;
		return value;
	}

	/** Record the fresh arm and then construct its one owned result. */
	static function freshWithOrder():ValidationResult {
		order = order * 10 + 1;
		return validate();
	}

	/** Record the borrowed arm and return the caller-owned value unchanged. */
	static function borrowedWithOrder(borrowed:ValidationResult):ValidationResult {
		order = order * 10 + 2;
		return borrowed;
	}

	/**
	 * Make condition and selected-arm evaluation observable without printing.
	 *
	 * A wrong eager or reordered lowering produces a different byte and
	 * deliberately loops, so both Eval and native executions detect it.
	 */
	static function chooseWithOrder(useFresh:Bool, borrowed:ValidationResult):ValidationResult {
		final selected = conditionWithOrder(useFresh) ? freshWithOrder() : borrowedWithOrder(borrowed);
		final expectedOrder = useFresh ? 91 : 92;
		while (order != expectedOrder) {}
		return selected;
	}

	/** Mutate the projected shared buffer without reading an inactive union arm. */
	static function improve(value:ValidationResult):Void {
		switch value {
			case ValidationFailed:
			case ValidationPassed(canonical):
				canonical.set(1, canonical.get(1) + 4);
		}
	}

	/** Read the active payload through an exhaustive, checked enum match. */
	static function score(value:ValidationResult):Int
		return switch value {
			case ValidationFailed: -1;
			case ValidationPassed(canonical): canonical.get(0) + canonical.get(1) + canonical.get(2);
		};

	/** Keep Eval and native C silent; a semantic mismatch remains observable as a loop. */
	static function main():Void {
		final original = validate();
		final copied = copy(original);
		final borrowedJoin = choose(false, original);
		final freshJoin = choose(true, original);
		final argumentJoin = chooseArgument(false, original);
		final nestedBorrowed = chooseNested(true, false, original);
		final nestedFresh = chooseNested(true, true, original);
		final nestedFailed = chooseNested(false, true, original);
		final orderedBorrowed = chooseWithOrder(false, original);
		final orderedFresh = chooseWithOrder(true, original);
		final failed = ValidationFailed;
		improve(copied);
		while (score(original) != 19 || score(copied) != 19 || score(borrowedJoin) != 19 || score(freshJoin) != 15 || score(argumentJoin) != 19
			|| score(nestedBorrowed) != 19 || score(nestedFresh) != 15 || score(nestedFailed) != -1 || score(orderedBorrowed) != 19
			|| score(orderedFresh) != 15 || score(failed) != -1) {}
	}
}
