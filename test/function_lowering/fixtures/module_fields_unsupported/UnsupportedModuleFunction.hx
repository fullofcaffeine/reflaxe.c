function optionalValue(?value:Int):Int
	return value == null ? 0 : value;

function main():Void {
	while (optionalValue() != 0) {}
}
