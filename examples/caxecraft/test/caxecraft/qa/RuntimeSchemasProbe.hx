package caxecraft.qa;

import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.LevelContentResolver.StatefulObjectContentResolution;
import caxecraft.content.RuntimeContentPack;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.content.RuntimeContentPack.RuntimeItemUseProfile;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.RuntimeUiCatalog.RuntimeUiCatalogResult;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.ContentId;
import haxe.io.Bytes;

/**
 * Proves the shipped content and UI schemas through one real package boundary.
 *
 * Preconditions: the process starts in the Caxecraft root and the checked-in
 * `content.json` and `ui.json` are present. The test reads their exact bytes,
 * admits the closed schemas, observes manually reviewed registry/catalog
 * values, and then applies one mutation for each distinct rejection family.
 * Eval and strict generated C execute this same Haxe owner; the C harness only
 * observes four scalar results.
 */
/** First broken invariant, or zero after every schema check passes. */
var observed:Int = 0;

/** Manually expected semantic proof from the reviewed base content JSON. */
var tracePack:Int = 0;

/** Manually expected text/shape proof from the reviewed UI JSON. */
var traceUi:Int = 0;

/** Source line from a representative version diagnostic. */
var traceDiagnosticLine:Int = 0;

/** Run the shared tracer under Eval or publish its envelope to native C. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(tracePack);
	Sys.println(traceUi);
	Sys.println(traceDiagnosticLine);
	#end
}

/** Return zero only when the real positive path and focused negatives agree. */
function selfCheck():Int {
	final store = switch ContentPackageStore.open(".", "caxecraft-runtime-schema", 32 * 1024) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final content = readRequired(store, "packs/caxecraft/base/content.json");
	if (content == null)
		return 2;
	final ui = readRequired(store, "locales/ui.json");
	if (ui == null)
		return 3;

	final registry = switch RuntimeContentPack.decode(content.bytes) {
		case RuntimeContentPackReady(value): value;
		case RuntimeContentPackRejected(_): return 4;
	};
	final catalog = switch RuntimeUiCatalog.decode(ui.bytes) {
		case RuntimeUiCatalogReady(value): value;
		case RuntimeUiCatalogRejected(_): return 5;
	};

	tracePack = registry.semanticProof();
	final sand = new ContentId("caxecraft:sand");
	final sandBlock = new ContentId("caxecraft:sand-block");
	final sandBlockCode = registry.itemStorageCode(sandBlock);
	final sandPresentation = registry.itemPresentation(sandBlockCode);
	if (tracePack != 132089
		|| registry.packId() != "caxecraft:base"
		|| registry.packVersion() != 1
		|| registry.blockCount() != 10
		|| registry.itemCount() != 10
		|| registry.blockStorageCode(new ContentId("caxecraft:grass")) != 1
		|| registry.blockStorageCode(sand) != 5
		|| !registry.blockIsCollectable(sand)
		|| registry.blockDropItemStorageCode(sand) != sandBlockCode
		|| sandBlockCode != 7
		|| registry.itemUseProfile(sandBlockCode) != RuntimeItemUseProfile.PlaceBlock
		|| registry.itemPlacementBlockStorageCode(sandBlockCode) != 5
		|| registry.maximumItemQuantity(sandBlock) != 64
		|| sandPresentation == null
		|| sandPresentation.asset != "items"
		|| sandPresentation.cell != "sand-block"
		|| sandPresentation.cellIndex != 14
		|| registry.itemStorageCode(new ContentId("caxecraft:tideweave-suit")) != 9
		|| registry.maximumItemQuantity(new ContentId("caxecraft:berries")) != 64)
		return 6;
	switch registry.resolveFluid(new ContentId("caxecraft:water")) {
		case FluidContentResolved(_, 5):
		case _:
			return 7;
	}
	switch [
		registry.resolveActorPresentation(new ContentId("caxecraft:nia")),
		registry.resolveActorPresentation(new ContentId("caxecraft:mossling")),
		registry.resolveActorPresentation(new ContentId("caxecraft:ceesh"))
	] {
		case [
			ActorPresentationResolved("entities", 4),
			ActorPresentationResolved("entities", 8),
			ActorPresentationResolved("adventure-characters", 4)
		]:
		case _:
			return 39;
	}
	final gateId = new ContentId("caxecraft:vault-gate");
	final gateOpen = new ContentId("caxecraft:open");
	final gateSealed = new ContentId("caxecraft:sealed");
	if (registry.statefulObjectInteractionRadiusMilli(gateId) != 0
		|| registry.statefulObjectVisible(gateId, gateOpen)
		|| !registry.statefulObjectVisible(gateId, gateSealed))
		return 52;
	switch registry.resolveStatefulObject(gateId, gateSealed) {
		case StatefulObjectContentResolved(0, bounds, states, "terrain", 10)
			if (bounds.widthMilli == 7000 && bounds.heightMilli == 3000 && bounds.depthMilli == 500 && states.length == 2):
		case _:
			return 53;
	}

	if (catalog.localeCount() != 2
		|| catalog.messageCount() != 36
		|| catalog.text(LocaleCursor.Locale0, UiMessage.Brand) != "CAXECRAFT  //  C + HAXE"
		|| catalog.text(LocaleCursor.Locale1, UiMessage.MenuAdventure) != "AVENTURA"
		|| catalog.text(LocaleCursor.Locale1, UiMessage.EditorTitle) != "EDITOR DE MUNDOS CAXECRAFT")
		return 8;
	final completeProof = completeUiLookupProof(catalog);
	if (completeProof != 42396) {
		traceUi = completeProof;
		return 9;
	}
	traceUi = catalog.messageCount() * 100 + catalog.localeCount() * 10 + catalog.text(LocaleCursor.Locale1, UiMessage.MenuAdventure).length;
	if (traceUi != 3628)
		return 36;

	return negativeChecks();
}

