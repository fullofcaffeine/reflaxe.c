/** Rejects reflection-backed member lookup from a runtime String. */
class Main {
	/** A computed name has no closed-world numeric member token. */
	static function main():Void {
		final value:Dynamic = ({answer: 42} : {answer:Int});
		final name = "answer";
		final answer:Int = Reflect.field(value, name);
		Sys.println(answer);
	}
}
