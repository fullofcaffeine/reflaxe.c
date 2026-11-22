package hxc.bindgen;

import haxe.crypto.Sha256;
import hxc.bindgen.HxcBindgenModel.HxcBindgenPaths;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenLanguage;
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
	Turn Clang declarations and generated semantic probes into one primitive ABI model.

	This module owns target-measured scalar, typedef, qualifier, enum, and scalar
	macro facts. It does not emit Haxe files: later bindgen stages consume this
	model when they can preserve the same native identities without guessing.
**/
/** One target-measured scalar description used to map Clang builtin types. */
private typedef HxcBindgenScalar = {
	final id:String;
	final cSpelling:String;
	final category:String;
	final bitWidth:Int;
	final signed:Null<Bool>;
	final haxeType:String;
}

/** Named inputs prevent the independent Clang probe trees from being misordered. */
typedef HxcBindgenAbiRequest = {
	final ast:HxcJsonNode;
	final probeAst:HxcJsonNode;
	final paths:HxcBindgenPaths;
	final macroNames:Array<String>;
	final macroTypeAst:HxcJsonNode;
	final macroValueAst:HxcJsonNode;
	final language:HxcBindgenLanguage;
}

/** One source-nameable enum selected for an exact Clang storage probe. */
private typedef HxcBindgenEnumReference = {
	final id:String;
	final spelling:String;
	final alias:Null<String>;
}

/**
	Build the Clang source that measures primitive target facts.

	The source contains declarations only for Clang to evaluate. It is supplied
	over standard input after the configured headers, so it cannot become a
	handwritten native product path or a second source of ABI truth.
**/
function primitiveProbeSource(language:HxcBindgenLanguage):String {
	final boolType = language == HxcBindgenLanguage.Cxx ? "bool" : "_Bool";
	return 'enum HxcBindgenPrimitiveProbe {\n'
		+ '  HXC_BINDGEN_CHAR_BITS = __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_BOOL_BITS = sizeof($boolType) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_CHAR_WIDTH = sizeof(char) * __CHAR_BIT__,\n'
		+ '#ifdef __CHAR_UNSIGNED__\n'
		+ '  HXC_BINDGEN_CHAR_SIGNED = 0,\n'
		+ '#else\n'
		+ '  HXC_BINDGEN_CHAR_SIGNED = 1,\n'
		+ '#endif\n'
		+ '  HXC_BINDGEN_SCHAR_WIDTH = sizeof(signed char) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_UCHAR_WIDTH = sizeof(unsigned char) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_SHORT_WIDTH = sizeof(short) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_USHORT_WIDTH = sizeof(unsigned short) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_INT_WIDTH = sizeof(int) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_UINT_WIDTH = sizeof(unsigned int) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_LONG_WIDTH = sizeof(long) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_ULONG_WIDTH = sizeof(unsigned long) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_LLONG_WIDTH = sizeof(long long) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_ULLONG_WIDTH = sizeof(unsigned long long) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_FLOAT_WIDTH = sizeof(float) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_FLOAT_MANT_DIG = __FLT_MANT_DIG__,\n'
		+ '  HXC_BINDGEN_FLOAT_MAX_EXP = __FLT_MAX_EXP__,\n'
		+ '  HXC_BINDGEN_DOUBLE_WIDTH = sizeof(double) * __CHAR_BIT__,\n'
		+ '  HXC_BINDGEN_DOUBLE_MANT_DIG = __DBL_MANT_DIG__,\n'
		+ '  HXC_BINDGEN_DOUBLE_MAX_EXP = __DBL_MAX_EXP__\n'
		+ '};\n';
}

