#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyDate.CBodyDateRecognition;
import reflaxe.c.lowering.CBodyIterator.CBodyIteratorRecognition;
import reflaxe.c.lowering.CBodyIterator.CBodyArrayIteratorKind;
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

	/** Genuine compiler declarations preserve identity through both public recognition APIs. */
	static function checkClassDeclarations():Void {
		function expect(name:String, date:Bool, iterator:Null<CBodyArrayIteratorKind>):Void {
			final original = switch Context.getType(name) {
				case TInst(reference, _): reference;
				case _: throw new haxe.Exception('class required: $name');
			};
			var reads = 0;
			final counted:Ref<ClassType> = {
				get: () -> {
					reads++;
					return original.get();
				},
				toString: () -> original.toString()
			};
			require(CBodyDateRecognition.isCoreDate(counted) == date, '$name Date reference');
			require(reads == 1, '$name Date reference read count');
			require(CBodyIteratorRecognition.arrayKind(counted) == iterator, '$name iterator reference');
			require(reads == 2, '$name iterator reference read count');
			final declaration = counted.get();
			require(CBodyDateRecognition.isCoreDateClass(declaration) == date, '$name Date declaration');
			require(CBodyIteratorRecognition.arrayKindClass(declaration) == iterator, '$name iterator declaration');
			require(reads == 3, '$name helpers reread reference');
		}
		expect("Date", true, null);
		expect("other.Date", false, null);
		expect("String", false, null);
		expect("haxe.iterators.ArrayIterator", false, CBAIValues);
		expect("haxe.iterators.ArrayKeyValueIterator", false, CBAIKeyValues);
		expect("haxe.iterators.MapKeyValueIterator", false, null);
		final failing:Ref<ClassType> = {
			get: () -> {
				throw new haxe.Exception("class-reference-sentinel");
			},
			toString: () -> "failing"
		};
		var failures = 0;
		try
			CBodyDateRecognition.isCoreDate(failing)
		catch (error:haxe.Exception)
			if (error.message == "class-reference-sentinel")
				failures++;
		try
			CBodyIteratorRecognition.arrayKind(failing)
		catch (error:haxe.Exception)
			if (error.message == "class-reference-sentinel")
				failures++;
		require(failures == 2, "reference failure preserved");
	}

	/** Independently specify membership, arity, aliases, and lazy evaluation. */
	static function check():Void {
		MapRecognitionProbe.check();
		checkClassDeclarations();
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
