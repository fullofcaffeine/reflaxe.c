/** Reject recursive enum keys until cyclic hashing has a termination contract. */
final class Main {
	static function main():Void {
		final map:Map<RecursiveKey, Int> = [];
		map.set(End, 1);
	}
}

/** One recursive enum shape outside the bounded first identity-map slice. */
enum RecursiveKey {
	End;
	Next(value:RecursiveKey);
}