/**
	Derive the primitive ABI view from Clang's declaration and probe ASTs.

	Widths and signedness come only from evaluated probe constants. C spellings
	select a measured scalar entry but never choose its width. Typedefs retain a
	structural type tree, so qualifiers stay attached to the pointee or pointer
	level where Clang placed them.
**/
function buildPrimitiveAbiModel(request:HxcBindgenAbiRequest):HxcJsonNode {
	final ast = request.ast;
	final paths = request.paths;
	final probeValues:Map<String, Int> = [];
	collectProbeValues(request.probeAst, probeValues);
	if (requiredProbe(probeValues, "HXC_BINDGEN_CHAR_BITS") != 8)
		throw abiFailure("target C bytes are not eight bits", "Use a supported target whose CHAR_BIT is 8.");
	final scalars = buildScalars(probeValues, request.language);
	final scalarBySpelling:Map<String, HxcBindgenScalar> = [];
	for (scalar in scalars)
		scalarBySpelling.set(scalar.cSpelling, scalar);

	final typedefs:Array<HxcJsonNode> = [];
	final enums:Array<HxcJsonNode> = [];
	final enumReferences = enumReferences(ast, paths);
	final enumFacts:Map<String, {bitWidth:Int, signed:Bool, alias:Null<String>}> = [];
	final enumProbeValues:Map<String, Int> = [];
	collectProbeValues(request.probeAst, enumProbeValues);
	for (index in 0...enumReferences.length) {
		final width = requiredProbe(enumProbeValues, 'HXC_BINDGEN_ENUM_WIDTH_$index');
		final signed = requiredProbe(enumProbeValues, 'HXC_BINDGEN_ENUM_SIGNED_$index') == 1;
		enumFacts.set(enumReferences[index].id, {bitWidth: width, signed: signed, alias: enumReferences[index].alias});
	}
	var currentFile:Null<String> = null;
	for (declaration in childNodes(ast)) {
		final explicitFile = sourceFile(declaration);
		if (explicitFile != null)
			currentFile = paths.logical(explicitFile);
		if (currentFile == null || !isAuthoredPath(currentFile))
			continue;
		switch stringField(declaration, "kind") {
			case "TypedefDecl":
				final representation = typedefRepresentation(declaration, scalarBySpelling);
				if (representation != null)
					typedefs.push(jsonObject([
						jsonField("nativeName", jsonString(requireName(declaration, "typedef"))),
						jsonField("source", sourceLocation(declaration, currentFile)),
						jsonField("representation", representation)
					]));
			case "EnumDecl":
				final model = enumModel(declaration, currentFile, scalarBySpelling, enumFacts);
				if (model != null)
					enums.push(model);
			case _:
		}
	}
	typedefs.sort(compareNativeName);
	enums.sort(compareStableName);
	final macroConstants = buildMacroConstants(request.macroNames, request.macroTypeAst, request.macroValueAst, scalarBySpelling);
	return jsonObject([
		jsonField("schemaVersion", jsonInt(1)),
		jsonField("scalars", jsonArray(scalars.map(scalarJson))),
		jsonField("typedefs", jsonArray(typedefs)),
		jsonField("enums", jsonArray(enums)),
		jsonField("macroConstants", jsonArray(macroConstants))
	]);
}

/** Return header-introduced object-like scalar macro names from two Clang inventories. */
function discoverScalarMacros(baseline:String, configured:String):Array<String> {
	final baselineDefinitions = macroDefinitions(baseline);
	final configuredDefinitions = macroDefinitions(configured);
	final names:Array<String> = [];
	for (name in configuredDefinitions.keys()) {
		final replacement = configuredDefinitions.get(name);
		if (replacement != null && replacement != baselineDefinitions.get(name) && isScalarMacroReplacement(replacement))
			names.push(name);
	}
	names.sort(compareUtf8);
	return names;
}

/** Ask Clang to resolve each discovered macro's exact expression type. */
function macroTypeProbeSource(names:Array<String>):String {
	final output = new StringBuf();
	for (index in 0...names.length)
		output.add('static const __typeof__(${names[index]}) HXC_BINDGEN_MACRO_TYPE_$index = (${names[index]});\n');
	return output.toString();
}

/** Ask Clang to evaluate the macros whose resolved types are integral. */
function macroValueProbeSource(names:Array<String>, typeAst:HxcJsonNode):String {
	final integral = integralMacroIndices(names, typeAst);
	if (integral.length == 0)
		return "enum HxcBindgenEmptyMacroProbe { HXC_BINDGEN_NO_INTEGER_MACROS = 0 };\n";
	final output = new StringBuf();
	output.add("enum HxcBindgenMacroValueProbe {\n");
	for (position in 0...integral.length) {
		final index = integral[position];
		output.add('  HXC_BINDGEN_MACRO_VALUE_$index = (${names[index]})');
		output.add(position + 1 == integral.length ? "\n" : ",\n");
	}
	output.add("};\n");
	return output.toString();
}

