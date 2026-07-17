class ArrayRuntimeOracle {
	static function main():Void {
		final values = [1, 2, 3, 4];
		values.push(values[0]);
		values.insert(1, values[2]);
		values[2] = values[values.length - 1];
		values.splice(4, 1);
		values.splice(3, 1);
		// Eval is a dynamic target, so growing `resize` fills null rather than
		// the static Int default. Push the static-target defaults explicitly;
		// the C fixture separately proves failure-atomic resize growth.
		values.push(0);
		values.push(0);
		values.resize(5);
		Sys.println(values.length + ":" + values.join(","));
	}
}
