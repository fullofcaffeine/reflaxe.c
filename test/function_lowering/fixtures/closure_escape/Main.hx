/**
	A negative lifetime fixture for the stack-only closure capability.

	Returning a callback could let it outlive the caller-owned variables captured
	by the lambda. The current stack representation must reject this program
	instead of emitting a dangling context pointer. Escaping closures remain owned
	by the broader managed-closure task.
**/
class Main {
	static function escape(operation:Int->Int):Int->Int {
		return operation;
	}

	static function main():Void {
		final offset = 4;
		escape(value -> value + offset);
	}
}