/** Ask Clang to measure every source-nameable authored enum representation. */
function enumProbeSource(ast:HxcJsonNode, paths:HxcBindgenPaths):String {
	final references = enumReferences(ast, paths);
	if (references.length == 0)
		return "enum HxcBindgenEmptyEnumProbe { HXC_BINDGEN_NO_ENUMS = 0 };\n";
	final output = new StringBuf();
	output.add("enum HxcBindgenEnumStorageProbe {\n");
	for (index in 0...references.length) {
		final spelling = references[index].spelling;
		output.add('  HXC_BINDGEN_ENUM_WIDTH_$index = sizeof($spelling) * __CHAR_BIT__,\n');
		output.add('  HXC_BINDGEN_ENUM_SIGNED_$index = (($spelling)-1 < ($spelling)0)');
		output.add(index + 1 == references.length ? "\n" : ",\n");
	}
	output.add("};\n");
	return output.toString();
}

function buildScalars(values:Map<String, Int>, language:HxcBindgenLanguage):Array<HxcBindgenScalar> {
	final result:Array<HxcBindgenScalar> = [
		integerScalar("signed-char", "signed char", requiredProbe(values, "HXC_BINDGEN_SCHAR_WIDTH"), true),
		integerScalar("unsigned-char", "unsigned char", requiredProbe(values, "HXC_BINDGEN_UCHAR_WIDTH"), false),
		integerScalar("char", "char", requiredProbe(values, "HXC_BINDGEN_CHAR_WIDTH"), requiredProbe(values, "HXC_BINDGEN_CHAR_SIGNED") == 1),
		integerScalar("signed-short", "short", requiredProbe(values, "HXC_BINDGEN_SHORT_WIDTH"), true),
		integerScalar("unsigned-short", "unsigned short", requiredProbe(values, "HXC_BINDGEN_USHORT_WIDTH"), false),
		integerScalar("signed-int", "int", requiredProbe(values, "HXC_BINDGEN_INT_WIDTH"), true),
		integerScalar("unsigned-int", "unsigned int", requiredProbe(values, "HXC_BINDGEN_UINT_WIDTH"), false),
		integerScalar("signed-long", "long", requiredProbe(values, "HXC_BINDGEN_LONG_WIDTH"), true),
		integerScalar("unsigned-long", "unsigned long", requiredProbe(values, "HXC_BINDGEN_ULONG_WIDTH"), false),
		integerScalar("signed-long-long", "long long", requiredProbe(values, "HXC_BINDGEN_LLONG_WIDTH"), true),
		integerScalar("unsigned-long-long", "unsigned long long", requiredProbe(values, "HXC_BINDGEN_ULLONG_WIDTH"), false)
	];
	result.push({
		id: "bool",
		cSpelling: language == HxcBindgenLanguage.Cxx ? "bool" : "_Bool",
		category: "boolean",
		bitWidth: requiredProbe(values, "HXC_BINDGEN_BOOL_BITS"),
		signed: null,
		haxeType: "Bool"
	});
	final floatWidth = requiredProbe(values, "HXC_BINDGEN_FLOAT_WIDTH");
	if (floatWidth != 32
		|| requiredProbe(values, "HXC_BINDGEN_FLOAT_MANT_DIG") != 24
		|| requiredProbe(values, "HXC_BINDGEN_FLOAT_MAX_EXP") != 128)
		throw abiFailure("target `float` is not IEC 60559 binary32", "Use a supported target or add a reviewed exact floating carrier.");
	result.push({
		id: "binary32",
		cSpelling: "float",
		category: "floating",
		bitWidth: floatWidth,
		signed: null,
		haxeType: "c.Float32"
	});
	final doubleWidth = requiredProbe(values, "HXC_BINDGEN_DOUBLE_WIDTH");
	if (doubleWidth != 64
		|| requiredProbe(values, "HXC_BINDGEN_DOUBLE_MANT_DIG") != 53
		|| requiredProbe(values, "HXC_BINDGEN_DOUBLE_MAX_EXP") != 1024)
		throw abiFailure("target `double` is not IEC 60559 binary64", "Use a supported target or add a reviewed exact floating carrier.");
	result.push({
		id: "binary64",
		cSpelling: "double",
		category: "floating",
		bitWidth: doubleWidth,
		signed: null,
		haxeType: "Float"
	});
	result.sort((left, right) -> compareUtf8(left.id, right.id));
	return result;
}

