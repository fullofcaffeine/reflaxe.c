package hxc.bindgen;

import haxe.crypto.Sha256;
import hxc.bindgen.HxcBindgenModel.HxcBindgenPaths;
import hxc.config.HxcJsonCodec.jsonArray;
import hxc.config.HxcJsonCodec.jsonBool;
import hxc.config.HxcJsonCodec.jsonField;
import hxc.config.HxcJsonCodec.jsonInt;
import hxc.config.HxcJsonCodec.jsonObject;
import hxc.config.HxcJsonCodec.jsonString;
import hxc.config.HxcJsonCodec.objectField;
import hxc.config.HxcJsonValue;
import hxc.config.HxcJsonValue.HxcJsonNode;
import reflaxe.c.CUtf8Order.compare as compareUtf8;

/**
	Pair Clang's declared C aggregates with its target record-layout facts.

	The JSON AST owns names, fields, attributes, and source identity. Clang's
	machine-oriented record dump separately owns sizes, alignment, and bit
	offsets. Keeping those inputs separate makes an incomplete declaration stay
	opaque and makes a malformed or unsupported layout fail before a lock exists.
**/
/** Inputs for one aggregate-model capture. */
typedef HxcBindgenAggregateRequest = {
	final ast:HxcJsonNode;
	final layoutDump:String;
	final paths:HxcBindgenPaths;
}

private typedef HxcRecordLayout = {
	final type:String;
	final sizeBits:Int;
	final dataSizeBits:Null<Int>;
	final alignmentBits:Int;
	final fieldOffsets:Array<Int>;
}

private typedef HxcAggregateEntry = {
	final stableName:String;
	final complete:Bool;
	final model:HxcJsonNode;
}

/** Build the schema-1 aggregate ABI model or reject an unprovable layout. */
function buildAggregateAbiModel(request:HxcBindgenAggregateRequest):HxcJsonNode {
	final layouts = parseRecordLayouts(request.layoutDump);
	final aliases:Map<String, String> = [];
	collectRecordAliases(request.ast, aliases);
	final records:Map<String, HxcAggregateEntry> = [];
	collectAuthoredRecords(request.ast, null, aliases, layouts, request.paths, records);
	final ordered = [for (entry in records) entry];
	ordered.sort((left, right) -> compareUtf8(left.stableName, right.stableName));
	return jsonObject([
		jsonField("schemaVersion", jsonInt(1)),
		jsonField("records", jsonArray(ordered.map(entry -> entry.model)))
	]);
}

/** Parse Clang's `ASTRecordLayout` blocks without inferring any ABI values. */
function parseRecordLayouts(text:String):Map<String, HxcRecordLayout> {
	final result:Map<String, HxcRecordLayout> = [];
	final lines = text.split("\n");
	var index = 0;
	while (index < lines.length) {
		final line = StringTools.trim(lines[index]);
		if (!StringTools.startsWith(line, "Type: ")) {
			index++;
			continue;
		}
		final type = line.substr(6);
		var size:Null<Int> = null;
		var dataSize:Null<Int> = null;
		var alignment:Null<Int> = null;
		var offsets:Null<Array<Int>> = null;
		index++;
		while (index < lines.length && !StringTools.startsWith(StringTools.trim(lines[index]), "Type: ")) {
			final field = StringTools.trim(lines[index]);
			if (StringTools.startsWith(field, "Size:"))
				size = parseUnsigned(StringTools.trim(field.substr(5)));
			else if (StringTools.startsWith(field, "DataSize:"))
				dataSize = parseUnsigned(StringTools.trim(field.substr(9)));
			else if (StringTools.startsWith(field, "Alignment:"))
				alignment = parseUnsigned(StringTools.trim(field.substr(10)));
			else if (StringTools.startsWith(field, "FieldOffsets:"))
				offsets = parseOffsets(StringTools.trim(field.substr(13)));
			index++;
		}
		if (type == "" || size == null || alignment == null || offsets == null)
			throw aggregateFailure("Clang emitted an incomplete record-layout block",
				"Use a supported Clang record-layout format or report the selected Clang version and output.");
		result.set(type, {
			type: type,
			sizeBits: size,
			dataSizeBits: dataSize,
			alignmentBits: alignment,
			fieldOffsets: offsets
		});
	}
	return result;
}

