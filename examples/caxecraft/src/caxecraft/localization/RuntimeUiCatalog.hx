package caxecraft.localization;

import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import haxe.io.Bytes;

/**
 * Admits the shipped UI JSON into an immutable runtime text catalog.
 *
 * Locale and message arrays remain private. The checked JSON must still match
 * the current typed `LocaleCursor` and `UiMessage` constructors exactly, so a
 * data edit cannot silently reinterpret an existing call site. Text remains
 * owned Haxe `String`; converting it to a native rendering borrow belongs to
 * the later publication/application boundary.
 */
/** Complete runtime UI catalog or one located fail-closed diagnostic. */
enum RuntimeUiCatalogResult {
	/** Every locale, typed message, and translation was admitted atomically. */
	RuntimeUiCatalogReady(catalog:RuntimeUiCatalog);

	/** No partial catalog escaped the failed candidate. */
	RuntimeUiCatalogRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/**
 * Immutable typed lookup over runtime-loaded UI strings.
 *
 * Arrays remain private and are indexed only after the typed abstract's scalar
 * code passes the catalog bounds. This preserves the existing source API while
 * moving text ownership from generated literals to validated runtime Strings.
 */
final class RuntimeUiCatalog {
	final catalogIdValue:String;
	final locales:Array<String>;
	final messages:Array<RuntimeUiMessageDefinition>;

	/** Construct one complete catalog after every candidate check passed. */
	public function new(catalogId:String, locales:Array<String>, messages:Array<RuntimeUiMessageDefinition>) {
		this.catalogIdValue = catalogId;
		this.locales = locales;
		this.messages = messages;
	}

