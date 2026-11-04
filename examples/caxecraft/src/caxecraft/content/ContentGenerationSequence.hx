package caxecraft.content;

import caxecraft.content.LoadedContentGeneration.ContentGenerationId;

/**
 * Issues every transient and published content identity for one game process.
 *
 * Campaign preloads, ordinary transitions, and editor Test Play all pass
 * through this owner. An attempted load consumes its identity even when it is
 * rejected, so a renderer cache can never mistake later content for an older
 * candidate. After the positive `Int` range is exhausted, `allocate` returns
 * the invalid zero identity; candidate construction then fails closed instead
 * of wrapping to an identity that was already observed.
 *
 * This is a stateful class because uniqueness depends on one shared mutable
 * process lifetime. A stateless helper or caller-owned counter would recreate
 * the competing namespaces this owner removes.
 */
final class ContentGenerationSequence {
	static inline final MAX_SEQUENCE:Int = 2147483647;

	var nextSequence:Int;

	/** Start the process-owned sequence at the first valid generation. */
	public function new() {
		nextSequence = 1;
	}

	/**
	 * Consume and return one identity, or invalid zero after exhaustion.
	 *
	 * The zero sentinel cannot enter a `LoadedContentGeneration`: its existing
	 * constructor validation rejects non-positive identities before allocation.
	 */
	public function allocate():ContentGenerationId {
		final sequence = nextSequence;
		if (sequence == MAX_SEQUENCE)
			nextSequence = 0;
		else if (sequence > 0)
			nextSequence = sequence + 1;
		return ContentGenerationId.fromSequence(sequence);
	}
}
