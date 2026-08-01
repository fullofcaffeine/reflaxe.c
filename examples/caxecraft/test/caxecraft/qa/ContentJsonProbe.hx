package caxecraft.qa;

import caxecraft.content.ContentJson;
import caxecraft.content.ContentJson.ContentJsonErrorKind;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonReadResult;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.domain.CaxecraftTrace;
import haxe.io.Bytes;

/**
 * Proves that package JSON has one bounded typed meaning in Eval and native C.
 *
 * The positive document exercises every JSON kind and Unicode escapes. The
 * negative documents prove duplicate fields, malformed UTF-8, invalid numbers,
 * trailing separators, and excessive nesting are rejected before a schema can
 * receive a partial value tree.
 */
/** First broken invariant, or zero after every parser check passes. */
var observed:Int = 0;

/** Deterministic digest of the successfully parsed closed value tree. */
var traceTree:Int = 0;

/** Source line of the duplicate-field diagnostic. */
var traceDuplicateLine:Int = 0;

/** Byte offset reported for malformed UTF-8. */
var traceUtf8Offset:Int = 0;

/** Run the shared specification under Eval or export it to the C harness. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(traceTree);
	Sys.println(traceDuplicateLine);
	Sys.println(traceUtf8Offset);
	#end
}

/** Return zero, or the stable number of the first broken parser invariant. */
function selfCheck():Int {
	final valid = Bytes.ofString('{"array":[true,false,null,-12,3.5e2],"message":"Caxe \\uD83D\\uDE80","schemaVersion":1}');
	final root = switch ContentJson.read(valid) {
		case ContentJsonReady(value): value;
		case ContentJsonRejected(_): return 1;
	};
	return checkRoot(root);
}

/** Check the positive tree before running the bounded negative corpus. */
function checkRoot(root:ContentJsonNode):Int {
	return switch root.value {
		case JsonObject(fields): checkObjectFields(root, fields);
		case _: 2;
	};
}

/** Check the expected root fields without carrying a managed Array through a join. */
function checkObjectFields(root:ContentJsonNode, fields:Array<ContentJsonField>):Int {
	if (fields.length != 3 || fields[0].name != "array" || fields[1].name != "message" || fields[2].name != "schemaVersion")
		return 3;
	return switch fields[0].value.value {
		case JsonArray(array): checkArrayAndNegativeCorpus(root, fields, array);
		case _: 4;
	};
}

/** Check nested JSON values, diagnostics, and input bounds. */
function checkArrayAndNegativeCorpus(root:ContentJsonNode, fields:Array<ContentJsonField>, array:Array<ContentJsonNode>):Int {
	if (array.length != 5)
		return 5;
	switch array[0].value {
		case JsonBoolean(true):
		case _:
			return 6;
	}
	switch array[2].value {
		case JsonNull:
		case _:
			return 7;
	}
	switch array[3].value {
		case JsonNumber("-12"):
		case _:
			return 8;
	}
	switch fields[1].value.value {
		case JsonString(value) if (value == "Caxe 🚀"):
		case _:
			return 9;
	}
	final positiveStatus = checkRemainingPositiveKinds(root, fields, array);
	if (positiveStatus != 0)
		return positiveStatus;
	traceTree = digestNode(root);

	switch ContentJson.read(Bytes.ofString('{\n  "same": 1,\n  "same": 2\n}')) {
		case ContentJsonRejected(diagnostic):
			switch diagnostic.kind {
				case JsonDuplicateField("same"):
					traceDuplicateLine = diagnostic.line;
					if (diagnostic.line != 3) return 10;
				case _:
					return 11;
			}
		case ContentJsonReady(_):
			return 12;
	}

	final malformed = Bytes.alloc(2);
	malformed.set(0, 0xc3);
	malformed.set(1, 0x28);
	switch ContentJson.read(malformed) {
		case ContentJsonRejected(diagnostic):
			switch diagnostic.kind {
				case JsonMalformedUtf8(offset):
					traceUtf8Offset = offset;
					if (offset != 1) return 13;
				case _:
					return 14;
			}
		case ContentJsonReady(_):
			return 15;
	}
	if (!rejects(Bytes.ofString('{"number":01}'), JsonInvalidNumber))
		return 16;
	if (!rejects(Bytes.ofString('{"items":[1,]}'), JsonInvalidSeparator))
		return 17;
	if (!rejects(Bytes.ofString(depthDocument(ContentJson.MAXIMUM_DEPTH + 1)), JsonDepthExceeded(ContentJson.MAXIMUM_DEPTH)))
		return 18;
	if (traceTree == 0 || traceDuplicateLine != 3 || traceUtf8Offset != 1)
		return 19;
	return checkRemainingNegativeFamilies();
}

