enum GenericValue<T> {
	Present(value:T);
}

/**
 * Neighbor for specialization-size attribution.
 *
 * The generic source above and its use below match the baseline. The additional
 * ordinary enum is reachable C output, but it is not a generic specialization
 * and therefore must not consume the monomorphization budget.
 */
class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function main():Void {
		final value:GenericValue<Int> = Present(7);
		identity(value);
		consumeOrdinary(Unrelated.CaseF(11));
	}

	static function consumeOrdinary(value:Unrelated):Void {
		switch value {
			case CaseA | CaseB | CaseC | CaseD | CaseE:
			case CaseF(_):
		}
	}
}

enum Unrelated {
	CaseA;
	CaseB;
	CaseC;
	CaseD;
	CaseE;
	CaseF(value:Int);
}