function integerScalar(id:String, spelling:String, bitWidth:Int, signed:Bool):HxcBindgenScalar {
	final haxeType = integerHaxeType(bitWidth, signed, spelling);
	return {
		id: id,
		cSpelling: spelling,
		category: "integer",
		bitWidth: bitWidth,
		signed: signed,
		haxeType: haxeType
	};
}

function integerHaxeType(bitWidth:Int, signed:Bool, spelling:String = "enum"):String {
	return switch bitWidth {
		case 8: signed ? "c.Int8" : "c.UInt8";
		case 16: signed ? "c.Int16" : "c.UInt16";
		case 32: signed ? "c.Int32" : "c.UInt32";
		case 64: signed ? "c.Int64" : "c.UInt64";
		case _: throw abiFailure('target scalar `$spelling` has unsupported width $bitWidth',
				"Use an 8/16/32/64-bit integer target or add a reviewed exact carrier.");
	};
}

function scalarJson(scalar:HxcBindgenScalar):HxcJsonNode {
	return jsonObject([
		jsonField("id", jsonString(scalar.id)),
		jsonField("cSpelling", jsonString(scalar.cSpelling)),
		jsonField("category", jsonString(scalar.category)),
		jsonField("bitWidth", jsonInt(scalar.bitWidth)),
		jsonField("signed", scalar.signed == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonBool(scalar.signed)),
		jsonField("haxeType", jsonString(scalar.haxeType))
	]);
}

function typedefRepresentation(declaration:HxcJsonNode, scalars:Map<String, HxcBindgenScalar>):Null<HxcJsonNode> {
	final children = childNodes(declaration);
	if (children.length != 1)
		return null;
	return typeRepresentation(children[0], scalars);
}

function typeRepresentation(node:HxcJsonNode, scalars:Map<String, HxcBindgenScalar>):Null<HxcJsonNode> {
	final children = childNodes(node);
	return switch stringField(node, "kind") {
		case "BuiltinType":
			final spelling = typeSpelling(node);
			final scalar = scalars.get(spelling);
			if (scalar == null) null else jsonObject([
				jsonField("kind", jsonString("scalar")),
				jsonField("scalar", jsonString(scalar.id)),
				jsonField("haxeType", jsonString(scalar.haxeType))
			]);
		case "QualType":
			if (children.length != 1) null else {
				final inner = typeRepresentation(children[0], scalars);
				if (inner == null)
					null
				else
					jsonObject([
						jsonField("kind", jsonString("qualified")),
						jsonField("qualifiers", jsonArray(qualifierNames(stringField(node, "qualifiers")).map(jsonString))),
						jsonField("inner", inner)
					]);
			}
		case "PointerType":
			if (children.length != 1) null else {
				final pointee = typeRepresentation(children[0], scalars);
				pointee == null ? null : jsonObject([jsonField("kind", jsonString("pointer")), jsonField("pointee", pointee)]);
			}
		case "TypedefType":
			final declarationNode = objectField(node, "decl");
			final name = declarationNode == null ? "" : stringField(declarationNode, "name");
			name == "" ? null : jsonObject([
				jsonField("kind", jsonString("typedef")),
				jsonField("nativeName", jsonString(name))
			]);
		case "ElaboratedType":
			if (children.length != 1) null else typeRepresentation(children[0], scalars);
		case "EnumType":
			final declarationNode = objectField(node, "decl");
			final name = declarationNode == null ? "" : stringField(declarationNode, "name");
			name == "" ? null : jsonObject([jsonField("kind", jsonString("enum")), jsonField("nativeName", jsonString(name))]);
		case _: null;
	};
}

