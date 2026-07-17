enum Layer<T> {
	Wrap(value:T);
}

class Main {
	static function grow<T>(value:T):T {
		grow(Layer.Wrap(value));
		return value;
	}

	static function main():Void {
		grow(1);
	}
}
