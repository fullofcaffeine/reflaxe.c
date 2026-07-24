class GenericBase {
	public function new() {}

	public function echo<T>(value:T):T {
		return value;
	}
}

class Main {
	static function main():Void {
		final receiver:GenericBase = new GenericBase();
		receiver.echo(7);
	}
}