function enumModel(declaration:HxcJsonNode, file:String, scalars:Map<String, HxcBindgenScalar>,
		enumFacts:Map<String, {bitWidth:Int, signed:Bool, alias:Null<String>}>):Null<HxcJsonNode> {
	final constants:Array<HxcJsonNode> = [];
	var carrier:Null<HxcBindgenScalar> = null;
	for (child in childNodes(declaration)) {
		if (stringField(child, "kind") != "EnumConstantDecl")
			continue;
		final childCarrier = scalars.get(typeSpelling(child));
		if (childCarrier == null)
			return null;
		if (carrier == null)
			carrier = childCarrier;
		final value = constantValue(child);
		if (value == null)
			return null;
		constants.push(jsonObject([
			jsonField("nativeName", jsonString(requireName(child, "enum constant"))),
			jsonField("value", jsonString(value))
		]));
	}
	if (constants.length == 0 || carrier == null)
		return null;
	constants.sort(compareNativeName);
	final nativeName = stringField(declaration, "name");
	final location = sourceLocation(declaration, file);
	final facts = enumFacts.get(stringField(declaration, "id"));
	final stableName = nativeName != "" ? nativeName : facts != null && facts.alias != null ? facts.alias : anonymousName(file, location);
	final storageType = facts == null ? carrier.haxeType : integerHaxeType(facts.bitWidth, facts.signed);
	return jsonObject([
		jsonField("stableName", jsonString(stableName)),
		jsonField("nativeTag", nativeName == "" ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonString(nativeName)),
		jsonField("nativeTypedef", facts == null
			|| facts.alias == null ? new HxcJsonNode(HxcJsonValue.JNull, 0, 0) : jsonString(facts.alias)),
		jsonField("source", location),
		jsonField("valueCarrier", jsonString(carrier.id)),
		jsonField("storageBitWidth", jsonInt(facts == null ? carrier.bitWidth : facts.bitWidth)),
		jsonField("storageSigned", jsonBool(facts == null ? carrier.signed == true : facts.signed)),
		jsonField("haxeType", jsonString(storageType)),
		jsonField("constants", jsonArray(constants))
	]);
}

function buildMacroConstants(names:Array<String>, typeAst:HxcJsonNode, valueAst:HxcJsonNode, scalars:Map<String, HxcBindgenScalar>):Array<HxcJsonNode> {
	final types:Map<Int, String> = [];
	collectMacroTypes(typeAst, types);
	final values:Map<Int, String> = [];
	collectMacroValues(valueAst, values);
	final result:Array<HxcJsonNode> = [];
	for (index in 0...names.length) {
		final spelling = types.get(index);
		if (spelling == null)
			continue;
		final scalar = scalars.get(withoutLeadingConst(spelling));
		final value = values.get(index);
		if (scalar == null || scalar.category != "integer" || value == null)
			continue;
		result.push(jsonObject([
			jsonField("nativeName", jsonString(names[index])),
			jsonField("stableName", jsonString(names[index])),
			jsonField("category", jsonString("integer")),
			jsonField("scalar", jsonString(scalar.id)),
			jsonField("haxeType", jsonString(scalar.haxeType)),
			jsonField("value", jsonString(value)),
			jsonField("provenance", jsonString("configured-header-closure"))
		]));
	}
	result.sort(compareNativeName);
	return result;
}

function integralMacroIndices(names:Array<String>, ast:HxcJsonNode):Array<Int> {
	final types:Map<Int, String> = [];
	collectMacroTypes(ast, types);
	final result:Array<Int> = [];
	for (index in 0...names.length) {
		final spelling = types.get(index);
		if (spelling != null && isIntegralSpelling(withoutLeadingConst(spelling)))
			result.push(index);
	}
	return result;
}

function collectMacroTypes(node:HxcJsonNode, values:Map<Int, String>):Void {
	if (stringField(node, "kind") == "VarDecl") {
		final index = numberedSuffix(stringField(node, "name"), "HXC_BINDGEN_MACRO_TYPE_");
		if (index != null)
			values.set(index, typeSpelling(node));
	}
	for (child in childNodes(node))
		collectMacroTypes(child, values);
}

