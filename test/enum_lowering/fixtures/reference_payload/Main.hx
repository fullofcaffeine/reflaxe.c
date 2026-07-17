package;

enum Bad {
	Text(value:String);
}

class Main {
	static function main():Void {
		var value = Text("unsupported");
		switch value {
			case Text(_):
		}
	}
}
