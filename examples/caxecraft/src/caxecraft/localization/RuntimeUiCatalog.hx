package caxecraft.localization;

import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import caxecraft.scenario.MessageId;
import haxe.io.Bytes;

/**
 * Admits the shipped UI JSON into an immutable runtime text catalog.
 *
 * Locale and message arrays remain private. Typed call sites carry stable data
 * keys, while the JSON catalog alone owns message membership, ordering, and
 * translated prose. Text remains owned Haxe `String`; converting it to a native
 * rendering borrow belongs to the later publication/application boundary.
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
 * Parameterized templates use stable message IDs and checked replacement
 * slots, so diagnostics and editor cards do not duplicate translated prose in
 * authored Haxe.
 */
final class RuntimeUiCatalog {
	final catalogIdValue:String;
	final locales:Array<String>;
	final messages:Array<RuntimeUiMessageDefinition>;
	final templates:Array<RuntimeUiTemplateDefinition>;

	/** Construct one complete catalog after every candidate check passed. */
	public function new(catalogId:String, locales:Array<String>, messages:Array<RuntimeUiMessageDefinition>, templates:Array<RuntimeUiTemplateDefinition>) {
		this.catalogIdValue = catalogId;
		this.locales = locales;
		this.messages = messages;
		this.templates = templates;
	}

	/** Decode one complete UI candidate without filesystem or renderer authority. */
	public static function decode(input:Bytes):RuntimeUiCatalogResult {
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(input);
		if (root == null)
			return rejected(reader);
		final fields = reader.object(root, "UI catalog", [
			"schemaVersion",
			"catalogId",
			"defaultLocale",
			"locales",
			"messages",
			"templates"
		]);
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
		final messageNode = reader.field(fields, "messages");
		final messageValues = reader.array(messageNode, "messages", 1, 128);
		if (messageValues == null)
			return rejected(reader);
		final messages:Array<RuntimeUiMessageDefinition> = [];
		for (index in 0...messageValues.length) {
			final path = "messages[" + index + "]";
			final messageFields = reader.object(messageValues[index], path, ["id", "text"]);
			if (messageFields == null)
				return rejected(reader);
			final idNode = reader.field(messageFields, "id");
			final id = reader.string(idNode, path + ".id", 128);
			if (id == null)
				return rejected(reader);
			if (!RuntimeSchemaReader.validMessageId(id)) {
				reader.reject(idNode, SchemaInvalidString(path + ".id"));
				return rejected(reader);
			}
			for (existing in messages)
				if (existing.id == id) {
					reader.reject(idNode, SchemaDuplicateId("messages", id));
					return rejected(reader);
				}
			if (messages.length > 0 && RuntimeSchemaReader.compareUtf8(messages[messages.length - 1].id, id) > 0) {
				reader.reject(idNode, SchemaNonCanonicalOrder("messages"));
				return rejected(reader);
			}
			final texts = readTexts(reader, reader.field(messageFields, "text"), path + ".text", locales);
			if (texts == null)
				return rejected(reader);
			messages.push(new RuntimeUiMessageDefinition(id, texts));
		}
		final templates = readTemplates(reader, reader.field(fields, "templates"), locales);
		if (templates == null)
			return rejected(reader);
		return RuntimeUiCatalogReady(new RuntimeUiCatalog(catalogId, locales, messages, templates));
	}

	/** Stable catalog identity copied from the admitted document. */
	public inline function catalogId():String
		return catalogIdValue;

	/** Number of typed locales available for selection. */
	public inline function localeCount():Int
		return locales.length;

	/** Number of typed messages available at every locale. */
	public inline function messageCount():Int
		return messages.length;

	/** Number of data-owned parameterized messages available at every locale. */
	public inline function templateCount():Int
		return templates.length;

	/** True only when one validated parameterized message exists in every locale. */
	public function hasTemplate(message:MessageId):Bool
		return templateFor(message.text()) != null;

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

	/** Return owned text for one typed locale/message pair, or empty when absent. */
	public function text(locale:LocaleCursor, message:UiMessage):String {
		final localeCode = localeStorageCode(locale);
		if (localeCode < 0 || localeCode >= locales.length)
			return "";
		final definition = messageFor(message.text());
		return definition == null ? "" : definition.texts[localeCode];
	}

	/** Resolve one stable template ID and substitute its ordered typed arguments. */
	public function format(locale:LocaleCursor, message:MessageId, arguments:Array<String>):String {
		final localeCode = localeStorageCode(locale);
		if (localeCode < 0 || localeCode >= locales.length)
			return "";
		final template = templateFor(message.text());
		if (template == null || arguments.length != template.argumentCount)
			return "";
		var result = template.texts[localeCode];
		for (index in 0...arguments.length)
			result = replacePlaceholder(result, index, arguments[index]);
		return result;
	}

	/** Replace every one-digit slot in one bounded pass without an Array join. */
	static function replacePlaceholder(text:String, slot:Int, value:String):String {
		var result = "";
		var index = 0;
		while (index < text.length) {
			if (index + 2 < text.length
				&& text.charCodeAt(index) == 0x7b
				&& text.charCodeAt(index + 1) == 0x30 + slot
				&& text.charCodeAt(index + 2) == 0x7d) {
				result += value;
				index += 3;
			} else {
				result += text.charAt(index);
				index++;
			}
		}
		return result;
	}

