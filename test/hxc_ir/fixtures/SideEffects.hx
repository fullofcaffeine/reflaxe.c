class SideEffects {
	static function main():Void {
		final values = [1];
		final updated = update(values);
		Sys.println('${SideEffectSupport.calls.join(",")}:$updated');
	}

	static function update(values:Array<Int>):Int {
		values[SideEffectSupport.nextIndex()] += SideEffectSupport.produce();
		return values[0];
	}
}
