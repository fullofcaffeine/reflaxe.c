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
		// Keep the fieldless value named so this payload-focused fixture does not
		// also depend on the temporary short-circuit cleanup work in haxe_c-djl.6.
		final failed = ValidationFailed;
		improve(copied);
		while (score(original) != 19 || score(copied) != 19 || score(failed) != -1) {}
	}
}
