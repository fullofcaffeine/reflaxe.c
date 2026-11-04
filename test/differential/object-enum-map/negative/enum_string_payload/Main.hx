/** Keeps payload-enum formatting fail-closed until recursive spelling is proven. */
final class Main {
	/** Ask for the unsupported text through an otherwise admitted typed map. */
	static function main():Void {
		final values:Map<PayloadKey, Int> = [];
		values.set(Amount(1), 2);
		values.toString();
	}
}

/** One supported map key whose payload text does not yet have a formatting rule. */
enum PayloadKey {
	Amount(value:Int);
}