function collectMacroValues(node:HxcJsonNode, values:Map<Int, String>):Void {
	if (stringField(node, "kind") == "EnumConstantDecl") {
		final index = numberedSuffix(stringField(node, "name"), "HXC_BINDGEN_MACRO_VALUE_");
		final value = index == null ? null : constantValue(node);
		if (index != null && value != null)
			values.set(index, value);
	}
	for (child in childNodes(node))
		collectMacroValues(child, values);
}

function enumReferences(ast:HxcJsonNode, paths:HxcBindgenPaths):Array<HxcBindgenEnumReference> {
	final aliases:Map<String, String> = [];
	for (declaration in childNodes(ast)) {
		if (stringField(declaration, "kind") != "TypedefDecl")
			continue;
		final enumId = referencedEnumId(declaration);
		final name = stringField(declaration, "name");
		if (enumId != null && name != "")
			aliases.set(enumId, name);
	}
	final result:Array<HxcBindgenEnumReference> = [];
	var currentFile:Null<String> = null;
	for (declaration in childNodes(ast)) {
		final explicitFile = sourceFile(declaration);
		if (explicitFile != null)
			currentFile = paths.logical(explicitFile);
		if (currentFile == null || !isAuthoredPath(currentFile) || stringField(declaration, "kind") != "EnumDecl")
			continue;
		final id = stringField(declaration, "id");
		final nativeName = stringField(declaration, "name");
		final alias = aliases.get(id);
		if (id == "" || nativeName == "" && alias == null)
			continue;
		result.push({id: id, spelling: alias == null ? 'enum $nativeName' : alias, alias: alias});
	}
	return result;
}

function referencedEnumId(node:HxcJsonNode):Null<String> {
	for (fieldName in ["ownedTagDecl", "decl"]) {
		final declaration = objectField(node, fieldName);
		if (declaration != null && stringField(declaration, "kind") == "EnumDecl") {
			final id = stringField(declaration, "id");
			if (id != "")
				return id;
		}
	}
	for (child in childNodes(node)) {
		final id = referencedEnumId(child);
		if (id != null)
			return id;
	}
	return null;
}

function macroDefinitions(text:String):Map<String, String> {
	final result:Map<String, String> = [];
	for (line in text.split("\n")) {
		if (!StringTools.startsWith(line, "#define "))
			continue;
		var index = 8;
		if (index >= line.length || !identifierStart(line.charCodeAt(index)))
			continue;
		final start = index;
		index++;
		while (index < line.length && identifierPart(line.charCodeAt(index)))
			index++;
		if (index < line.length && line.charCodeAt(index) == 0x28)
			continue;
		final name = line.substring(start, index);
		final replacement = StringTools.trim(line.substr(index));
		result.set(name, replacement);
	}
	return result;
}

function isScalarMacroReplacement(value:String):Bool {
	if (value == "")
		return false;
	var hasDigit = false;
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (code >= 0x30 && code <= 0x39)
			hasDigit = true;
		if (!(identifierPart(code) || code == 0x20 || code == 0x09 || "+-*/%<>&|^~!().?".indexOf(String.fromCharCode(code)) >= 0))
			return false;
	}
	return hasDigit;
}

function isIntegralSpelling(spelling:String):Bool {
	return spelling == "char" || spelling == "signed char" || spelling == "unsigned char" || spelling == "short" || spelling == "unsigned short"
		|| spelling == "int" || spelling == "unsigned int" || spelling == "long" || spelling == "unsigned long" || spelling == "long long"
		|| spelling == "unsigned long long";
}

function withoutLeadingConst(spelling:String):String
	return StringTools.startsWith(spelling, "const ") ? spelling.substr(6) : spelling;

function numberedSuffix(value:String, prefix:String):Null<Int> {
	if (!StringTools.startsWith(value, prefix))
		return null;
	final suffix = value.substr(prefix.length);
	if (suffix == "")
		return null;
	for (index in 0...suffix.length) {
		final code = suffix.charCodeAt(index);
		if (code < 0x30 || code > 0x39)
			return null;
	}
	return Std.parseInt(suffix);
}

