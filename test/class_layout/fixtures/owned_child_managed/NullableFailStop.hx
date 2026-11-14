/** A genuinely nullable receiver must still fail before dereferencing it. */
final class NullableFailStop {
	/** No IO or assertion failure can be confused with the receiver guard. */
	static function main():Void {
		OwnedCounter.readNullable(null);
	}
}