	/** Decode one complete UI candidate without filesystem or renderer authority. */
	public static function decode(input:Bytes):RuntimeUiCatalogResult {
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(input);
		if (root == null)
			return rejected(reader);
		final fields = reader.object(root, "UI catalog", ["schemaVersion", "catalogId", "defaultLocale", "locales", "messages"]);
		if (fields == null)
			return rejected(reader);

		final versionNode = reader.field(fields, "schemaVersion");
		final version = reader.integer(versionNode, "schemaVersion", 0, 2147483647);
		if (version == null)
			return rejected(reader);
		if (version != 1) {
			reader.reject(versionNode, SchemaUnsupportedVersion("schemaVersion", 1));
			return rejected(reader);
		}
		final catalogNode = reader.field(fields, "catalogId");
		final catalogId = reader.string(catalogNode, "catalogId", 128);
		if (catalogId == null)
			return rejected(reader);
		if (catalogId != "caxecraft.ui") {
			reader.reject(catalogNode, SchemaIncompatibleTypedCatalog("catalogId"));
			return rejected(reader);
		}

		final localeNode = reader.field(fields, "locales");
		final localeValues = reader.array(localeNode, "locales", 1, 8);
		if (localeValues == null)
			return rejected(reader);
		final locales:Array<String> = [];
		for (index in 0...localeValues.length) {
			final value = reader.string(localeValues[index], "locales[" + index + "]", 128);
			if (value == null)
				return rejected(reader);
			if (!RuntimeSchemaReader.validLocale(value)) {
				reader.reject(localeValues[index], SchemaInvalidLocale("locales[" + index + "]"));
				return rejected(reader);
			}
			for (existing in locales)
				if (existing == value) {
					reader.reject(localeValues[index], SchemaInvalidLocale("locales[" + index + "]"));
					return rejected(reader);
				}
			locales.push(value);
		}
		final defaultNode = reader.field(fields, "defaultLocale");
		final defaultLocale = reader.string(defaultNode, "defaultLocale", 128);
		if (defaultLocale == null)
			return rejected(reader);
		if (!RuntimeSchemaReader.validLocale(defaultLocale) || defaultLocale != locales[0]) {
			reader.reject(defaultNode, SchemaInvalidLocale("defaultLocale"));
			return rejected(reader);
		}
		if (!sameStrings(locales, ["en", "es-MX"])) {
			reader.reject(localeNode, SchemaIncompatibleTypedCatalog("locales"));
			return rejected(reader);
		}

		final messageNode = reader.field(fields, "messages");
		final messageValues = reader.array(messageNode, "messages", 1, 128);
		if (messageValues == null)
			return rejected(reader);
		final expectedIds = expectedMessageIds();
		final expectedSymbols = expectedMessageSymbols();
		final messages:Array<RuntimeUiMessageDefinition> = [];
		for (index in 0...messageValues.length) {
			final path = "messages[" + index + "]";
			final messageFields = reader.object(messageValues[index], path, ["id", "symbol", "text"]);
			if (messageFields == null)
				return rejected(reader);
			final idNode = reader.field(messageFields, "id");
			final symbolNode = reader.field(messageFields, "symbol");
			final id = reader.string(idNode, path + ".id", 128);
			final symbol = reader.string(symbolNode, path + ".symbol", 128);
			if (id == null || symbol == null)
				return rejected(reader);
			if (!RuntimeSchemaReader.validMessageId(id)) {
				reader.reject(idNode, SchemaInvalidString(path + ".id"));
				return rejected(reader);
			}
			if (!RuntimeSchemaReader.validSymbol(symbol)) {
				reader.reject(symbolNode, SchemaInvalidString(path + ".symbol"));
				return rejected(reader);
			}
			for (existing in messages)
				if (existing.id == id || existing.symbol == symbol) {
					reader.reject(idNode, SchemaDuplicateId("messages", id));
					return rejected(reader);
				}
			if (messages.length > 0 && RuntimeSchemaReader.compareUtf8(messages[messages.length - 1].id, id) > 0) {
				reader.reject(idNode, SchemaNonCanonicalOrder("messages"));
				return rejected(reader);
			}
			if (index < expectedIds.length && id == expectedIds[index] && symbol != expectedSymbols[index]) {
				reader.reject(symbolNode, SchemaIncompatibleTypedCatalog(path));
				return rejected(reader);
			}
			final texts = readTexts(reader, reader.field(messageFields, "text"), path + ".text", locales);
			if (texts == null)
				return rejected(reader);
			messages.push(new RuntimeUiMessageDefinition(id, symbol, texts));
		}
		if (messages.length != expectedIds.length) {
			reader.reject(messageNode, SchemaIncompatibleTypedCatalog("messages"));
			return rejected(reader);
		}
		for (index in 0...messages.length)
			if (messages[index].id != expectedIds[index] || messages[index].symbol != expectedSymbols[index]) {
				reader.reject(messageValues[index], SchemaIncompatibleTypedCatalog("messages[" + index + "]"));
				return rejected(reader);
			}
		return RuntimeUiCatalogReady(new RuntimeUiCatalog(catalogId, locales, messages));
	}

	/** Return the typed API's reviewed message IDs in constructor order. */
	static function expectedMessageIds():Array<String>
		return [
			"aquatic_gear_equipped",
			"brand",
			"capture_prompt",
			"controls",
			"debug_cells",
			"debug_draws",
			"debug_frame",
			"debug_tick",
			"debug_visible",
			"editor_advanced",
			"editor_back",
			"editor_build",
			"editor_canvas_help",
			"editor_checkpoint",
			"editor_coordinates",
			"editor_erase",
			"editor_ground",
			"editor_invalid",
			"editor_keep_editing",
			"editor_leave_without_saving",
			"editor_material",
			"editor_more_details",
			"editor_name",
			"editor_new_world",
			"editor_plan",
			"editor_ready",
			"editor_redo",
			"editor_scene",
			"editor_select",
			"editor_stop_test",
			"editor_test",
			"editor_testing",
			"editor_title",
			"editor_tool_list",
			"editor_undo",
			"editor_unsaved_changes",
			"editor_valid",
			"editor_validate",
			"editor_world_list",
			"health_full",
			"menu_adventure",
			"menu_creative",
			"menu_editor",
			"menu_instructions",
			"no_block_in_reach",
			"pause_help",
			"pause_title",
			"place_blocked",
			"title_fallback"
		];

