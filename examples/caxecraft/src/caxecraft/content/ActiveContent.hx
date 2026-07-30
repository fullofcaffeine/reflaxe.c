package caxecraft.content;

import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LoadedContentGeneration.LoadedContentGenerationTrace;
import caxecraft.domain.GameSession;

/**
 * Owns the one content generation visible to the main loop.
 *
 * Candidate construction never receives this owner. `publish` first rejects an
 * out-of-order candidate, constructs its result value, and then performs one
 * non-failing reference swap. The previous generation becomes collectible once
 * call-bounded readers release it; no rollback mutation log is needed.
 */
/** Why a complete candidate was not selected as the active generation. */
enum ContentPublicationError {
	/** An older or duplicate request completed after the current generation. */
	OutOfOrderGeneration(candidate:Int, active:Int);
}

/** Result of one prechecked generation publication request. */
enum ContentPublicationResult {
	/**
	 * The candidate became active and the prior generation left the owner.
	 *
	 * Only identities escape. Returning the mutable retired generation would
	 * encourage callers to retain stale world state after the frame boundary.
	 */
	ContentPublished(retired:ContentGenerationId, active:ContentGenerationId);

	/** The active owner and its complete simulation remain unchanged. */
	ContentPublicationRejected(error:ContentPublicationError);
}

/**
 * Main-loop owner for one currently playable content generation.
 *
 * A stateful class makes the publication point explicit: every frame asks this
 * owner for its current session, and only this owner may replace the generation
 * reference. The class invokes no callbacks and performs no allocation after
 * the swap.
 */
final class ActiveContent {
	var active:LoadedContentGeneration;
	var completedPublications:Int = 0;

	/** Begin with one already complete generation; no empty live state exists. */
	public function new(initial:LoadedContentGeneration) {
		active = initial;
	}

	/** Identity of the generation future main-loop operations will observe. */
	public inline function generationId():ContentGenerationId
		return active.generationId();

	/**
	 * Borrow the current session for one main-loop operation.
	 *
	 * Do not retain this reference across `publish`; ask the owner again at the
	 * next frame or command boundary.
	 */
	public inline function session():GameSession
		return active.session();

	/** Capture target-neutral state from the currently selected generation. */
	public inline function semanticTrace():LoadedContentGenerationTrace
		return active.semanticTrace();

	/** Number of successful owner swaps after the required initial generation. */
	public inline function publicationCount():Int
		return completedPublications;

	/**
	 * Select one newer complete candidate through a single commit assignment.
	 *
	 * Ordering validation and result construction finish before `active` changes.
	 * After the assignment, only an integer increment remains; neither operation
	 * can reject or invoke application code.
	 */
	public function publish(candidate:LoadedContentGeneration):ContentPublicationResult {
		final candidateId = candidate.generationId();
		final activeId = active.generationId();
		if (candidateId.value() <= activeId.value())
			return ContentPublicationRejected(OutOfOrderGeneration(candidateId.value(), activeId.value()));
		final result = ContentPublished(activeId, candidateId);
		active = candidate;
		completedPublications++;
		return result;
	}
}
