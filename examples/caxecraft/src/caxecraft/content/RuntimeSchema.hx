package caxecraft.content;

import caxecraft.content.ContentJson.ContentJsonErrorKind;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonReadResult;
import caxecraft.content.ContentJson.ContentJsonValue;
import haxe.io.Bytes;

/**
 * Supplies exact, located checks shared by Caxecraft's runtime JSON schemas.
 *
 * `ContentJson` owns syntax and resource bounds. This layer owns schema shape:
 * exact fields, scalar types, bounded integers, canonical identifiers, and
 * source locations. It never constructs gameplay values itself, so pack and UI
 * decoders remain the owners of their distinct cross-reference invariants.
 */
/** Closed reasons a parsed JSON tree cannot become trusted runtime content. */
enum RuntimeSchemaErrorKind {
	/** JSON syntax or resource admission failed before schema inspection. */
	SchemaJsonSyntax(error:ContentJsonErrorKind);

	/** A schema object was required at this path. */
	SchemaExpectedObject(path:String);

	/** A schema array was required at this path. */
	SchemaExpectedArray(path:String);

	/** An exact object omitted one required field. */
	SchemaMissingField(path:String, field:String);

	/** An exact object supplied one field outside its closed schema. */
	SchemaUnknownField(path:String, field:String);

	/** A value used the wrong JSON scalar or container kind. */
	SchemaWrongType(path:String, expected:String);

	/** A document named a schema version this runtime does not admit. */
	SchemaUnsupportedVersion(path:String, expected:Int);

	/** A String violated its non-empty, byte, control, or identifier contract. */
	SchemaInvalidString(path:String);

	/** A number was not an exact bounded integer. */
	SchemaInvalidInteger(path:String, minimum:Int, maximum:Int);

	/** A namespaced content ID did not use the canonical spelling. */
	SchemaInvalidContentId(path:String);

	/** A package logical path was not canonical and relative. */
	SchemaInvalidLogicalPath(path:String);

	/** A closed mechanic or profile name was not admitted. */
	SchemaInvalidClosedValue(path:String, value:String);

	/** One typed array repeated an ID. */
	SchemaDuplicateId(path:String, id:String);

	/** One closed package list repeated the same scalar value. */
	SchemaDuplicateValue(path:String, value:String);

	/** Two campaign levels claimed the same package-relative map path. */
	SchemaDuplicateLogicalPath(path:String);

	/** Two portable package paths differ only by ASCII letter case. */
	SchemaCaseCollidingLogicalPath(path:String, previous:String);

	/** One package entry requires a semantic role the package did not declare. */
	SchemaRoleEntryMismatch(path:String, role:String);

	/** One declared package role has no payload entry to fulfill it. */
	SchemaMissingRoleEntry(role:String);

	/** One package dependency identity appeared more than once. */
	SchemaDuplicateDependency(id:String);

	/** A package attempted to depend on its own stable identity. */
	SchemaSelfDependency(id:String);

	/** One inclusive dependency interval placed its minimum after its maximum. */
	SchemaIncompatibleVersionBounds(id:String, minimum:Int, maximum:Int);

	/** Declared package receipts exceeded the bounded total verification budget. */
	SchemaPackageByteBudgetExceeded(maximum:Int);

	/** One typed array was not ascending by exact UTF-8 bytes. */
	SchemaNonCanonicalOrder(path:String);

	/** Two distinct content kinds claimed the same stable ID. */
	SchemaCrossKindId(id:String);

	/** Two terrain definitions claimed the same compact byte. */
	SchemaDuplicateStorageCode(code:Int);

	/** Individually valid values violated one schema-level relationship. */
	SchemaInvalidInvariant(path:String);

	/** Required campaign transitions formed a cycle with no terminal level. */
	SchemaRequiredTransitionCycle(levelId:String);

	/** A stable ID did not exist in any admitted content kind. */
	SchemaUnresolvedReference(path:String, id:String, expectedKind:String);

