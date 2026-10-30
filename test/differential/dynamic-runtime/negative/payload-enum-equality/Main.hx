/** A payload enum whose equality needs recursive typed payload semantics. */
private enum PayloadTone {
	Amount(value:Int);
}

/** Rejects payload-enum boxing instead of pretending tag-only equality. */
class Main {
	/** Keep both unsupported operands reachable at the equality boundary. */
	static function main():Void {
		final left:Dynamic = PayloadTone.Amount(7);
		final right:Dynamic = PayloadTone.Amount(7);
		Sys.println(left == right);
	}
}
