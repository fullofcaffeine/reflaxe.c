/** Two independent enum shapes exercise provenance discovered after a checkpoint. */
enum FirstReasonEnum {
	First;
}

/** A later-discovered shape must retain its initial source range too. */
enum SecondReasonEnum {
	Second;
}

/** Minimal Eval entry point for the compiler-side provenance contract. */
class ProvenanceFixture {
	/** The macro performs the assertions; runtime execution has no side effects. */
	static function main():Void {}
}