	/** A stable ID existed, but in a different closed content kind. */
	SchemaWrongReferenceKind(path:String, id:String, actualKind:String);

	/** A presentation named no reviewed visual asset. */
	SchemaUnknownAsset(path:String, id:String);

	/** A presentation named no reviewed cell in its selected asset. */
	SchemaUnknownAssetCell(path:String, asset:String, cell:String);

	/** A reserved kind became non-empty before the engine admitted it. */
	SchemaUnsupportedReservedKind(path:String);

	/** A locale ID, default, order, or translation key was invalid. */
	SchemaInvalidLocale(path:String);

	/** The runtime JSON no longer matched the current typed locale/message API. */
	SchemaIncompatibleTypedCatalog(path:String);

	/** Player-visible text violated its non-empty, control, or byte bound. */
	SchemaInvalidText(path:String);
}

/** One fail-closed schema reason at a one-based JSON source coordinate. */
typedef RuntimeSchemaDiagnostic = {
	/** One-based source line. */
	final line:Int;

	/** One-based source column. */
	final column:Int;

	/** Stable closed reason for Eval/native comparison. */
	final kind:RuntimeSchemaErrorKind;
}

/**
 * Stateful first-error reader for one parsed schema document.
 *
 * A class is appropriate because all field checks in one request share one
 * terminal diagnostic. The reader never throws and records only the first
 * failure, which keeps diagnostics deterministic and prevents partial typed
 * values from escaping after a later error.
 */
final class RuntimeSchemaReader {
	/** First schema failure, or null while admission may continue. */
	public var failure(default, null):Null<RuntimeSchemaDiagnostic> = null;

	/** Construct fresh request-local schema state. */
	public function new() {}

	/** Parse bounded JSON and translate syntax failure into the shared model. */
	public function parse(input:Bytes):Null<ContentJsonNode> {
		return switch ContentJson.read(input) {
			case ContentJsonReady(root): root;
			case ContentJsonRejected(diagnostic):
				rejectAt(diagnostic.line, diagnostic.column, SchemaJsonSyntax(diagnostic.kind));
				null;
		};
	}

	/** Require one object with exactly the supplied field names. */
	public function object(node:ContentJsonNode, path:String, expected:Array<String>):Null<Array<ContentJsonField>> {
		return switch node.value {
			case JsonObject(fields): exactObject(node, path, expected, fields);
			case _:
				reject(node, SchemaExpectedObject(path));
				null;
		};
	}

	/** Compare one object field set after the closed JSON kind is known. */
	function exactObject(node:ContentJsonNode, path:String, expected:Array<String>, fields:Array<ContentJsonField>):Null<Array<ContentJsonField>> {
		for (field in fields) {
			var known = false;
			for (name in expected)
				if (field.name == name)
					known = true;
			if (!known) {
				rejectAt(field.line, field.column, SchemaUnknownField(path, field.name));
				return null;
			}
		}
		for (name in expected) {
			var present = false;
			for (field in fields)
				if (field.name == name)
					present = true;
			if (!present) {
				reject(node, SchemaMissingField(path, name));
				return null;
			}
		}
		return fields;
	}

	/** Return one field from an object already admitted by `object`. */
	public function field(fields:Array<ContentJsonField>, name:String):Null<ContentJsonNode> {
		for (field in fields)
			if (field.name == name)
				return field.value;
		return null;
	}

	/** Require an array whose count falls inside the schema bounds. */
	public function array(node:ContentJsonNode, path:String, minimum:Int, maximum:Int):Null<Array<ContentJsonNode>> {
		return switch node.value {
			case JsonArray(values): boundedArray(node, path, minimum, maximum, values);
			case _:
				reject(node, SchemaExpectedArray(path));
				null;
		};
	}

