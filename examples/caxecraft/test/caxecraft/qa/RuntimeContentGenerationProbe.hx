package caxecraft.qa;

import caxecraft.content.ActiveRuntimeContent;
import caxecraft.content.ActiveRuntimeContent.RuntimeContentPublicationResult;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.ContentReceipt;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentFileReceipt;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentLoadResult;
import caxecraft.content.RuntimeContentGeneration.RuntimeContentReceipt;
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContent;
#if c
import caxecraft.content.RuntimeContentGeneration.rebuildRuntimeContentForPublicationTesting;
#end
#if !c
import caxecraft.content.RuntimeContentGeneration.loadRuntimeContentForTesting;
#end
import caxecraft.content.RuntimeContentDigest.runtimeSha256;
import caxecraft.content.RuntimeContentReceiptWriter.runtimeGenerationSha256;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import haxe.io.Bytes;

/**
 * Proves one real pack, UI catalog, and map publish as one generation.
 *
 * The process starts in the Caxecraft source package. The loader must read the
 * checked-in runtime receipt and its three named files, verify independently
 * authored SHA-256 expectations, decode the pack and UI schemas, resolve the
 * map through that decoded registry, build two complete candidates, publish the
 * newer one, and reject older or duplicate candidates. Eval reads both
 * candidates from the package. Generated native C reads it once, then rebuilds
 * only a fresh level/session from those verified immutable package facts; both
 * execute the same Haxe publication and observation path.
 */
/** First broken invariant, or zero after the complete tracer passes. */
var observed:Int = 0;

/** Monotonic identity retained after one successful complete-owner swap. */
var traceGenerationId:Int = 0;

/** Existing independent semantic proof from the decoded pack registry. */
var tracePack:Int = 0;

/** Data-derived locale shape and one non-empty translated-message observation. */
var traceUi:Int = 0;

/** Deterministic world state built from the real map and runtime registry. */
var traceWorldState:Int = 0;

/** Total exact byte count across pack, UI, and map source receipts. */
var traceSourceBytes:Int = 0;

/** Run the shared tracer or publish its scalar envelope to the C observer. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(traceGenerationId);
	Sys.println(tracePack);
	Sys.println(traceUi);
	Sys.println(traceWorldState);
	Sys.println(traceSourceBytes);
	#end
}

/** Return zero only when the real package-to-generation path is complete. */
function selfCheck():Int {
	if (!digestMatches("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")
		|| !digestMatches("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad")
		|| !digestMatches("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"))
		return 1;
	final store = switch ContentPackageStore.open(".", "caxecraft-runtime-content", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 2;
	};
	final candidate = switch load(store, 1) {
		case RuntimeContentReady(value): value;
		case RuntimeContentRejected(_): return 8;
	};
	#if !c
	final activeGenerationBeforeFailures = candidate.generationId().value();
	final rejected = verifyReceiptRejections(store, candidate.receipt());
	if (rejected != 0)
		return rejected;
	#end
	final active = new ActiveRuntimeContent(candidate);
	final secondResult = #if c rebuildRuntimeContentForPublicationTesting(candidate, ContentGenerationId.fromSequence(2)) #else load(store, 2) #end;
	final second = switch secondResult {
		case RuntimeContentReady(value): value;
		case RuntimeContentRejected(_): return 12;
	};
	switch active.publish(second) {
		case RuntimeContentPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 13;
	}
	if (active.generation() != second || active.generation() == candidate)
		return 18;
	switch active.publish(candidate) {
		case RuntimeContentPublicationRejected(OutOfOrderRuntimeContent(1, 2)):
		case _:
			return 14;
	}
	switch active.publish(second) {
		case RuntimeContentPublicationRejected(OutOfOrderRuntimeContent(2, 2)):
		case _:
			return 15;
	}
	if (active.publicationCount() != 1)
		return 16;
	if (candidate.generationId().value() != 1
		|| candidate.registry().semanticProof() != 132089
		|| candidate.catalog().text(LocaleCursor.Locale1, UiMessage.MenuAdventure).length == 0
		|| candidate.level().generation().semanticTrace().worldState != -1465000778)
		return 17;
	final selected = active.generation();
	final receipt = selected.receipt();
	if (!validReceipt(receipt))
		return 9;
	traceGenerationId = selected.generationId().value();
	#if !c
	if (activeGenerationBeforeFailures != 1)
		return 11;
	#end
	tracePack = selected.registry().semanticProof();
	traceUi = selected.catalog().localeCount() * 10 + selected.catalog().text(LocaleCursor.Locale1, UiMessage.MenuAdventure).length;
	traceWorldState = selected.level().generation().semanticTrace().worldState;
	traceSourceBytes = receipt.content.byteLength + receipt.ui.byteLength + receipt.map.byteLength;
	return traceGenerationId == 2
		&& tracePack == 132089
		&& traceUi > selected.catalog().localeCount() * 10
		&& traceWorldState == -1465000778
		&& traceSourceBytes > 0 ? 0 : 10;
}