/** Verify every positive JSON kind and scalar-based source coordinates. */
function checkRemainingPositiveKinds(root:ContentJsonNode, fields:Array<ContentJsonField>, array:Array<ContentJsonNode>):Int {
	if (root.line != 1 || root.column != 1)
		return 20;
	switch array[1].value {
		case JsonBoolean(false):
		case _:
			return 21;
	}
	switch array[4].value {
		case JsonNumber("3.5e2"):
		case _:
			return 22;
	}
	switch fields[2].value.value {
		case JsonNumber("1"):
		case _:
			return 23;
	}
	final located = switch ContentJson.read(Bytes.ofString('{\n  "é": "🚀"\n}')) {
		case ContentJsonReady(value): value;
		case ContentJsonRejected(_): return 24;
	};
	return switch located.value {
		case JsonObject(locatedFields) if (locatedFields.length == 1):
			final field = locatedFields[0];
			if (field.line != 2 || field.column != 3 || field.value.line != 2 || field.value.column != 8) 25; else switch field.value.value {
				case JsonString("🚀"): 0;
				case _: 26;
			};
		case _:
			27;
	};
}

/** Exercise the remaining syntax and resource-bound rejection families. */
function checkRemainingNegativeFamilies():Int {
	if (!rejects(Bytes.ofString('"\\uD800"'), JsonInvalidEscape))
		return 28;
	if (!rejects(Bytes.ofString("true false"), JsonTrailingContent))
		return 29;
	if (!rejects(Bytes.alloc(ContentJson.MAXIMUM_BYTES + 1), JsonFileTooLarge(ContentJson.MAXIMUM_BYTES)))
		return 30;
	if (!rejects(Bytes.ofString(arrayDocument(ContentJson.MAXIMUM_COLLECTION_ENTRIES + 1)),
		JsonCollectionLimitExceeded(ContentJson.MAXIMUM_COLLECTION_ENTRIES)))
		return 31;
	if (!rejects(Bytes.ofString(nodeCountDocument(ContentJson.MAXIMUM_NODES + 1)), JsonNodeLimitExceeded(ContentJson.MAXIMUM_NODES)))
		return 32;
	if (!rejects(Bytes.ofString(""), JsonExpectedValue))
		return 33;
	if (!rejects(Bytes.ofString("tru"), JsonUnexpectedEnd))
		return 34;
	if (!rejects(Bytes.ofString("?"), JsonUnexpectedCharacter))
		return 35;
	if (!rejects(Bytes.ofString("\"bad\nstring\""), JsonInvalidString))
		return 36;
	if (!rejects(Bytes.ofString('"\\u12x4"'), JsonInvalidEscape))
		return 37;
	if (!rejects(Bytes.ofString("1e+"), JsonInvalidNumber))
		return 38;
	return checkLimitEdgesAndLineEndings();
}

/** Prove every configured ceiling accepts its edge and rejects one step beyond. */
function checkLimitEdgesAndLineEndings():Int {
	if (!accepts(byteLimitDocument()))
		return 39;
	if (!accepts(Bytes.ofString(depthDocument(ContentJson.MAXIMUM_DEPTH))))
		return 40;
	if (!accepts(Bytes.ofString(arrayDocument(ContentJson.MAXIMUM_COLLECTION_ENTRIES))))
		return 41;
	if (!accepts(Bytes.ofString(nodeCountDocument(ContentJson.MAXIMUM_NODES))))
		return 42;
	if (duplicateLine('{\r  "same": 1,\r  "same": 2\r}') != 3)
		return 43;
	if (duplicateLine('{\r\n  "same": 1,\r\n  "same": 2\r\n}') != 3)
		return 44;
	return 0;
}

/** True only when one complete document escapes as a typed tree. */
function accepts(bytes:Bytes):Bool {
	return switch ContentJson.read(bytes) {
		case ContentJsonReady(_): true;
		case ContentJsonRejected(_): false;
	};
}

/** Return the duplicate-name line for one line-ending spelling. */
function duplicateLine(source:String):Int {
	return switch ContentJson.read(Bytes.ofString(source)) {
		case ContentJsonRejected(diagnostic):
			switch diagnostic.kind {
				case JsonDuplicateField("same"): diagnostic.line;
				case _: -1;
			}
		case ContentJsonReady(_): -1;
	};
}

/** Check one negative document without weakening the exact expected reason. */
function rejects(bytes:Bytes, expected:ContentJsonErrorKind):Bool {
	return switch ContentJson.read(bytes) {
		case ContentJsonRejected(diagnostic): sameError(diagnostic.kind, expected);
		case ContentJsonReady(_): false;
	};
}