	/** Check one array's count after its closed JSON kind is known. */
	function boundedArray(node:ContentJsonNode, path:String, minimum:Int, maximum:Int, values:Array<ContentJsonNode>):Null<Array<ContentJsonNode>> {
		if (values.length < minimum || values.length > maximum) {
			reject(node, SchemaInvalidInvariant(path));
			return null;
		}
		return values;
	}

	/** Require one non-empty scalar-valid String within a UTF-8 byte bound. */
	public function string(node:ContentJsonNode, path:String, maximumBytes:Int):Null<String> {
		return switch node.value {
			case JsonString(value): boundedString(node, path, maximumBytes, value);
			case _:
				reject(node, SchemaWrongType(path, "string"));
				null;
		};
	}

	/** Check one String after its closed JSON kind is known. */
	function boundedString(node:ContentJsonNode, path:String, maximumBytes:Int, value:String):Null<String> {
		if (value.length == 0 || Bytes.ofString(value).length > maximumBytes || hasControl(value)) {
			reject(node, SchemaInvalidString(path));
			return null;
		}
		return value;
	}

	/** Require one exact decimal integer without target-dependent float parsing. */
	public function integer(node:ContentJsonNode, path:String, minimum:Int, maximum:Int):Null<Int> {
		return switch node.value {
			case JsonNumber(lexeme): boundedInteger(node, path, minimum, maximum, lexeme);
			case _:
				reject(node, SchemaWrongType(path, "integer"));
				null;
		};
	}

	/** Parse one number lexeme after its closed JSON kind is known. */
	function boundedInteger(node:ContentJsonNode, path:String, minimum:Int, maximum:Int, lexeme:String):Null<Int> {
		if (lexeme.length == 0 || lexeme.charCodeAt(0) == 0x2d) {
			reject(node, SchemaInvalidInteger(path, minimum, maximum));
			return null;
		}
		var value = 0;
		for (index in 0...lexeme.length) {
			final code = lexeme.charCodeAt(index);
			if (code < 0x30 || code > 0x39) {
				reject(node, SchemaInvalidInteger(path, minimum, maximum));
				return null;
			}
			final digit = code - 0x30;
			if (value > Std.int((maximum - digit) / 10)) {
				reject(node, SchemaInvalidInteger(path, minimum, maximum));
				return null;
			}
			value = value * 10 + digit;
		}
		if (value < minimum || value > maximum) {
			reject(node, SchemaInvalidInteger(path, minimum, maximum));
			return null;
		}
		return value;
	}

	/** Require one JSON Boolean. */
	public function boolean(node:ContentJsonNode, path:String):Null<Bool> {
		return switch node.value {
			case JsonBoolean(value): value;
			case _:
				reject(node, SchemaWrongType(path, "boolean"));
				null;
		};
	}

	/** Require null or one canonical content ID. */
	public function nullableContentId(node:ContentJsonNode, path:String):Null<LocatedOptionalString> {
		return switch node.value {
			case JsonNull: {value: null, line: node.line, column: node.column};
			case JsonString(value):
				if (!validContentId(value)) {
					reject(node, SchemaInvalidContentId(path));
					null;
				} else {
					{value: value, line: node.line, column: node.column};
				}
			case _:
				reject(node, SchemaWrongType(path, "content ID or null"));
				null;
		};
	}

	/** Record one node-located failure, preserving an earlier reason. */
	public function reject(node:ContentJsonNode, kind:RuntimeSchemaErrorKind):Void
		rejectAt(node.line, node.column, kind);

	/** Record one explicit source coordinate, preserving an earlier reason. */
	public function rejectAt(line:Int, column:Int, kind:RuntimeSchemaErrorKind):Void {
		if (failure == null)
			failure = {line: line, column: column, kind: kind};
	}