	/** Return the existing typed message constructors in matching storage order. */
	static function expectedMessageSymbols():Array<String>
		return [
			"AquaticGearEquipped",
			"Brand",
			"CapturePrompt",
			"Controls",
			"DebugCells",
			"DebugDraws",
			"DebugFrame",
			"DebugTick",
			"DebugVisible",
			"EditorAdvanced",
			"EditorBack",
			"EditorBuild",
			"EditorCanvasHelp",
			"EditorCheckpoint",
			"EditorCoordinates",
			"EditorErase",
			"EditorGround",
			"EditorInvalid",
			"EditorKeepEditing",
			"EditorLeaveWithoutSaving",
			"EditorMaterial",
			"EditorMoreDetails",
			"EditorName",
			"EditorNewWorld",
			"EditorPlan",
			"EditorReady",
			"EditorRedo",
			"EditorScene",
			"EditorSelect",
			"EditorStopTest",
			"EditorTest",
			"EditorTesting",
			"EditorTitle",
			"EditorToolList",
			"EditorUndo",
			"EditorUnsavedChanges",
			"EditorValid",
			"EditorValidate",
			"EditorWorldList",
			"HealthFull",
			"MenuAdventure",
			"MenuCreative",
			"MenuEditor",
			"MenuInstructions",
			"NoBlockInReach",
			"PauseHelp",
			"PauseTitle",
			"PlaceBlocked",
			"TitleFallback"
		];

	/** Stable catalog identity copied from the admitted document. */
	public inline function catalogId():String
		return catalogIdValue;

	/** Number of typed locales available for selection. */
	public inline function localeCount():Int
		return locales.length;

	/** Number of typed messages available at every locale. */
	public inline function messageCount():Int
		return messages.length;

	/** Return the locale selected by the validated document's default entry. */
	public inline function defaultLocale():LocaleCursor
		return LocaleCursor.Locale0;

	/** Cycle through the validated locale set without exposing its storage. */
	public function nextLocale(locale:LocaleCursor):LocaleCursor
		return switch locale {
			case Locale0: locales.length > 1 ? LocaleCursor.Locale1 : LocaleCursor.Locale0;
			case Locale1: LocaleCursor.Locale0;
			case _: LocaleCursor.Locale0;
		};

	/** Return owned text for one typed locale/message pair, or empty on invalid raw codes. */
	public function text(locale:LocaleCursor, message:UiMessage):String {
		final localeCode = localeStorageCode(locale);
		final messageCode = messageStorageCode(message);
		if (localeCode < 0 || localeCode >= locales.length || messageCode < 0 || messageCode >= messages.length)
			return "";
		return messages[messageCode].texts[localeCode];
	}

	/** Map the existing closed locale constructors without an unchecked cast. */
	static function localeStorageCode(locale:LocaleCursor):Int {
		return switch locale {
			case Locale0: 0;
			case Locale1: 1;
			case _: -1;
		};
	}

	/** Map the existing closed message constructors without an unchecked cast. */
	static function messageStorageCode(message:UiMessage):Int {
		return switch message {
			case AquaticGearEquipped: 0;
			case Brand: 1;
			case CapturePrompt: 2;
			case Controls: 3;
			case DebugCells: 4;
			case DebugDraws: 5;
			case DebugFrame: 6;
			case DebugTick: 7;
			case DebugVisible: 8;
			case EditorAdvanced: 9;
			case EditorBack: 10;
			case EditorBuild: 11;
			case EditorCanvasHelp: 12;
			case EditorCheckpoint: 13;
			case EditorCoordinates: 14;
			case EditorErase: 15;
			case EditorGround: 16;
			case EditorInvalid: 17;
			case EditorKeepEditing: 18;
			case EditorLeaveWithoutSaving: 19;
			case EditorMaterial: 20;
			case EditorMoreDetails: 21;
			case EditorName: 22;
			case EditorNewWorld: 23;
			case EditorPlan: 24;
			case EditorReady: 25;
			case EditorRedo: 26;
			case EditorScene: 27;
			case EditorSelect: 28;
			case EditorStopTest: 29;
			case EditorTest: 30;
			case EditorTesting: 31;
			case EditorTitle: 32;
			case EditorToolList: 33;
			case EditorUndo: 34;
			case EditorUnsavedChanges: 35;
			case EditorValid: 36;
			case EditorValidate: 37;
			case EditorWorldList: 38;
			case HealthFull: 39;
			case MenuAdventure: 40;
			case MenuCreative: 41;
			case MenuEditor: 42;
			case MenuInstructions: 43;
			case NoBlockInReach: 44;
			case PauseHelp: 45;
			case PauseTitle: 46;
			case PlaceBlocked: 47;
			case TitleFallback: 48;
			case _: -1;
		};
	}