/** Exercise every typed message lookup against a source-reviewed length sum. */
function completeUiLookupProof(catalog:RuntimeUiCatalog):Int {
	final messages:Array<UiMessage> = [
		AquaticGearEquipped,
		Brand,
		CapturePrompt,
		Controls,
		DebugCells,
		DebugDraws,
		DebugFrame,
		DebugTick,
		DebugVisible,
		EditorAdvanced,
		EditorBack,
		EditorCanvasHelp,
		EditorInvalid,
		EditorName,
		EditorNewWorld,
		EditorReady,
		EditorRedo,
		EditorScene,
		EditorStopTest,
		EditorTest,
		EditorTesting,
		EditorTitle,
		EditorToolList,
		EditorUndo,
		EditorValid,
		EditorValidate,
		HealthFull,
		MenuAdventure,
		MenuCreative,
		MenuEditor,
		MenuInstructions,
		NoBlockInReach,
		PauseHelp,
		PauseTitle,
		PlaceBlocked,
		TitleFallback
	];
	var proof = 0;
	for (index in 0...messages.length) {
		final weight = index + 1;
		proof += weight * catalog.text(Locale0, messages[index]).length;
		proof += weight * 2 * catalog.text(Locale1, messages[index]).length;
	}
	return proof;
}

/** Read one required package file without exposing the store's host root. */
function readRequired(store:ContentPackageStore, logicalPath:String):Null<LoadedPackageBytes> {
	return switch store.read(logicalPath) {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(_): null;
	};
}

