function restCount(values:haxe.Rest<Int>):Int
	return values.length;

function main():Void {
	while (restCount(1, 2) != 2) {}
}