function identifierStart(code:Null<Int>):Bool
	return code != null && (code == 0x5F || code >= 0x41 && code <= 0x5A || code >= 0x61 && code <= 0x7A);

function identifierPart(code:Null<Int>):Bool
	return identifierStart(code) || code != null && code >= 0x30 && code <= 0x39;

function collectProbeValues(node:HxcJsonNode, values:Map<String, Int>):Void {
	if (stringField(node, "kind") == "EnumConstantDecl") {
		final name = stringField(node, "name");
		if (StringTools.startsWith(name, "HXC_BINDGEN_")) {
			final value = constantValue(node);
			final parsed = value == null ? null : Std.parseInt(value);
			if (parsed != null)
				values.set(name, parsed);
		}
	}
	for (child in childNodes(node))
		collectProbeValues(child, values);
}

function constantValue(node:HxcJsonNode):Null<String> {
	final direct = stringField(node, "value");
	if (direct != "")
		return direct;
	for (child in childNodes(node)) {
		final value = constantValue(child);
		if (value != null)
			return value;
	}
	return null;
}

function requiredProbe(values:Map<String, Int>, name:String):Int {
	final value = values.get(name);
	if (value == null)
		throw abiFailure('Clang omitted primitive probe `$name`', "Check the selected Clang AST JSON adapter and report its output.");
	return value;
}

function childNodes(node:HxcJsonNode):Array<HxcJsonNode> {
	final inner = objectField(node, "inner");
	return switch inner == null ? HxcJsonValue.JNull : inner.value {
		case JArray(values): values;
		case _: [];
	};
}

function typeSpelling(node:HxcJsonNode):String {
	final type = objectField(node, "type");
	if (type == null)
		return "";
	final desugared = stringField(type, "desugaredQualType");
	return desugared == "" ? stringField(type, "qualType") : desugared;
}

function sourceFile(node:HxcJsonNode):Null<String> {
	final location = objectField(node, "loc");
	if (location == null)
		return null;
	final file = stringField(location, "file");
	return file == "" ? null : file;
}

function sourceLocation(node:HxcJsonNode, file:String):HxcJsonNode {
	final location = objectField(node, "loc");
	return jsonObject([
		jsonField("file", jsonString(file)),
		jsonField("line", jsonInt(location == null ? 0 : intField(location, "line"))),
		jsonField("column", jsonInt(location == null ? 0 : intField(location, "col")))
	]);
}

function anonymousName(file:String, location:HxcJsonNode):String {
	final line = intField(location, "line");
	final column = intField(location, "column");
	return "anonymous-enum-" + Sha256.encode('$file:$line:$column').substr(0, 12);
}

function qualifierNames(spelling:String):Array<String> {
	final result:Array<String> = [];
	for (part in spelling.split(" ")) {
		final normalized = switch part {
			case "const": "const";
			case "volatile": "volatile";
			case "restrict", "__restrict", "__restrict__": "restrict";
			case _: "";
		};
		if (normalized != "" && result.indexOf(normalized) < 0)
			result.push(normalized);
	}
	result.sort(compareUtf8);
	return result;
}

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

function requireName(node:HxcJsonNode, kind:String):String {
	final name = stringField(node, "name");
	if (name == "")
		throw abiFailure('$kind has no native name', "Report the Clang AST node so deterministic naming can be extended.");
	return name;
}

function isAuthoredPath(path:String):Bool
	return StringTools.startsWith(path, "$SOURCE") || StringTools.startsWith(path, "$INCLUDE") || StringTools.startsWith(path, "$SYSROOT");

function compareNativeName(left:HxcJsonNode, right:HxcJsonNode):Int
	return compareUtf8(stringField(left, "nativeName"), stringField(right, "nativeName"));

function compareStableName(left:HxcJsonNode, right:HxcJsonNode):Int
	return compareUtf8(stringField(left, "stableName"), stringField(right, "stableName"));

function abiFailure(message:String, remediation:String):HxcBindgenError
	return new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0810", message, remediation);
