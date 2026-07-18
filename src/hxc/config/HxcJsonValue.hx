package hxc.config;

/** Location-bearing closed JSON tree used at the project-file boundary. */
enum HxcJsonValue {
	JNull;
	JBool(value:Bool);
	JNumber(lexeme:String);
	JString(value:String);
	JArray(values:Array<HxcJsonNode>);
	JObject(fields:Array<HxcJsonField>);
}

class HxcJsonNode {
	public final value:HxcJsonValue;
	public final line:Int;
	public final column:Int;

	public function new(value:HxcJsonValue, line:Int, column:Int) {
		this.value = value;
		this.line = line;
		this.column = column;
	}
}

class HxcJsonField {
	public final name:String;
	public final value:HxcJsonNode;
	public final line:Int;
	public final column:Int;

	public function new(name:String, value:HxcJsonNode, line:Int, column:Int) {
		this.name = name;
		this.value = value;
		this.line = line;
		this.column = column;
	}
}