function collectRecordAliases(node:HxcJsonNode, aliases:Map<String, String>):Void {
	if (stringField(node, "kind") == "TypedefDecl") {
		final recordId = referencedRecordId(node);
		final name = stringField(node, "name");
		if (recordId != null && name != "")
			aliases.set(recordId, name);
	}
	for (child in childNodes(node))
		collectRecordAliases(child, aliases);
}

function referencedRecordId(node:HxcJsonNode):Null<String> {
	for (fieldName in ["ownedTagDecl", "decl"]) {
		final declaration = objectField(node, fieldName);
		if (declaration != null && stringField(declaration, "kind") == "RecordDecl") {
			final id = stringField(declaration, "id");
			if (id != "")
				return id;
		}
	}
	for (child in childNodes(node)) {
		final id = referencedRecordId(child);
		if (id != null)
			return id;
	}
	return null;
}

function collectAuthoredRecords(node:HxcJsonNode, inheritedFile:Null<String>, aliases:Map<String, String>, layouts:Map<String, HxcRecordLayout>,
		paths:HxcBindgenPaths, records:Map<String, HxcAggregateEntry>):Void {
	var currentFile = inheritedFile;
	for (child in childNodes(node)) {
		final explicitFile = sourceFile(child);
		if (explicitFile != null)
			currentFile = explicitFile;
		if (stringField(child, "kind") == "RecordDecl" && currentFile != null && isAuthoredPath(paths.logical(currentFile))) {
			final entry = recordModel(child, currentFile, aliases, layouts, paths);
			final previous = records.get(entry.stableName);
			if (previous == null || !previous.complete && entry.complete)
				records.set(entry.stableName, entry);
		}
		collectAuthoredRecords(child, currentFile, aliases, layouts, paths, records);
	}
}

