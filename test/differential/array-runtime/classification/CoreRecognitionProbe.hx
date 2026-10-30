#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyDate.CBodyDateRecognition;
#end

/** Aliases and ordinary abstracts must keep their different recognition rules. */
typedef RecognitionArrayAlias = Array<Int>;

typedef RecognitionDateAlias = Date;
typedef RecognitionGenericAlias<T> = Array<T>;
abstract RecognitionArrayBacked(Array<Int>) {}
abstract RecognitionGenericBacked<T>(Array<T>) {}

/**
	Protect type recognition, including effects of Haxe's macro type bridge.

	A recognizer returns only a Boolean, but following a type also crosses Haxe's
	macro bridge. The pinned Eval host rejects synthetic lazy callbacks, including
	nested ones. Fresh inputs prove recognition does not silently hide that error.
	Compiler-owned lazy values retain the same fallback; these synthetic tests do
	not manufacture or claim to exercise Haxe's private native lazy wrapper.
**/
class CoreRecognitionProbe {
	#if macro
	/** Resolve fixture declarations after macro initialization has completed. */
	public static function install():Void
		Context.onAfterInitMacros(check);

	static function require(value:Bool, message:String):Void {
		if (!value)
			throw new haxe.Exception(message);
	}

	/** Independently specify membership, arity, aliases, and lazy evaluation. */
	static function check():Void {
		final arrayRef = switch Context.getType("Array") {
			case TInst(reference, _): reference;
			case _: throw "core Array fixture";
		};
		final dateRef = switch Context.getType("Date") {
			case TInst(reference, _): reference;
			case _: throw "core Date fixture";
		};
		final nullRef = switch Context.getType("Null") {
			case TAbstract(reference, _): reference;
			case _: throw "core Null fixture";
		};
		final backedRef = switch Context.getType("CoreRecognitionProbe.RecognitionGenericBacked") {
			case TAbstract(reference, _): reference;
			case _: throw "generic abstract fixture";
		};
		final aliasRef = switch Context.getType("CoreRecognitionProbe.RecognitionGenericAlias") {
			case TType(reference, _): reference;
			case _: throw "generic alias fixture";
		};
		final intType = Context.getType("Int");
		final arrayType = TInst(arrayRef, [intType]);
		final dateType = TInst(dateRef, []);
		final recognizers:Array<Type->Bool> = [CBodyArrayRecognition.isCoreArrayType, CBodyDateRecognition.isCoreDateType];
		function expect(name:String, make:Void->Type, array:Bool, date:Bool):Void {
			for (index => recognize in recognizers)
				require(recognize(make()) == (index == 0 ? array : date), '$name recognizer $index');
		}
		expect("Array", () -> arrayType, true, false);
		expect("Date", () -> dateType, false, true);
		expect("Null Array", () -> TAbstract(nullRef, [arrayType]), true, false);
		expect("Null Date", () -> TAbstract(nullRef, [dateType]), false, true);
		expect("Array alias", () -> Context.getType("CoreRecognitionProbe.RecognitionArrayAlias"), true, false);
		expect("Date alias", () -> Context.getType("CoreRecognitionProbe.RecognitionDateAlias"), false, true);
		expect("Null alias", () -> TAbstract(nullRef, [TType(aliasRef, [intType])]), true, false);
		expect("namesake Array", () -> Context.getType("other.Array"), false, false);
		expect("namesake Date", () -> Context.getType("other.Date"), false, false);
		expect("namesake Null", () -> Context.getType("other.Null"), false, false);
		expect("ordinary Array abstract", () -> Context.getType("CoreRecognitionProbe.RecognitionArrayBacked"), false, false);
		expect("generic Array abstract", () -> TAbstract(backedRef, [intType]), false, false);
		expect("Date wrong arity", () -> TInst(dateRef, [intType]), false, false);
		// This helper recognizes identity; the Array registry owns arity rejection.
		expect("Array zero arity", () -> TInst(arrayRef, []), true, false);
		expect("Null zero arity", () -> TAbstract(nullRef, []), false, false);
		expect("Null wrong arity", () -> TAbstract(nullRef, [arrayType, intType]), false, false);
		for (name in ["Int", "Bool", "Float", "String"])
			expect(name, () -> Context.getType(name), false, false);
		for (target in [arrayType, dateType]) {
			expect("resolved monomorph", () -> {
				final mono = Context.makeMonomorph();
				require(Context.unify(mono, target), "bind monomorph");
				return mono;
			}, target == arrayType, target == dateType);
		}
		for (recognize in recognizers) {
			final mono = Context.makeMonomorph();
			require(!recognize(mono), "unresolved monomorph membership");
			switch mono {
				case TMono(reference):
					require(reference.get() == null, "recognition bound an unresolved monomorph");
				case _:
					throw "monomorph fixture";
			}
		}

		function lazyCase(name:String, inner:Type, wrap:Type->Type):Void {
			for (index => recognize in recognizers) {
				for (fail in [false, true]) {
					var calls = 0;
					final lazy = TLazy(() -> {
						calls++;
						if (fail)
							throw new haxe.Exception("recognizer-lazy-sentinel");
						return inner;
					});
					var result:Null<Bool> = null;
					var error:Null<String> = null;
					try
						result = recognize(wrap(lazy))
					catch (failure:haxe.Exception)
						error = failure.message;
					require(calls == 0, '$name recognizer $index invoked a rejected lazy callback');
					require(error == "Unexpected value VFunction(#fun), expected lazy type" && result == null,
						'$name recognizer $index changed the pinned lazy rejection: $error');
				}
			}
		}
		lazyCase("top Array", arrayType, type -> type);
		lazyCase("top Date", dateType, type -> type);
		lazyCase("Array parameter", intType, type -> TInst(arrayRef, [type]));
		lazyCase("Date wrong-arity parameter", intType, type -> TInst(dateRef, [type]));
		lazyCase("abstract parameter", intType, type -> TAbstract(backedRef, [type]));
		lazyCase("alias parameter", intType, type -> TType(aliasRef, [type]));
		lazyCase("Null parameter", arrayType, type -> TAbstract(nullRef, [type]));
		lazyCase("function result", intType, type -> TFun([], type));
		lazyCase("function argument", intType, type -> TFun([{name: "value", opt: false, t: type}], intType));
		lazyCase("Dynamic subtype", intType, type -> TDynamic(type));
	}
	#end
}
