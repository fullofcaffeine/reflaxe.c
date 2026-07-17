enum Layer<T> {
	LayerValue;
}

enum Growing<T> {
	Stop;
	Next(value:Growing<Layer<T>>);
}

class Main {
	static function main():Void {
		final value:Growing<Int> = Stop;
	}
}