/** Exercise one representative mutation for every distinct pack family. */
function negativeChecks():Int {
	final version = expectPackRejection(locatedUnsupportedVersionPack(), UnsupportedVersion);
	if (version == null)
		return 10;
	traceDiagnosticLine = version.line;
	if (version.line != 2 || version.column <= 0)
		return 11;
	final minimal = minimalPack();
	switch RuntimeContentPack.decode(Bytes.ofString(minimal)) {
		case RuntimeContentPackReady(registry):
			final objectId = new ContentId("caxecraft:glyph-control");
			final active = new ContentId("caxecraft:active");
			final idle = new ContentId("caxecraft:idle");
			final activePresentation = registry.statefulObjectPresentation(objectId, active);
			final idlePresentation = registry.statefulObjectPresentation(objectId, idle);
			if (!registry.hasStatefulObject(objectId)
				|| !registry.hasState(active)
				|| !registry.hasState(idle)
				|| registry.statefulObjectInteractionRadiusMilli(objectId) != 2500
				|| activePresentation == null
				|| activePresentation.cell != "glyph-leaf"
				|| idlePresentation == null
				|| idlePresentation.cell != "glyph-wave"
				|| !registry.statefulObjectVisible(objectId, active)
				|| registry.statefulObjectVisible(objectId, idle))
				return 45;
			switch registry.resolveStatefulObject(objectId, active) {
				case StatefulObjectContentResolved(_, bounds, states, _, _):
					if (bounds.widthMilli != 1000
						|| bounds.heightMilli != 1000
						|| bounds.depthMilli != 1000
						|| states.length != 2
						|| states[0].state != active
						|| states[0].collision != StatefulObjectSolid
						|| !states[0].visible
						|| states[1].state != idle
						|| states[1].collision != StatefulObjectPassable
						|| states[1].visible) return 48;
				case UnknownStatefulObjectContent:
					return 48;
			}
		case RuntimeContentPackRejected(_):
			return 37;
	}
	if (!rejectsPack(replaceOnce(minimal, '"id":"caxecraft:idle","collision"', '"id":"caxecraft:missing","collision"'), UnresolvedReference))
		return 46;
	if (!rejectsPack(replaceOnce(minimal, '"id":"caxecraft:active","collision"', '"id":"caxecraft:idle","collision"'), DuplicateId))
		return 47;
	if (!rejectsPack(replaceOnce(minimal, '"widthMilli":1000', '"widthMilli":0'), InvalidInteger))
		return 49;
	if (!rejectsPack(replaceOnce(minimal, '"collision":"solid"', '"collision":"blocking"'), InvalidClosedValue))
		return 50;
	if (!rejectsPack(replaceOnce(minimal, '"render":"hidden"', '"render":"sometimes"'), InvalidClosedValue))
		return 51;
	if (!rejectsPack(replaceOnce(minimal, '"interaction":"activate"', '"interaction":"none"'), InvalidInvariant))
		return 54;
	if (!rejectsPack(replaceOnce(minimal, '"id":"entities"', '"id":"adventure-items"'), DuplicateId))
		return 41;
	if (!rejectsPack(replaceOnce(minimal, '"grass-block"', '"berries"'), DuplicateValue))
		return 42;
	if (!rejectsPack(replaceOnce(minimal, '"id":"adventure-items"', '"id":"zz-assets"'), NonCanonicalOrder))
		return 43;
	if (!rejectsPack(replaceOnce(minimal, '"mossling-front"', '"Mossling"'), InvalidString))
		return 44;
	if (!rejectsPack(replaceOnce(minimal, '"packVersion":1,', ""), MissingField))
		return 12;
	if (!rejectsPack(replaceOnce(minimal, '"packVersion":1', '"surprise":1'), UnknownField))
		return 13;
	if (!rejectsPack(replaceOnce(minimal, '"packVersion":1', '"packVersion":"1"'), WrongType))
		return 14;
	if (!rejectsPack(replaceOnce(minimal, '"id":"caxecraft:dirt"', '"id":"caxecraft:air"'), DuplicateId))
		return 15;
	if (!rejectsPack(replaceOnce(minimal, '"id":"caxecraft:feedback"', '"id":"caxecraft:core"'), CrossKindId))
		return 16;
	if (!rejectsPack(replaceOnce(minimal, '"simulationProfile":"bounded-water"', '"simulationProfile":"unbounded-water"'), InvalidClosedValue))
		return 17;
	final missingAir = replaceOnce(minimal, '"airBlock":"caxecraft:air"', '"airBlock":"caxecraft:missing"');
	final missingAirDiagnostic = expectPackRejection(missingAir, UnresolvedReference);
	if (!pointsAtFirstValue(missingAirDiagnostic, missingAir, "caxecraft:missing"))
		return 34;
	final missingDefault = replaceOnce(minimal, '"defaultAquaticProfile":"caxecraft:standard"', '"defaultAquaticProfile":"caxecraft:missing"');
	final missingDefaultDiagnostic = expectPackRejection(missingDefault, UnresolvedReference);
	if (!pointsAtFirstValue(missingDefaultDiagnostic, missingDefault, "caxecraft:missing"))
		return 35;
	if (!rejectsPack(replaceOnce(minimal, '"placementBlock":"caxecraft:dirt"', '"placementBlock":"caxecraft:missing"'), UnresolvedReference))
		return 18;
	if (!rejectsPack(replaceOnce(minimal, '"dropItem":"caxecraft:block-item"', '"dropItem":"caxecraft:water"'), WrongReferenceKind))
		return 19;
	if (!rejectsPack(replaceOnce(minimal, '"aquaticProfile":"caxecraft:standard"', '"aquaticProfile":"caxecraft:missing"'), UnresolvedReference))
		return 20;
	if (!rejectsPack(replaceOnce(minimal, '"drop":"caxecraft:drop"', '"drop":"caxecraft:missing"'), UnresolvedReference))
		return 21;
	if (!rejectsPack(replaceOnce(minimal, '"asset":"terrain"', '"asset":"missing"'), UnknownAsset))
		return 22;
	if (!rejectsPack(replaceOnce(minimal, '"cell":"teal-water"', '"cell":"missing"'), UnknownAssetCell))
		return 23;
	if (!rejectsPack(replaceOnce(minimal, '"id":"caxecraft:air"', '"id":"caxecraft:zz-air"'), NonCanonicalOrder))
		return 24;
	if (!rejectsPack(replaceOnce(minimal, '"maxStack":64', '"maxStack":65'), InvalidInteger))
		return 25;
	if (!rejectsPack(replaceOnce(minimal, '"storageCode":1', '"storageCode":0'), DuplicateStorageCode))
		return 38;
	if (!rejectsPack(replaceOnce(minimal, '"prefabs":[]', '"prefabs":[null]'), UnsupportedReservedKind))
		return 26;
	return uiNegativeChecks(minimalUiCatalog());
}