	/** True only for the content schema's canonical namespaced ID grammar. */
	public static function validContentId(value:String):Bool {
		final colon = value.indexOf(":");
		if (colon < 1 || colon > 32 || value.indexOf(":", colon + 1) >= 0 || colon + 1 >= value.length)
			return false;
		if (!isLower(value.charCodeAt(0)))
			return false;
		for (index in 1...colon) {
			final code = value.charCodeAt(index);
			if (!isLower(code) && !isDigit(code) && code != 0x2d)
				return false;
		}
		return validSegmentedLower(value, colon + 1, value.length, true);
	}

	/** True only for lower-case profile tokens separated by single hyphens. */
	public static function validProfile(value:String):Bool
		return validSegmentedLower(value, 0, value.length, false);

	/** True only for a canonical package-relative path with at least two parts. */
	public static function validLogicalPath(value:String):Bool {
		var start = 0;
		var parts = 0;
		for (index in 0...value.length + 1) {
			if (index == value.length || value.charCodeAt(index) == 0x2f) {
				final length = index - start;
				if (length < 1 || length > 64 || !isLower(value.charCodeAt(start)))
					return false;
				for (cursor in start + 1...index) {
					final code = value.charCodeAt(cursor);
					if (!isLower(code) && !isDigit(code) && code != 0x2d)
						return false;
				}
				parts++;
				start = index + 1;
			}
		}
		return parts >= 2;
	}

	/** True for the reusable UI catalog's exact locale spelling. */
	public static function validLocale(value:String):Bool {
		if (value.length == 2)
			return isLower(value.charCodeAt(0)) && isLower(value.charCodeAt(1));
		return value.length == 5 && isLower(value.charCodeAt(0)) && isLower(value.charCodeAt(1)) && value.charCodeAt(2) == 0x2d
			&& isUpper(value.charCodeAt(3)) && isUpper(value.charCodeAt(4));
	}

	/** True for a canonical lower-case message ID with bounded separators. */
	public static function validMessageId(value:String):Bool
		return validSegmentedLower(value, 0, value.length, true);

	/** True for a bounded Haxe constructor spelling used by the typed catalog. */
	public static function validSymbol(value:String):Bool {
		if (value.length < 1 || value.length > 64 || !isUpper(value.charCodeAt(0)))
			return false;
		for (index in 1...value.length) {
			final code = value.charCodeAt(index);
			if (!isLower(code) && !isUpper(code) && !isDigit(code))
				return false;
		}
		return true;
	}

	/** Compare two strings by exact UTF-8 bytes, independent of host collation. */
	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final shared = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...shared) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	/** True when a decoded string contains a schema-disallowed control scalar. */
	public static function hasControl(value:String):Bool {
		for (index in 0...value.length)
			if (value.charCodeAt(index) < 0x20)
				return true;
		return false;
	}

	/** Validate one lower-case identifier body with optional separators. */
	static function validSegmentedLower(value:String, start:Int, end:Int, broadSeparators:Bool):Bool {
		if (start >= end || !isLower(value.charCodeAt(start)))
			return false;
		var separator = false;
		for (index in start + 1...end) {
			final code = value.charCodeAt(index);
			if (isLower(code) || isDigit(code)) {
				separator = false;
			} else if (code == 0x2d || broadSeparators && (code == 0x2e || code == 0x5f)) {
				if (separator || index + 1 == end)
					return false;
				separator = true;
			} else {
				return false;
			}
		}
		return true;
	}

	/** True for one ASCII lower-case letter. */
	static inline function isLower(code:Int):Bool
		return code >= 0x61 && code <= 0x7a;

	/** True for one ASCII upper-case letter. */
	static inline function isUpper(code:Int):Bool
		return code >= 0x41 && code <= 0x5a;

	/** True for one ASCII decimal digit. */
	static inline function isDigit(code:Int):Bool
		return code >= 0x30 && code <= 0x39;
}

/** A nullable reference plus the exact location of its JSON scalar. */
typedef LocatedOptionalString = {
	/** Decoded ID, or null when JSON explicitly used null. */
	final value:Null<String>;

	/** One-based source line. */
	final line:Int;

	/** One-based source column. */
	final column:Int;
}