/** Compare the parameterized error cases used by this probe. */
function sameError(actual:ContentJsonErrorKind, expected:ContentJsonErrorKind):Bool {
	return switch [actual, expected] {
		case [JsonExpectedValue, JsonExpectedValue]: true;
		case [JsonUnexpectedEnd, JsonUnexpectedEnd]: true;
		case [JsonUnexpectedCharacter, JsonUnexpectedCharacter]: true;
		case [JsonInvalidString, JsonInvalidString]: true;
		case [JsonInvalidNumber, JsonInvalidNumber]: true;
		case [JsonInvalidSeparator, JsonInvalidSeparator]: true;
		case [JsonInvalidEscape, JsonInvalidEscape]: true;
		case [JsonTrailingContent, JsonTrailingContent]: true;
		case [JsonDepthExceeded(actualMaximum), JsonDepthExceeded(expectedMaximum)]: actualMaximum == expectedMaximum;
		case [JsonFileTooLarge(actualMaximum), JsonFileTooLarge(expectedMaximum)]: actualMaximum == expectedMaximum;
		case [
			JsonCollectionLimitExceeded(actualMaximum),
			JsonCollectionLimitExceeded(expectedMaximum)
		]: actualMaximum == expectedMaximum;
		case [JsonNodeLimitExceeded(actualMaximum), JsonNodeLimitExceeded(expectedMaximum)]: actualMaximum == expectedMaximum;
		case _: false;
	};
}

/** Build one document with an exact array-nesting depth. */
function depthDocument(depth:Int):String {
	final output = new StringBuf();
	for (_ in 0...depth)
		output.add("[");
	output.add("0");
	for (_ in 0...depth)
		output.add("]");
	return output.toString();
}

/** Build one array with an exact number of scalar entries. */
function arrayDocument(entries:Int):String {
	final output = new StringBuf();
	output.add("[");
	for (index in 0...entries) {
		if (index > 0)
			output.add(",");
		output.add("0");
	}
	output.add("]");
	return output.toString();
}

/** Build an exact node count without approaching the per-collection bound. */
function nodeCountDocument(totalNodes:Int):String {
	final output = new StringBuf();
	output.add("[");
	final groupCount = 8;
	var remaining = totalNodes - 1 - groupCount;
	for (group in 0...groupCount) {
		if (group > 0)
			output.add(",");
		output.add("[");
		final groupMaximum = ContentJson.MAXIMUM_COLLECTION_ENTRIES - 1;
		final entries = remaining > groupMaximum ? groupMaximum : remaining;
		for (index in 0...entries) {
			if (index > 0)
				output.add(",");
			output.add("0");
		}
		output.add("]");
		remaining -= entries;
	}
	output.add("]");
	return output.toString();
}

/** Build valid whitespace plus one scalar at exactly the byte ceiling. */
function byteLimitDocument():Bytes {
	final bytes = Bytes.alloc(ContentJson.MAXIMUM_BYTES);
	for (index in 0...bytes.length)
		bytes.set(index, 0x20);
	bytes.set(bytes.length - 1, 0x30);
	return bytes;
}

/** Fold the closed tree into target-neutral evidence without serialization. */
function digestNode(node:ContentJsonNode):Int {
	var hash = CaxecraftTrace.mix(0x811C9DC5, node.line);
	hash = CaxecraftTrace.mix(hash, node.column);
	return switch node.value {
		case JsonNull: CaxecraftTrace.mix(hash, 1);
		case JsonBoolean(value): CaxecraftTrace.mix(hash, value ? 3 : 2);
		case JsonNumber(value): digestText(CaxecraftTrace.mix(hash, 4), value);
		case JsonString(value): digestText(CaxecraftTrace.mix(hash, 5), value);
		case JsonArray(values):
			var result = CaxecraftTrace.mix(hash, 6);
			for (value in values)
				result = CaxecraftTrace.mix(result, digestNode(value));
			result;
		case JsonObject(fields):
			var result = CaxecraftTrace.mix(hash, 7);
			for (field in fields)
				result = digestField(result, field);
			result;
	};
}

/** Fold one ordered field into the tree trace. */
function digestField(hash:Int, field:ContentJsonField):Int
	return CaxecraftTrace.mix(digestText(hash, field.name), digestNode(field.value));

/** Fold exact UTF-8 bytes into the target-neutral trace. */
function digestText(hash:Int, text:String):Int {
	var result = hash;
	final bytes = Bytes.ofString(text);
	for (index in 0...bytes.length)
		result = CaxecraftTrace.mix(result, bytes.get(index));
	return result;
}