/** Recompute receipt identity from its data-owned file records and stable roles. */
function validReceipt(receipt:RuntimeContentReceipt):Bool {
	if (receipt.assetManifestSchemaVersion <= 0
		|| receipt.assetManifestId.length == 0
		|| receipt.assetManifestSha256.length != 64
		|| !validReceiptFile(receipt.content, "content-pack")
		|| !validReceiptFile(receipt.ui, "ui-catalog")
		|| !validReceiptFile(receipt.map, "level-map"))
		return false;
	final content = new ContentReceipt(receipt.content.logicalPath, receipt.content.byteLength, receipt.content.sha256);
	final ui = new ContentReceipt(receipt.ui.logicalPath, receipt.ui.byteLength, receipt.ui.sha256);
	final map = new ContentReceipt(receipt.map.logicalPath, receipt.map.byteLength, receipt.map.sha256);
	return receipt.generationSha256 == runtimeGenerationSha256(receipt.assetManifestId, receipt.assetManifestSha256, content, ui, map);
}

/** Validate one closed receipt role without copying its current bytes or digest. */
function validReceiptFile(receipt:RuntimeContentFileReceipt, kind:String):Bool
	return receipt.kind == kind && receipt.logicalPath.length > 0 && receipt.byteLength > 0 && receipt.sha256.length == 64;

/** Load one real complete candidate through the shared package path. */
function load(store:ContentPackageStore, sequence:Int):RuntimeContentLoadResult
	return loadRuntimeContent(store, ContentGenerationId.fromSequence(sequence), {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH
	});

#if !c
/** Prove malformed, unlisted, stale, mismatched, and missing inputs publish nothing. */
function verifyReceiptRejections(store:ContentPackageStore, expected:RuntimeContentReceipt):Int {
	return switch store.read("packs/caxecraft/base/runtime-content.json") {
		case PackageBytesRejected(_): 2;
		case PackageBytesRead(receipt):
			switch store.read("packs/caxecraft/base/content.json") {
				case PackageBytesRejected(_): 2;
				case PackageBytesRead(content):
					switch store.read("locales/ui.json") {
						case PackageBytesRejected(_): 2;
						case PackageBytesRead(ui):
							switch store.read("scenarios/first-playable/map.caxemap") {
								case PackageBytesRejected(_): 2;
								case PackageBytesRead(map): verifyReceiptBytes(receipt.bytes, content.bytes, ui.bytes, map.bytes, expected);
							};
					};
			};
	};
}

/** Challenge the verifier after the real package store has supplied exact bytes. */
function verifyReceiptBytes(receipt:Bytes, content:Bytes, ui:Bytes, map:Bytes, expected:RuntimeContentReceipt):Int {
	final receiptText = receipt.toString();
	final player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions = {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH
	};
	final generation = ContentGenerationId.fromSequence(1);

	if (!receiptRejected(Bytes.ofString("{"), content, ui, map, "", generation, player))
		return 3;
	final unlisted = replaceOnce(receiptText, expected.map.logicalPath, expected.map.logicalPath + ".unlisted");
	if (!receiptRejected(Bytes.ofString(unlisted), content, ui, map, "", generation, player))
		return 4;
	final contentLength = '"byteLength": ${expected.content.byteLength},\n      "kind": "content-pack"';
	final staleContentLength = '"byteLength": ${expected.content.byteLength - 1},\n      "kind": "content-pack"';
	final stale = replaceOnce(receiptText, contentLength, staleContentLength);
	if (!lengthRejected(Bytes.ofString(stale), content, ui, map, expected.content.logicalPath, expected.content.byteLength - 1, expected.content.byteLength,
		generation, player))
		return 5;
	if (!assetManifestMismatchRejected(receiptText, content, ui, map, expected, generation, player))
		return 19;
	final mismatchedContent = Bytes.alloc(content.length);
	mismatchedContent.blit(0, content, 0, content.length);
	mismatchedContent.set(0, mismatchedContent.get(0) ^ 1);
	if (!hashRejected(receipt, mismatchedContent, ui, map, expected.content.logicalPath, generation, player))
		return 6;
	if (!missingRejected(receipt, content, ui, map, expected.ui.logicalPath, generation, player))
		return 7;
	return 0;
}

/** Replace one reviewed receipt fragment and fail if fixture provenance drifts. */
function replaceOnce(source:String, needle:String, replacement:String):String {
	final at = source.indexOf(needle);
	if (at < 0 || source.indexOf(needle, at + needle.length) >= 0)
		return "";
	return source.substring(0, at) + replacement + source.substring(at + needle.length);
}