	/** Parse an exact translation object whose keys equal the admitted locales. */
	static function readTexts(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, locales:Array<String>):Null<Array<String>> {
		return switch node.value {
			case JsonObject(fields): readTextFields(reader, node, path, locales, fields);
			case _:
				reader.reject(node, SchemaWrongType(path, "locale text object"));
				null;
		};
	}

	/** Validate one translation object's exact locale keys and bounded texts. */
	static function readTextFields(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String, locales:Array<String>,
			fields:Array<ContentJsonField>):Null<Array<String>> {
		if (fields.length != locales.length) {
			reader.reject(node, SchemaInvalidLocale(path));
			return null;
		}
		for (field in fields) {
			var admitted = false;
			for (locale in locales)
				if (field.name == locale)
					admitted = true;
			if (!admitted) {
				reader.rejectAt(field.line, field.column, SchemaInvalidLocale(path + "." + field.name));
				return null;
			}
		}
		final result:Array<String> = [];
		for (locale in locales) {
			var found:Null<ContentJsonNode> = null;
			for (field in fields)
				if (field.name == locale)
					found = field.value;
			if (found == null) {
				reader.reject(node, SchemaInvalidLocale(path + "." + locale));
				return null;
			}
			final text = readText(reader, found, path + "." + locale);
			if (text == null)
				return null;
			result.push(text);
		}
		return result;
	}

	/** Parse one non-empty control-free display string within 240 UTF-8 bytes. */
	static function readText(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = switch node.value {
			case JsonString(text): text;
			case _:
				reader.reject(node, SchemaWrongType(path, "display string"));
				return null;
		};
		if (value.length == 0 || Bytes.ofString(value).length > 240 || RuntimeSchemaReader.hasControl(value)) {
			reader.reject(node, SchemaInvalidText(path));
			return null;
		}
		return value;
	}

	/** Compare two small ordered String arrays without exposing either one. */
	static function sameStrings(left:Array<String>, right:Array<String>):Bool {
		if (left.length != right.length)
			return false;
		for (index in 0...left.length)
			if (left[index] != right[index])
				return false;
		return true;
	}

	/** Return the reader's first failure, with an unreachable defensive fallback. */
	static function rejected(reader:RuntimeSchemaReader):RuntimeUiCatalogResult {
		final diagnostic = reader.failure;
		return diagnostic == null ? RuntimeUiCatalogRejected({
			line: 1,
			column: 1,
			kind: SchemaInvalidInvariant("UI decoder")
		}) : RuntimeUiCatalogRejected(diagnostic);
	}
}

/** One typed message identity and locale-ordered owned text vector. */
private final class RuntimeUiMessageDefinition {
	/** Stable JSON message ID. */
	public final id:String;

	/** Existing typed `UiMessage` constructor spelling. */
	public final symbol:String;

	/** One complete text per catalog locale; never exposed as an Array. */
	public final texts:Array<String>;

	/** Construct one complete message after locale validation. */
	public function new(id:String, symbol:String, texts:Array<String>) {
		this.id = id;
		this.symbol = symbol;
		this.texts = texts;
	}
}