function recordModel(declaration:HxcJsonNode, sourceFile:String, aliases:Map<String, String>, layouts:Map<String, HxcRecordLayout>,
		paths:HxcBindgenPaths):HxcAggregateEntry {
	final kind = stringField(declaration, "tagUsed");
	if (kind != "struct" && kind != "union")
		throw aggregateFailure("Clang record declaration has no supported struct or union kind",
			"Report the declaration so its native aggregate kind can be modeled explicitly.");
	final id = stringField(declaration, "id");
	final nativeTag = stringField(declaration, "name");
	final nativeTypedef = id == "" ? null : aliases.get(id);
	final location = objectField(declaration, "loc");
	final line = location == null ? 0 : intField(location, "line");
	final column = location == null ? 0 : intField(location, "col");
	final logicalFile = paths.logical(sourceFile);
	final stableName = nativeTag != "" ? nativeTag : nativeTypedef != null ? nativeTypedef : anonymousName(logicalFile, line, column);
	final complete = boolField(declaration, "completeDefinition");
	if (!complete) {
		return {
			stableName: stableName,
			complete: false,
			model: jsonObject([
				jsonField("stableName", jsonString(stableName)),
				jsonField("nativeTag", nullableString(nativeTag == "" ? null : nativeTag)),
				jsonField("nativeTypedef", nullableString(nativeTypedef)),
				jsonField("source", sourceLocation(logicalFile, line, column)),
				jsonField("kind", jsonString(kind)),
				jsonField("complete", jsonBool(false)),
				jsonField("opaque", jsonBool(true)),
				jsonField("layout", new HxcJsonNode(HxcJsonValue.JNull, 0, 0)),
				jsonField("fields", jsonArray([]))
			])
		};
	}
	for (child in childNodes(declaration)) {
		final childKind = stringField(child, "kind");
		if (StringTools.endsWith(childKind, "Attr") && childKind != "PackedAttr" && childKind != "AlignedAttr")
			throw aggregateFailure('aggregate `$stableName` uses unsupported nonportable layout attribute `$childKind`',
				"Remove the target-specific attribute or add an exact reviewed Clang-derived layout contract.");
		if ((childKind == "PackedAttr" || childKind == "AlignedAttr") && attributeFollowsFields(child, declaration))
			throw aggregateFailure('aggregate `$stableName` places a layout attribute after its field list',
				"Place packed/aligned attributes on the record declaration before `{`; Clang's record dump otherwise describes a different underlying type.");
	}
	final layoutKey = nativeTag != "" ? '$kind $nativeTag' : '$kind (unnamed at $sourceFile:$line:$column)';
	final layout = layouts.get(layoutKey);
	if (layout == null)
		throw aggregateFailure('Clang omitted the complete layout for aggregate `$stableName`',
			"Report the selected Clang version and its machine-oriented record-layout output.");
	final fields = [
		for (child in childNodes(declaration))
			if (stringField(child, "kind") == "FieldDecl") child
	];
	if (fields.length != layout.fieldOffsets.length)
		throw aggregateFailure('Clang layout for aggregate `$stableName` has ${layout.fieldOffsets.length} offsets for ${fields.length} fields',
			"Report the AST and record-layout output; field order cannot be paired safely.");
	final fieldModels:Array<HxcJsonNode> = [];
	for (ordinal in 0...fields.length)
		fieldModels.push(fieldModel(fields[ordinal], ordinal, layout.fieldOffsets[ordinal], paths));
	final packed = hasChildKind(declaration, "PackedAttr");
	final requestedAlignment = alignedBytes(declaration);
	return {
		stableName: stableName,
		complete: true,
		model: jsonObject([
			jsonField("stableName", jsonString(stableName)),
			jsonField("nativeTag", nullableString(nativeTag == "" ? null : nativeTag)),
			jsonField("nativeTypedef", nullableString(nativeTypedef)),
			jsonField("source", sourceLocation(logicalFile, line, column)),
			jsonField("kind", jsonString(kind)),
			jsonField("complete", jsonBool(true)),
			jsonField("opaque", jsonBool(false)),
			jsonField("layout", jsonObject([
				jsonField("sizeBits", jsonInt(layout.sizeBits)),
				jsonField("dataSizeBits", layout.dataSizeBits == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonInt(layout.dataSizeBits)),
				jsonField("alignmentBits", jsonInt(layout.alignmentBits)),
				jsonField("packed", jsonBool(packed)),
				jsonField("requestedAlignmentBits", requestedAlignment == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonInt(requestedAlignment * 8))
			])),
			jsonField("fields", jsonArray(fieldModels))
		])
	};
}

function fieldModel(field:HxcJsonNode, ordinal:Int, bitOffset:Int, paths:HxcBindgenPaths):HxcJsonNode {
	final nativeName = stringField(field, "name");
	final bitfield = boolField(field, "isBitfield");
	final bitWidth = bitfield ? constantExpression(field) : null;
	if (bitfield && bitWidth == null)
		throw aggregateFailure("Clang omitted an aggregate bitfield width", "Report the field AST so its evaluated width can be modeled.");
	final typeNode = objectField(field, "type");
	final spelling = typeNode == null ? "" : stringField(typeNode, "qualType");
	final canonical = typeNode == null ? "" : stringField(typeNode, "desugaredQualType");
	if (spelling == "")
		throw aggregateFailure("Clang omitted an aggregate field type", "Report the field AST so its declared type can be modeled.");
	return jsonObject([
		jsonField("nativeName", nullableString(nativeName == "" ? null : nativeName)),
		jsonField("ordinal", jsonInt(ordinal)),
		jsonField("bitOffset", jsonInt(bitOffset)),
		jsonField("bitWidth", bitWidth == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonInt(bitWidth)),
		jsonField("zeroWidthBitfield", jsonBool(bitWidth == 0)),
		jsonField("anonymous", jsonBool(boolField(field, "isImplicit") || nativeName == "" && !bitfield)),
		jsonField("flexibleArray", jsonBool(StringTools.endsWith(spelling, "[]"))),
		jsonField("type", jsonObject([
			jsonField("kind", jsonString("declared")),
			jsonField("spelling", jsonString(paths.logicalText(spelling))),
			jsonField("canonicalSpelling", jsonString(paths.logicalText(canonical == "" ? spelling : canonical)))
		]))
	]);
}

