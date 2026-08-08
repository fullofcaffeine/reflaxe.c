package caxecraft.qa;

import caxecraft.content.ActiveRuntimeContent;
import caxecraft.content.ActiveRuntimeContent.RuntimeContentPublicationResult;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentLoadResult;
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContent;
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContentForTesting;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import haxe.io.Bytes;

/**
 * Proves registry, UI, map/session, and receipt identity publish together.
 *
 * Generation 1 first becomes active from the real package root. A malformed
 * generation-2 request must leave every observable unchanged. A complete
 * generation 2 then swaps once, while older and duplicate candidates reject.
 * The expected pack, UI, and world values come from their existing independent
 * focused owners rather than from the publication implementation.
 */
/** Independent semantic proof borrowed from the active runtime registry. */
var tracePack:Int = 0;

/** Reviewed Spanish Adventure label length from the active runtime catalog. */
var traceUi:Int = 0;

/** Existing deterministic world-state oracle from the active level. */
var traceWorld:Int = 0;

/** Run the fast Eval owner and print its small reviewed result envelope. */
function main():Void {
	final result = selfCheck();
	Sys.println(result);
	Sys.println(tracePack);
	Sys.println(traceUi);
	Sys.println(traceWorld);
}

/** Return zero only after failed, successful, old, and duplicate paths agree. */
function selfCheck():Int {
	final store = switch ContentPackageStore.open(".", "runtime-content-publication", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final first = switch load(store, 1) {
		case RuntimeContentReady(value): value;
		case RuntimeContentRejected(_): return 2;
	};
	final active = new ActiveRuntimeContent(first);
	if (active.generation() != first || !activeMatches(active, 1, 0))
		return 3;

	final malformed = switch readPackageSources(store) {
		case null: return 4;
		case sources:
			loadRuntimeContentForTesting({
				receipt: Bytes.ofString("{"),
				content: sources.content,
				ui: sources.ui,
				map: sources.map,
				missingLogicalPath: ""
			}, ContentGenerationId.fromSequence(2), player());
	};
	switch malformed {
		case RuntimeContentRejected(RuntimeContentReceiptRejected(_)):
		case _:
			return 5;
	}
	if (active.generation() != first || !activeMatches(active, 1, 0))
		return 6;

	final second = switch load(store, 2) {
		case RuntimeContentReady(value): value;
		case RuntimeContentRejected(_): return 7;
	};
	final retained = active.generation();
	switch active.publish(second) {
		case RuntimeContentPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 8;
	}
	if (active.generation() != second || active.generation() == retained || !activeMatches(active, 2, 1))
		return 9;
	if (retained.generationId().value() != 1
		|| retained.registry().semanticProof() != 132089
		|| retained.catalog().text(LocaleCursor.Locale1, UiMessage.MenuAdventure) != "AVENTURA"
		|| retained.level().generation().semanticTrace().worldState != 454073574)
		return 14;

	switch active.publish(first) {
		case RuntimeContentPublicationRejected(OutOfOrderRuntimeContent(1, 2)):
		case _:
			return 10;
	}
	switch active.publish(second) {
		case RuntimeContentPublicationRejected(OutOfOrderRuntimeContent(2, 2)):
		case _:
			return 11;
	}
	if (active.generation() != second || !activeMatches(active, 2, 1))
		return 12;
	final selected = active.generation();
	tracePack = selected.registry().semanticProof();
	traceUi = selected.catalog().text(LocaleCursor.Locale1, UiMessage.MenuAdventure).length;
	traceWorld = selected.level().generation().semanticTrace().worldState;
	return tracePack == 132089 && traceUi == 8 && traceWorld == 454073574 ? 0 : 13;
}

/** Load one complete real-package candidate with a selected sequence. */
function load(store:ContentPackageStore, sequence:Int):RuntimeContentLoadResult
	return loadRuntimeContent(store, ContentGenerationId.fromSequence(sequence), player());

/** Application-selected player facts shared by both complete candidates. */
function player():caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions
	return {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH
	};

/** Exact real bytes needed only to make one malformed receipt request faithful. */
function readPackageSources(store:ContentPackageStore):Null<{content:Bytes, ui:Bytes, map:Bytes}> {
	final content = switch store.read("packs/caxecraft/base/content.json") {
		case PackageBytesRead(value): value.bytes;
		case PackageBytesRejected(_): return null;
	};
	final ui = switch store.read("locales/ui.json") {
		case PackageBytesRead(value): value.bytes;
		case PackageBytesRejected(_): return null;
	};
	final map = switch store.read("scenarios/first-playable/map.caxemap") {
		case PackageBytesRead(value): value.bytes;
		case PackageBytesRejected(_): return null;
	};
	return {content: content, ui: ui, map: map};
}

/** Compare independently owned observations from one complete active owner. */
function activeMatches(active:ActiveRuntimeContent, generation:Int, publications:Int):Bool {
	final selected = active.generation();
	final receipt = selected.receipt();
	return selected.generationId().value() == generation
		&& active.publicationCount() == publications
		&& selected.registry().semanticProof() == 132089
		&& selected.catalog().text(LocaleCursor.Locale1, UiMessage.MenuAdventure) == "AVENTURA"
		&& selected.level().generation().semanticTrace().worldState == 454073574
		&& receipt.generationSha256 == "b0b99b2a21d0c6d2a4b527183cf8473c9ba1db366954ffd6d84d9178f9641101";
}
