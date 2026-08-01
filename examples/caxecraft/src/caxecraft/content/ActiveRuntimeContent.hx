package caxecraft.content;

import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentGeneration;

/**
 * Owns the one complete runtime-content generation visible to the product.
 *
 * Candidate construction receives no reference to this module. Product code
 * asks this owner for the registry, UI catalog, session, trace, or receipt that
 * is current when an operation begins. Those borrowed values are not live
 * handles: code that retains one across publication still sees the retired
 * generation. Publication validates ordering and constructs its result before
 * one non-failing owner swap.
 */
/** Why one complete candidate could not replace the active generation. */
enum RuntimeContentPublicationError {
	/** An older or duplicate complete candidate finished after the active one. */
	OutOfOrderRuntimeContent(candidate:Int, active:Int);
}

/** Result of one prechecked complete runtime-content publication request. */
enum RuntimeContentPublicationResult {
	/** Every active view now comes from the selected complete generation. */
	RuntimeContentPublished(retired:ContentGenerationId, active:ContentGenerationId);

	/** The previously active complete generation remains unchanged. */
	RuntimeContentPublicationRejected(error:RuntimeContentPublicationError);
}

/**
 * Main-loop owner for one coherent registry, catalog, receipt, and simulation.
 *
 * This separate class preserves `ActiveContent` as the lower map-only owner.
 * A wrapper is preferable to widening that class because schema and map tests
 * must remain independently constructible; revisit the separation only if the
 * lower map capability itself begins requiring pack/UI publication.
 */
final class ActiveRuntimeContent {
	var active:RuntimeContentGeneration;
	var completedPublications:Int = 0;

	/** Begin with one fully verified generation; no empty live state exists. */
	public function new(initial:RuntimeContentGeneration)
		active = initial;

	/**
	 * Borrow one coherent complete generation for a product operation.
	 *
	 * Derive the registry, catalog, level/session, and receipt from this one
	 * return value. Reacquire it after publication instead of mixing a retained
	 * retired view with a newly selected view.
	 */
	public inline function generation():RuntimeContentGeneration
		return active;

	/** Number of successful complete-owner swaps after the initial generation. */
	public inline function publicationCount():Int
		return completedPublications;

	/**
	 * Select one newer complete candidate through a single commit assignment.
	 *
	 * Ordering validation and result construction finish before `active` changes.
	 * The assignment and following integer increment cannot reject, allocate, or
	 * invoke application code, so no rollback mutation log is needed.
	 */
	public function publish(candidate:RuntimeContentGeneration):RuntimeContentPublicationResult {
		final candidateId = candidate.generationId();
		final activeId = active.generationId();
		if (candidateId.value() <= activeId.value())
			return RuntimeContentPublicationRejected(OutOfOrderRuntimeContent(candidateId.value(), activeId.value()));
		final result = RuntimeContentPublished(activeId, candidateId);
		active = candidate;
		completedPublications++;
		return result;
	}
}