function parseOffsets(value:String):Null<Array<Int>> {
	if (StringTools.endsWith(value, ">"))
		value = StringTools.trim(value.substr(0, value.length - 1));
	if (value.length < 2 || value.charAt(0) != "[" || value.charAt(value.length - 1) != "]")
		return null;
	final body = StringTools.trim(value.substring(1, value.length - 1));
	if (body == "")
		return [];
	final result:Array<Int> = [];
	for (part in body.split(",")) {
		final parsed = parseUnsigned(StringTools.trim(part));
		if (parsed == null)
			return null;
		result.push(parsed);
	}
	return result;
}

function parseUnsigned(value:String):Null<Int> {
	if (value == "")
		return null;
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (code < 0x30 || code > 0x39)
			return null;
	}
	return Std.parseInt(value);
}

function alignedBytes(record:HxcJsonNode):Null<Int> {
	for (child in childNodes(record))
		if (stringField(child, "kind") == "AlignedAttr")
			return constantExpression(child);
	return null;
}

function constantExpression(node:HxcJsonNode):Null<Int> {
	final direct = stringField(node, "value");
	final parsed = direct == "" ? null : Std.parseInt(direct);
	if (parsed != null)
		return parsed;
	for (child in childNodes(node)) {
		final value = constantExpression(child);
		if (value != null)
			return value;
	}
	return null;
}

function hasChildKind(node:HxcJsonNode, kind:String):Bool {
	for (child in childNodes(node))
		if (stringField(child, "kind") == kind)
			return true;
	return false;
}

function attributeFollowsFields(attribute:HxcJsonNode, record:HxcJsonNode):Bool {
	final attributeOffset = rangeOffset(attribute, "begin");
	var lastFieldOffset = 0;
	for (child in childNodes(record))
		if (stringField(child, "kind") == "FieldDecl") {
			final offset = rangeOffset(child, "end");
			if (offset > lastFieldOffset)
				lastFieldOffset = offset;
		}
	return attributeOffset > 0 && lastFieldOffset > 0 && attributeOffset > lastFieldOffset;
}

function rangeOffset(node:HxcJsonNode, edge:String):Int {
	final range = objectField(node, "range");
	final position = range == null ? null : objectField(range, edge);
	return position == null ? 0 : intField(position, "offset");
}

function childNodes(node:HxcJsonNode):Array<HxcJsonNode> {
	final inner = objectField(node, "inner");
	return switch inner == null ? HxcJsonValue.JNull : inner.value {
		case JArray(values): values;
		case _: [];
	};
}

function sourceFile(node:HxcJsonNode):Null<String> {
	final location = objectField(node, "loc");
	if (location == null)
		return null;
	final file = stringField(location, "file");
	return file == "" ? null : file;
}

function sourceLocation(file:String, line:Int, column:Int):HxcJsonNode
	return jsonObject([
		jsonField("file", jsonString(file)),
		jsonField("line", jsonInt(line)),
		jsonField("column", jsonInt(column))
	]);

function stringField(node:HxcJsonNode, name:String):String {
	final value = objectField(node, name);
	return switch value == null ? HxcJsonValue.JNull : value.value {
		case JString(text): text;
		case _: "";
	};
}

function intField(node:HxcJsonNode, name:String):Int {
	final value = objectField(node, name);
	return switch value == null ? HxcJsonValue.JNull : value.value {
		case JNumber(text):
			final parsed = Std.parseInt(text);
			parsed == null ? 0 : parsed;
		case _: 0;
	};
}

function boolField(node:HxcJsonNode, name:String):Bool {
	final value = objectField(node, name);
	return switch value == null ? HxcJsonValue.JNull : value.value {
		case JBool(enabled): enabled;
		case _: false;
	};
}

function nullableString(value:Null<String>):HxcJsonNode
	return value == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonString(value);

function anonymousName(file:String, line:Int, column:Int):String
	return "anonymous-record-" + Sha256.encode('$file:$line:$column').substr(0, 12);

function isAuthoredPath(path:String):Bool
	return StringTools.startsWith(path, "$SOURCE") || StringTools.startsWith(path, "$INCLUDE") || StringTools.startsWith(path, "$SYSROOT");

function aggregateFailure(message:String, remediation:String):HxcBindgenError
	return new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0811", message, remediation);
