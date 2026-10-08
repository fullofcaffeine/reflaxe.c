package hxc.config;

import haxe.Json;
import hxc.config.HxcJsonValue.HxcJsonField;
import hxc.config.HxcJsonValue.HxcJsonNode;

/**
	Render and transform the validated JSON tree without converting it to `Dynamic`.

	Inspection uses this module to retain exact number lexemes and object order while
	redacting path-shaped strings before they cross the command-line boundary.
**/
function renderJson(node:HxcJsonNode, pretty:Bool = false):String {
	final output = new StringBuf();
	renderValue(node.value, output, pretty, 0);
	return output.toString();
}

/** Return a deep copy in which absolute host paths are replaced by a stable token. */
function redactHostPaths(node:HxcJsonNode):HxcJsonNode {
	final value = switch node.value {
		case JNull: HxcJsonValue.JNull;
		case JBool(value): HxcJsonValue.JBool(value);
		case JNumber(value): HxcJsonValue.JNumber(value);
		case JString(value): HxcJsonValue.JString(isAbsolutePath(value) ? "<redacted-path>" : value);
		case JArray(values): HxcJsonValue.JArray(values.map(redactHostPaths));
		case JObject(fields): HxcJsonValue.JObject(fields.map(field -> new HxcJsonField(field.name, redactHostPaths(field.value), field.line, field.column)));
	};
	return new HxcJsonNode(value, node.line, node.column);
}

/** Find one unique field in an already duplicate-checked object. */
function objectField(node:HxcJsonNode, name:String):Null<HxcJsonNode> {
	return switch node.value {
		case JObject(fields):
			var found:Null<HxcJsonNode> = null;
			for (field in fields)
				if (field.name == name)
					found = field.value;
			found;
		case _: null;
	};
}

/** Create an artificial location-free object for a derived inspection view. */
function jsonObject(fields:Array<HxcJsonField>):HxcJsonNode
	return new HxcJsonNode(HxcJsonValue.JObject(fields), 0, 0);

/** Create an artificial location-free field for a derived inspection view. */
function jsonField(name:String, value:HxcJsonNode):HxcJsonField
	return new HxcJsonField(name, value, 0, 0);

/** Create a location-free string value. */
function jsonString(value:String):HxcJsonNode
	return new HxcJsonNode(HxcJsonValue.JString(value), 0, 0);

/** Create a location-free integer value. */
function jsonInt(value:Int):HxcJsonNode
	return new HxcJsonNode(HxcJsonValue.JNumber(Std.string(value)), 0, 0);

/** Create a location-free boolean value. */
function jsonBool(value:Bool):HxcJsonNode
	return new HxcJsonNode(HxcJsonValue.JBool(value), 0, 0);

/** Create a location-free array value. */
function jsonArray(values:Array<HxcJsonNode>):HxcJsonNode
	return new HxcJsonNode(HxcJsonValue.JArray(values), 0, 0);

function renderValue(value:HxcJsonValue, output:StringBuf, pretty:Bool, depth:Int):Void {
	switch value {
		case JNull:
			output.add("null");
		case JBool(value):
			output.add(value ? "true" : "false");
		case JNumber(value):
			output.add(value);
		case JString(value):
			output.add(Json.stringify(value));
		case JArray(values):
			output.add("[");
			for (index in 0...values.length) {
				if (index > 0)
					output.add(",");
				separator(output, pretty, depth + 1);
				renderValue(values[index].value, output, pretty, depth + 1);
			}
			if (values.length > 0)
				separator(output, pretty, depth);
			output.add("]");
		case JObject(fields):
			output.add("{");
			for (index in 0...fields.length) {
				if (index > 0)
					output.add(",");
				separator(output, pretty, depth + 1);
				output.add(Json.stringify(fields[index].name));
				output.add(pretty ? ": " : ":");
				renderValue(fields[index].value.value, output, pretty, depth + 1);
			}
			if (fields.length > 0)
				separator(output, pretty, depth);
			output.add("}");
	}
}

function separator(output:StringBuf, pretty:Bool, depth:Int):Void {
	if (!pretty)
		return;
	output.add("\n");
	for (_ in 0...depth)
		output.add("  ");
}

function isAbsolutePath(value:String):Bool
	return StringTools.startsWith(value, "/") || StringTools.startsWith(value, "~") || ~/^[A-Za-z]:[\\\/]/.match(value);
