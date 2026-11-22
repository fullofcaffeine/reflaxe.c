/** Two independent enum shapes exercise provenance discovered after a checkpoint. */
enum FirstReasonEnum {
	First;
}

/** A later-discovered shape must retain its initial source range too. */
enum SecondReasonEnum {
	Second;
}

/** A transparent alias must retain the original enum's identity and use sites. */
typedef FirstReasonAlias = FirstReasonEnum;

/** Concrete payload arguments must remain distinct through value classification. */
enum GenericReasonEnum<T> {
	Present(value:T);
}

/** Preparing a recursive payload must find the enum whose cases are still being built. */
enum RecursiveReasonEnum {
	End;
	Next(child:RecursiveReasonEnum);
}

/** Ordinary enum preparation must reject externally owned representations. */
extern enum ExternalReasonEnum {
	External;
}

/** Minimal Eval entry point for the compiler-side provenance contract. */
class ProvenanceFixture {
	/** The macro performs the assertions; runtime execution has no side effects. */
	static function main():Void {}
}