/** Build a tiny exact root whose unsupported version begins on source line two. */
function locatedUnsupportedVersionPack():String
	return '{\n"schemaVersion":3,"logicalPath":null,"packId":null,"packVersion":null,"assetManifestId":null,"assetCells":null,"airBlock":null,'
		+ '"defaultAquaticProfile":null,"features":null,"blocks":null,"fluids":null,"aquaticProfiles":null,"items":null,"npcs":null,"enemies":null,'
		+ '"drops":null,"effects":null,"prefabs":null,"statefulObjects":null,"states":null,"signals":null}';

/**
 * Return one manually authored valid pack for fast negative sensitivity.
 *
 * The real positive path above remains the product tracer. This smaller source
 * contains every current reference family and closed mechanic but no copied
 * decoder algorithm, so each mutation can isolate one failure without paying
 * to rebuild the complete showcase registry repeatedly.
 */
function minimalPack():String
	return '{"schemaVersion":2,"logicalPath":"packs/test","packId":"caxecraft:test","packVersion":1,"assetManifestId":"caxecraft-showcase-v1-draft",'
		+ '"assetCells":[{"id":"adventure-items","cells":["glyph-wave","glyph-leaf","tideweave-suit-folded"]},'
		+ '{"id":"entities","cells":["mossling-front"]},'
		+ '{"id":"items","cells":["berries","grass-block"]},{"id":"terrain","cells":["teal-water"]}],'
		+ '"airBlock":"caxecraft:air","defaultAquaticProfile":"caxecraft:standard","features":["caxecraft:core"],'
		+ '"blocks":[{"id":"caxecraft:air","storageCode":0,"collision":"passable","edit":"immutable","dropItem":null,"renderProfile":"air"},'
		+ '{"id":"caxecraft:dirt","storageCode":1,"collision":"solid","edit":"collectable","dropItem":"caxecraft:block-item","renderProfile":"rich-soil"}],'
		+ '"fluids":[{"id":"caxecraft:water","simulationProfile":"bounded-water","renderProfile":"translucent-voxel","cameraProfile":"clear-submersion",'
		+ '"audioProfile":"fresh-water","presentation":{"asset":"terrain","cell":"teal-water"}}],'
		+ '"aquaticProfiles":[{"id":"caxecraft:standard","maximumBreathTicks":120,"breathRecoveryPerTick":4,"horizontalControlMilli":350,'
		+ '"ascentAccelerationMilli":14000,"descentAccelerationMilli":20000,"buoyancyAccelerationMilli":12000,"dragPerTickMilli":180,'
		+ '"drowningIntervalTicks":20,"underwaterMining":false,"coldProtection":false}],'
		+ '"items":[{"id":"caxecraft:block-item","maxStack":64,"useProfile":"place-block","placementBlock":"caxecraft:dirt","aquaticProfile":null,'
		+ '"icon":{"asset":"items","cell":"grass-block"}},{"id":"caxecraft:gear","maxStack":1,"useProfile":"equip-aquatic","placementBlock":null,'
		+ '"aquaticProfile":"caxecraft:standard","icon":{"asset":"adventure-items","cell":"tideweave-suit-folded"}},'
		+ '{"id":"caxecraft:item","maxStack":64,"useProfile":"none","placementBlock":null,"aquaticProfile":null,"icon":{"asset":"items","cell":"berries"}}],'
		+ '"npcs":[],"enemies":[{"id":"caxecraft:enemy","behaviorProfile":"wander-chase-melee","maxHealth":3,"noticeRadiusMilli":6000,'
		+ '"strikeRadiusMilli":3000,"attackRadiusMilli":1400,"windupTicks":8,"recoveryTicks":12,"stepMilli":80,"drop":"caxecraft:drop",'
		+ '"presentation":{"asset":"entities","cell":"mossling-front"}}],'
		+ '"drops":[{"id":"caxecraft:drop","item":"caxecraft:item","quantity":1,"pickupRadiusMilli":1500,"presentation":{"asset":"items","cell":"berries"}}],'
		+ '"effects":[{"id":"caxecraft:feedback","profile":"pickup-feedback"}],"prefabs":[],'
		+ '"statefulObjects":[{"id":"caxecraft:glyph-control","interaction":"activate","interactionRadiusMilli":2500,'
		+ '"bounds":{"widthMilli":1000,"heightMilli":1000,"depthMilli":1000},'
		+ '"states":[{"id":"caxecraft:active","collision":"solid","render":"visible","presentation":{"asset":"adventure-items","cell":"glyph-leaf"}},'
		+ '{"id":"caxecraft:idle","collision":"passable","render":"hidden","presentation":{"asset":"adventure-items","cell":"glyph-wave"}}]}],'
		+ '"states":["caxecraft:active","caxecraft:idle"],"signals":[]}';

