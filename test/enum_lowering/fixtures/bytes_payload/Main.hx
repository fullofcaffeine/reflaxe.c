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

/** Closed selector used to reproduce Haxe's switch-generated flow carrier. */
enum ValidationChoice {
	ChooseFresh;
	ChooseBorrowed;
	ChooseFailed;
}

/** Unmanaged result used to prove the same carrier shape needs no fake enum value. */
enum ValidationFamily {
	FreshFamily;
	BorrowedFamily;
	FailedFamily;
}

/**
 * Three constructors keep ignored-payload switches in Haxe's full tag-switch form.
 *
 * `SwitchBytes` makes the whole enum managed even when a particular match does
 * not bind its payload. This complements `ValidationResult`, whose two cases may
 * be shortened by Haxe into one tag comparison.
 */
enum WideSwitchResult {
	SwitchEmpty;
	SwitchBytes(canonical:Bytes);
	SwitchCode(code:Int);
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

	/**
	 * Return a fresh enum owner for both the empty and managed-payload cases.
	 *
	 * The caller deliberately switches on this call directly. No source local
	 * owns the result, so haxe.c must keep the temporary alive while a case reads
	 * its payload and then destroy it at the switch boundary.
	 */
	static function validationForSwitch(passes:Bool):ValidationResult
		return passes ? validate() : ValidationFailed;

	/** Create a fresh three-case owner for statement, value, and early-exit switches. */
	static function validationForWideSwitch(choice:Int):WideSwitchResult {
		if (choice < 0)
			return SwitchEmpty;
		if (choice == 0)
			return SwitchCode(7);
		final canonical = Bytes.alloc(1);
		canonical.set(0, 15);
		return SwitchBytes(canonical);
	}

	/**
	 * Match a fresh enum call result, then continue after the switch.
	 *
	 * The payload branch reads the owned Bytes value before the switch releases
	 * the enclosing enum. The payload-free branch proves cleanup does not inspect
	 * an inactive union member.
	 */
	static function scoreFreshSwitchSubject(passes:Bool):Int {
		var result = -2;
		switch validationForSwitch(passes) {
			case ValidationFailed:
				result = -1;
			case ValidationPassed(canonical):
				result = canonical.get(0) + canonical.get(1) + canonical.get(2);
		}
		return result;
	}

	/**
	 * Match the same fresh value while deliberately ignoring every payload.
	 *
	 * Haxe may optimize this source switch into a direct enum-tag test because no
	 * arm needs a payload variable. That shorter typed-tree shape still consumes
	 * a fresh managed enum owner and therefore needs the same bounded cleanup as
	 * the payload-reading switch above.
	 */
	static function scoreIgnoredFreshSwitchSubject(passes:Bool):Int {
		var result = -2;
		switch validationForSwitch(passes) {
			case ValidationFailed:
				result = -1;
			case ValidationPassed(_):
				result = 15;
		}
		return result;
	}

	/** Continue after a full three-case switch whose fresh payloads are ignored. */
	static function scoreWideFreshSwitchSubject(choice:Int):Int {
		var result = -2;
		switch validationForWideSwitch(choice) {
			case SwitchEmpty:
				result = -1;
			case SwitchBytes(_):
				result = 15;
			case SwitchCode(_):
				result = 7;
		}
		return result;
	}

	/** Preserve an `Int` call boundary so its argument remains a value expression. */
	static function observeInt(value:Int):Int {
		order = order;
		return value;
	}

	/** Produce a call argument through value-switch lowering over the same fresh owner. */
	static function scoreWideFreshValueSwitchSubject(choice:Int):Int
		return observeInt(switch validationForWideSwitch(choice) {
			case SwitchEmpty: -1;
			case SwitchBytes(_): 15;
			case SwitchCode(_): 7;
		});

	/**
	 * Ask one Boolean question about a fresh two-case enum.
	 *
	 * Haxe may reduce this wildcard match to `enumIndex(value) == tag`. The enum
	 * result must still be destroyed after the tag Boolean has been copied.
	 */
	static function freshSwitchSubjectPassed(passes:Bool):Bool
		return switch validationForSwitch(passes) {
			case ValidationPassed(_): true;
			case _: false;
		};