	/** Find one canonical template in logarithmic time without a mutable map. */
	function templateFor(expected:String):Null<RuntimeUiTemplateDefinition> {
		var low = 0;
		var high = templates.length - 1;
		while (low <= high) {
			final middle = low + Std.int((high - low) / 2);
			final candidate = templates[middle];
			final ordering = RuntimeSchemaReader.compareUtf8(candidate.id, expected);
			if (ordering == 0)
				return candidate;
			if (ordering < 0)
				low = middle + 1;
			else
				high = middle - 1;
		}
		return null;
	}

	/** Find one canonical non-parameterized message by its stable data key. */
	function messageFor(expected:String):Null<RuntimeUiMessageDefinition> {
		var low = 0;
		var high = messages.length - 1;
		while (low <= high) {
			final middle = low + Std.int((high - low) / 2);
			final candidate = messages[middle];
			final ordering = RuntimeSchemaReader.compareUtf8(candidate.id, expected);
			if (ordering == 0)
				return candidate;
			if (ordering < 0)
				low = middle + 1;
			else
				high = middle - 1;
		}
		return null;
	}

	/** Map the existing closed locale constructors without an unchecked cast. */
	static function localeStorageCode(locale:LocaleCursor):Int {
		return switch locale {
			case Locale0: 0;
			case Locale1: 1;
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

	/** Decode canonically ordered parameterized messages without Haxe symbols. */
	static function readTemplates(reader:RuntimeSchemaReader, node:ContentJsonNode, locales:Array<String>):Null<Array<RuntimeUiTemplateDefinition>> {
		final values = reader.array(node, "templates", 1, 256);
		if (values == null)
			return null;
		final result:Array<RuntimeUiTemplateDefinition> = [];
		for (index in 0...values.length) {
			final path = 'templates[$index]';
			final fields = reader.object(values[index], path, ["id", "text"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = reader.string(idNode, path + ".id", 128);
			if (id == null)
				return null;
			if (!RuntimeSchemaReader.validMessageId(id)) {
				reader.reject(idNode, SchemaInvalidString(path + ".id"));
				return null;
			}
			if (result.length > 0 && RuntimeSchemaReader.compareUtf8(result[result.length - 1].id, id) >= 0) {
				reader.reject(idNode, result[result.length - 1].id == id ? SchemaDuplicateId("templates", id) : SchemaNonCanonicalOrder("templates"));
				return null;
			}
			final texts = readTexts(reader, reader.field(fields, "text"), path + ".text", locales);
			if (texts == null)
				return null;
			final argumentCount = placeholderArgumentCount(texts);
			if (argumentCount < 0) {
				reader.reject(values[index], SchemaIncompatibleTypedCatalog(path + ".text"));
				return null;
			}
			result.push(new RuntimeUiTemplateDefinition(id, texts, argumentCount));
		}
		return result;
	}

	/** Require every locale to retain the same bounded replacement slots. */
	static function placeholderArgumentCount(texts:Array<String>):Int {
		if (texts.length == 0)
			return -1;
		for (text in texts)
			if (!validPlaceholderTokens(text))
				return -1;
		var argumentCount = 0;
		for (slot in 0...10) {
			final expected = placeholderCount(texts[0], slot);
			for (index in 1...texts.length)
				if (placeholderCount(texts[index], slot) != expected)
					return -1;
			if (expected > 0) {
				if (slot != argumentCount)
					return -1;
				argumentCount++;
			}
		}
		return argumentCount;
	}

	/** Reject unmatched braces and replacement slots outside the bounded 0..9 set. */
	static function validPlaceholderTokens(text:String):Bool {
		var index = 0;
		while (index < text.length) {
			final code = text.charCodeAt(index);
			if (code == 0x7b) {
				if (index + 2 >= text.length || text.charCodeAt(index + 1) < 0x30 || text.charCodeAt(index + 1) > 0x39 || text.charCodeAt(index + 2) != 0x7d)
					return false;
				index += 3;
			} else {
				if (code == 0x7d)
					return false;
				index++;
			}
		}
		return true;
	}

	/** Count one exact replacement token without regular expressions or locale rules. */
	static function placeholderCount(text:String, slot:Int):Int {
		final token = '{$slot}';
		var count = 0;
		var offset = 0;
		while (offset <= text.length - token.length) {
			final found = text.indexOf(token, offset);
			if (found < 0)
				break;
			count++;
			offset = found + token.length;
		}
		return count;
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

/** One data-owned message identity and locale-ordered text vector. */
private final class RuntimeUiMessageDefinition {
	/** Stable JSON message ID. */
	public final id:String;

	/** One complete text per catalog locale; never exposed as an Array. */
	public final texts:Array<String>;

	/** Construct one complete message after locale validation. */
	public function new(id:String, texts:Array<String>) {
		this.id = id;
		this.texts = texts;
	}
}

/** One stable parameterized message with locale-ordered data-owned text. */
private final class RuntimeUiTemplateDefinition {
	public final id:String;
	public final texts:Array<String>;
	public final argumentCount:Int;

	/** Construct one complete template after locale and placeholder validation. */
	public function new(id:String, texts:Array<String>, argumentCount:Int) {
		this.id = id;
		this.texts = texts;
		this.argumentCount = argumentCount;
	}
}