/** Return true only for a receipt-shape rejection before source decoding. */
function receiptRejected(receipt:Bytes, content:Bytes, ui:Bytes, map:Bytes, missing:String, generation:ContentGenerationId,
		player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions):Bool {
	return switch loadRuntimeContentForTesting({
		receipt: receipt,
		content: content,
		ui: ui,
		map: map,
		missingLogicalPath: missing
	}, generation, player) {
		case RuntimeContentRejected(RuntimeContentReceiptRejected(_)): true;
		case _: false;
	};
}

/** Return true only when a stale receipt length rejects the exact content file. */
function lengthRejected(receipt:Bytes, content:Bytes, ui:Bytes, map:Bytes, expectedPath:String, expectedLength:Int, actualLength:Int,
		generation:ContentGenerationId, player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions):Bool {
	return switch loadRuntimeContentForTesting({
		receipt: receipt,
		content: content,
		ui: ui,
		map: map,
		missingLogicalPath: ""
	}, generation, player) {
		case RuntimeContentRejected(RuntimeContentLengthMismatch(path, expected, actual))
			if (path == expectedPath && expected == expectedLength && actual == actualLength): true;
		case _: false;
	};
}

/** Reach the cross-file identity check with an otherwise self-consistent receipt. */
function assetManifestMismatchRejected(receiptText:String, content:Bytes, ui:Bytes, map:Bytes, expected:RuntimeContentReceipt, generation:ContentGenerationId,
		player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions):Bool {
	final originalId = expected.assetManifestId;
	final otherId = "caxecraft-other-v1-draft";
	final otherGeneration = runtimeGenerationSha256(otherId, expected.assetManifestSha256,
		new ContentReceipt(expected.content.logicalPath, content.length, expected.content.sha256),
		new ContentReceipt(expected.ui.logicalPath, ui.length, expected.ui.sha256),
		new ContentReceipt(expected.map.logicalPath, map.length, expected.map.sha256));
	final changedId = replaceOnce(receiptText, originalId, otherId);
	final changedReceipt = replaceOnce(changedId, expected.generationSha256, otherGeneration);
	return switch loadRuntimeContentForTesting({
		receipt: Bytes.ofString(changedReceipt),
		content: content,
		ui: ui,
		map: map,
		missingLogicalPath: ""
	}, generation, player) {
		case RuntimeContentRejected(RuntimeContentAssetManifestMismatch(receiptId, contentId)) if (receiptId == otherId && contentId == originalId): true;
		case _: false;
	};
}

/** Return true only when changed bytes fail the receipt's independent hash. */
function hashRejected(receipt:Bytes, content:Bytes, ui:Bytes, map:Bytes, expectedPath:String, generation:ContentGenerationId,
		player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions):Bool {
	return switch loadRuntimeContentForTesting({
		receipt: receipt,
		content: content,
		ui: ui,
		map: map,
		missingLogicalPath: ""
	}, generation, player) {
		case RuntimeContentRejected(RuntimeContentHashMismatch(path, _)) if (path == expectedPath): true;
		case _: false;
	};
}

/** Return true only when a listed source is absent from the read capability. */
function missingRejected(receipt:Bytes, content:Bytes, ui:Bytes, map:Bytes, missingPath:String, generation:ContentGenerationId,
		player:caxecraft.content.RuntimeContentGeneration.RuntimeContentPlayerOptions):Bool {
	return switch loadRuntimeContentForTesting({
		receipt: receipt,
		content: content,
		ui: ui,
		map: map,
		missingLogicalPath: missingPath
	}, generation, player) {
		case RuntimeContentRejected(RuntimeContentSourceRejected(path, EntryMissing)) if (path == missingPath): true;
		case _: false;
	};
}
#end

/** Compare digest bytes with an independently authored lowercase expectation. */
function digestMatches(input:String, expected:String):Bool {
	final digest = runtimeSha256(Bytes.ofString(input));
	if (digest.length != 32 || expected.length != 64)
		return false;
	for (index in 0...digest.length) {
		final high = hexDigit(expected.charCodeAt(index * 2));
		final low = hexDigit(expected.charCodeAt(index * 2 + 1));
		if (high < 0 || low < 0 || digest.get(index) != high * 16 + low)
			return false;
	}
	return true;
}

/** Decode one lowercase hexadecimal digit from a reviewed SHA-256 vector. */
inline function hexDigit(code:Int):Int
	return code >= 0x30 && code <= 0x39 ? code - 0x30 : code >= 0x61 && code <= 0x66 ? code - 0x61 + 10 : -1;