	/** Prove every early-return arm destroys the full switch subject once. */
	static function scoreWideFreshSwitchSubjectEarly(choice:Int):Int {
		switch validationForWideSwitch(choice) {
			case SwitchEmpty:
				return -1;
			case SwitchBytes(_):
				return 15;
			case SwitchCode(_):
				return 7;
		}
	}

	/**
	 * Return from both arms while a fresh switch-subject owner is still live.
	 *
	 * These early exits cannot pass through the normal switch join. Their HxcIR
	 * return edges must therefore run the same enum cleanup exactly once.
	 */
	static function scoreFreshSwitchSubjectEarly(passes:Bool):Int {
		switch validationForSwitch(passes) {
			case ValidationFailed:
				return -1;
			case ValidationPassed(canonical):
				return canonical.get(0) + canonical.get(1) + canonical.get(2);
		}
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

	/**
	 * Join managed values through Haxe's block-valued `if` carrier.
	 *
	 * The extra assignment makes each arm a block rather than a direct ternary
	 * operand. Haxe's typed tree represents the result as an empty compiler
	 * local, exhaustive arm assignments, and one later read. Haxe.c must apply
	 * the same move-fresh/retain-borrowed protocol as it does for a direct `if`.
	 */
	static function chooseBlockLocal(useFresh:Bool, borrowed:ValidationResult):ValidationResult {
		order = 4;
		final selected = if (useFresh) {
			order = order * 10 + 1;
			validate();
		} else {
			order = order * 10 + 2;
			borrowed;
		};
		while (order != (useFresh ? 41 : 42)) {}
		return selected;
	}

	/** Pass a block-valued managed carrier directly to a borrowing call. */
	static function chooseBlockArgument(useFresh:Bool, borrowed:ValidationResult):ValidationResult
		return copy(if (useFresh) {
			order = order;
			validate();
		} else {
			order = order;
			borrowed;
		});

	/** Transfer a block-valued managed carrier directly to the caller. */
	static function chooseBlockReturn(useFresh:Bool, borrowed:ValidationResult):ValidationResult
		return if (useFresh) {
			order = order;
			validate();
		} else {
			order = order;
			borrowed;
		};

	/** Nest two block-valued joins without losing either carrier's owner. */
	static function chooseBlockNested(outer:Bool, inner:Bool, borrowed:ValidationResult):ValidationResult
		return if (outer) {
			order = order;
			if (inner) {
				order = order;
				validate();
			} else {
				order = order;
				borrowed;
			};
		} else {
			order = order;
			ValidationFailed;
		};

	/** Select a managed result through the same switch shape used by Caxecraft. */
	static function chooseBlockSwitch(choice:ValidationChoice, borrowed:ValidationResult):ValidationResult {
		final selected = switch choice {
			case ChooseFresh:
				order = order;
				validate();
			case ChooseBorrowed:
				order = order;
				borrowed;
			case ChooseFailed:
				order = order;
				ValidationFailed;
		};
		return selected;
	}

	/**
	 * Select an unmanaged enum without inventing a zero-valued constructor.
	 *
	 * Haxe emits the same empty-local/arm-assignment/read graph as it does for
	 * the managed switch above. The existing uninitialized-carrier verifier, not
	 * a fabricated enum constant, proves that every normal path assigns it.
	 */
	static function familyForChoice(choice:ValidationChoice):ValidationFamily {
		final family = switch choice {
			case ChooseFresh: FreshFamily;
			case ChooseBorrowed: BorrowedFamily;
			case ChooseFailed: FailedFamily;
		};
		return family;
	}

	/**
	 * Keep primitive switch carriers outside the managed-enum classification.
	 *
	 * Haxe gives this switch the same empty-local shape as the enum cases above.
	 * The compiler must first recognize that `Int` is not an enum, then use its
	 * ordinary scalar initialization path without inspecting enum lifecycle data.
	 */
	static function intForChoice(choice:ValidationChoice):Int {
		final value = switch choice {
			case ChooseFresh: 11;
			case ChooseBorrowed: 22;
			case ChooseFailed: 33;
		};
		return value;
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

	/**
	 * Replace one owned enum local along ordinary statement control flow.
	 *
	 * This is the reduced form of an application selector that starts with an
	 * empty case, finds better candidates in a loop, and returns the last choice.
	 * Each assignment must acquire the replacement before it destroys the old
	 * active payload. The final return transfers the local's one remaining owner.
	 */
	static function replaceSelection(selectFirst:Bool, selectSecond:Bool):ValidationResult {
		var selected:ValidationResult = ValidationFailed;
		if (selectFirst)
			selected = validate();
		if (selectSecond)
			selected = validate();
		return selected;
	}

	/** Replace a fresh local with a borrowed alias without invalidating either owner. */
	static function replaceSelectionWithBorrowed(borrowed:ValidationResult):ValidationResult {
		var selected = validate();
		selected = borrowed;
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
		final blockBorrowed = chooseBlockLocal(false, original);
		final blockFresh = chooseBlockLocal(true, original);
		final blockArgument = chooseBlockArgument(false, original);
		final blockReturn = chooseBlockReturn(true, original);
		final blockNestedBorrowed = chooseBlockNested(true, false, original);
		final blockNestedFresh = chooseBlockNested(true, true, original);
		final blockNestedFailed = chooseBlockNested(false, true, original);
		final blockSwitchBorrowed = chooseBlockSwitch(ChooseBorrowed, original);
		final blockSwitchFresh = chooseBlockSwitch(ChooseFresh, original);
		final blockSwitchFailed = chooseBlockSwitch(ChooseFailed, original);
		final replacementEmpty = replaceSelection(false, false);
		final replacementFirst = replaceSelection(true, false);
		final replacementSecond = replaceSelection(true, true);
		final replacementBorrowed = replaceSelectionWithBorrowed(original);
		final failed = ValidationFailed;
		improve(copied);
		while (score(original) != 19
			|| score(copied) != 19
			|| score(borrowedJoin) != 19
			|| score(freshJoin) != 15
			|| score(argumentJoin) != 19
			|| score(nestedBorrowed) != 19
			|| score(nestedFresh) != 15
			|| score(nestedFailed) != -1
			|| score(orderedBorrowed) != 19
			|| score(orderedFresh) != 15
			|| score(blockBorrowed) != 19
			|| score(blockFresh) != 15
			|| score(blockArgument) != 19
			|| score(blockReturn) != 15
			|| score(blockNestedBorrowed) != 19
			|| score(blockNestedFresh) != 15
			|| score(blockNestedFailed) != -1
			|| score(blockSwitchBorrowed) != 19
			|| score(blockSwitchFresh) != 15
			|| score(blockSwitchFailed) != -1
			|| score(replacementEmpty) != -1
			|| score(replacementFirst) != 15
			|| score(replacementSecond) != 15
			|| score(replacementBorrowed) != 19
			|| familyForChoice(ChooseFresh) != FreshFamily
			|| familyForChoice(ChooseBorrowed) != BorrowedFamily
			|| familyForChoice(ChooseFailed) != FailedFamily
			|| intForChoice(ChooseFresh) != 11
			|| intForChoice(ChooseBorrowed) != 22
			|| intForChoice(ChooseFailed) != 33
			|| scoreFreshSwitchSubject(false) != -1
			|| scoreFreshSwitchSubject(true) != 15
			|| scoreIgnoredFreshSwitchSubject(false) != -1
			|| scoreIgnoredFreshSwitchSubject(true) != 15
			|| scoreWideFreshSwitchSubject(-1) != -1
			|| scoreWideFreshSwitchSubject(0) != 7
			|| scoreWideFreshSwitchSubject(1) != 15
			|| scoreWideFreshValueSwitchSubject(-1) != -1
			|| scoreWideFreshValueSwitchSubject(0) != 7
			|| scoreWideFreshValueSwitchSubject(1) != 15
			|| freshSwitchSubjectPassed(false)
			|| !freshSwitchSubjectPassed(true)
			|| scoreWideFreshSwitchSubjectEarly(-1) != -1
			|| scoreWideFreshSwitchSubjectEarly(0) != 7
			|| scoreWideFreshSwitchSubjectEarly(1) != 15
			|| scoreFreshSwitchSubjectEarly(false) != -1
			|| scoreFreshSwitchSubjectEarly(true) != 15
			|| score(failed) != -1) {}
	}
}