/**
 * Return the first two correctly shaped typed messages for fast UI negatives.
 *
 * Every mutation below fails before the complete-catalog compatibility check;
 * the real 35-message positive path remains the proof that all shipped text is
 * admitted and mapped to the existing constructors.
 */
function minimalUiCatalog():String
	return '{"schemaVersion":1,"catalogId":"caxecraft.ui","defaultLocale":"en","locales":["en","es-MX"],"messages":['
		+ '{"id":"aquatic_gear_equipped","symbol":"AquaticGearEquipped","text":{"en":"AQUATIC GEAR EQUIPPED","es-MX":"EQUIPO ACUATICO ACTIVADO"}},'
		+ '{"id":"brand","symbol":"Brand","text":{"en":"CAXECRAFT  //  C + HAXE","es-MX":"CAXECRAFT  //  C + HAXE"}}]}';

/** Exercise catalog identity, ordering, locale, message, and text bounds. */
function uiNegativeChecks(ui:String):Int {
	if (!rejectsUi(replaceOnce(ui, '"schemaVersion":1', '"schemaVersion":2'), UnsupportedVersion))
		return 27;
	if (!rejectsUi(replaceOnce(ui, '"defaultLocale":"en"', '"defaultLocale":"fr"'), InvalidLocale))
		return 28;
	if (!rejectsUi(replaceOnce(ui, '"id":"brand"', '"id":"aquatic_gear_equipped"'), DuplicateId))
		return 29;
	if (!rejectsUi(replaceOnce(ui, '"id":"aquatic_gear_equipped"', '"id":"zz_aquatic_gear_equipped"'), NonCanonicalOrder))
		return 30;
	if (!rejectsUiAt(replaceOnce(ui, '"symbol":"Brand"', '"symbol":"DifferentBrand"'), IncompatibleTypedCatalog, "messages[1]"))
		return 31;
	if (!rejectsUi(replaceOnce(ui, '"es-MX":"CAXECRAFT  //  C + HAXE"', '"fr":"CAXECRAFT  //  C + HAXE"'), InvalidLocale))
		return 32;
	if (!rejectsUi(replaceOnce(ui, '"en":"CAXECRAFT  //  C + HAXE"', '"en":""'), InvalidText))
		return 33;
	return 0;
}

/** Decode one mutated pack and compare only its intended rejection family. */
function rejectsPack(source:String, family:ExpectedSchemaFamily):Bool
	return expectPackRejection(source, family) != null;

/** Return one located pack diagnostic when its family is the expected one. */
function expectPackRejection(source:String, family:ExpectedSchemaFamily):Null<RuntimeSchemaDiagnostic> {
	return switch RuntimeContentPack.decode(Bytes.ofString(source)) {
		case RuntimeContentPackRejected(diagnostic) if (sameFamily(diagnostic.kind, family) && diagnostic.line > 0 && diagnostic.column > 0): diagnostic;
		case _: null;
	};
}

/** Prove one diagnostic points at the mutated JSON String, not a fallback. */
function pointsAtFirstValue(diagnostic:Null<RuntimeSchemaDiagnostic>, source:String, value:String):Bool {
	if (diagnostic == null || diagnostic.line != 1)
		return false;
	return diagnostic.column == source.indexOf('"' + value + '"') + 1;
}

/** Decode one mutated UI catalog and compare its intended rejection family. */
function rejectsUi(source:String, family:ExpectedSchemaFamily):Bool {
	return switch RuntimeUiCatalog.decode(Bytes.ofString(source)) {
		case RuntimeUiCatalogRejected(diagnostic): sameFamily(diagnostic.kind, family) && diagnostic.line > 0 && diagnostic.column > 0;
		case RuntimeUiCatalogReady(_): false;
	};
}

/** Require one UI rejection family and exact schema path for sensitivity. */
function rejectsUiAt(source:String, family:ExpectedSchemaFamily, expectedPath:String):Bool {
	return switch RuntimeUiCatalog.decode(Bytes.ofString(source)) {
		case RuntimeUiCatalogRejected(diagnostic): final pathMatches = switch diagnostic.kind {
				case SchemaIncompatibleTypedCatalog(path): path == expectedPath;
				case _: false;
			}; sameFamily(diagnostic.kind, family) && pathMatches && diagnostic.line > 0 && diagnostic.column > 0;
		case RuntimeUiCatalogReady(_): false;
	};
}

/** Test-only family names keep assertions precise without copying payload text. */
private enum ExpectedSchemaFamily {
	UnsupportedVersion;
	MissingField;
	UnknownField;
	WrongType;
	DuplicateId;
	DuplicateValue;
	InvalidString;
	CrossKindId;
	InvalidClosedValue;
	InvalidInvariant;
	UnresolvedReference;
	WrongReferenceKind;
	UnknownAsset;
	UnknownAssetCell;
	NonCanonicalOrder;
	InvalidInteger;
	DuplicateStorageCode;
	UnsupportedReservedKind;
	InvalidLocale;
	IncompatibleTypedCatalog;
	InvalidText;
}

/** Match one closed diagnostic constructor while retaining source location. */
function sameFamily(actual:RuntimeSchemaErrorKind, expected:ExpectedSchemaFamily):Bool {
	return switch [actual, expected] {
		case [SchemaUnsupportedVersion(_, _), UnsupportedVersion]: true;
		case [SchemaMissingField(_, _), MissingField]: true;
		case [SchemaUnknownField(_, _), UnknownField]: true;
		case [SchemaWrongType(_, _), WrongType]: true;
		case [SchemaDuplicateId(_, _), DuplicateId]: true;
		case [SchemaDuplicateValue(_, _), DuplicateValue]: true;
		case [SchemaInvalidString(_), InvalidString]: true;
		case [SchemaCrossKindId(_), CrossKindId]: true;
		case [SchemaInvalidClosedValue(_, _), InvalidClosedValue]: true;
		case [SchemaInvalidInvariant(_), InvalidInvariant]: true;
		case [SchemaUnresolvedReference(_, _, _), UnresolvedReference]: true;
		case [SchemaWrongReferenceKind(_, _, _), WrongReferenceKind]: true;
		case [SchemaUnknownAsset(_, _), UnknownAsset]: true;
		case [SchemaUnknownAssetCell(_, _, _), UnknownAssetCell]: true;
		case [SchemaNonCanonicalOrder(_), NonCanonicalOrder]: true;
		case [SchemaInvalidInteger(_, _, _), InvalidInteger]: true;
		case [SchemaDuplicateStorageCode(_), DuplicateStorageCode]: true;
		case [SchemaUnsupportedReservedKind(_), UnsupportedReservedKind]: true;
		case [SchemaInvalidLocale(_), InvalidLocale]: true;
		case [SchemaIncompatibleTypedCatalog(_), IncompatibleTypedCatalog]: true;
		case [SchemaInvalidText(_), InvalidText]: true;
		case _: false;
	};
}

/** Replace exactly the first reviewed token and leave later references intact. */
function replaceOnce(source:String, needle:String, replacement:String):String {
	final at = source.indexOf(needle);
	if (at < 0)
		return source;
	return source.substring(0, at) + replacement + source.substring(at + needle.length);
}
