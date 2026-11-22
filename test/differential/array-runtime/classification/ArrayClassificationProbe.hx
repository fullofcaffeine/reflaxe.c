#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import haxe.macro.Expr.Position;
import reflaxe.c.CompilationContext;
import reflaxe.c.CProfile;
import reflaxe.c.frontend.TypedAstNormalizer;
import reflaxe.c.macros.TypedCContractMacro;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyArray.CPreparedBodyArray;
#end

/**
	Check Array representation identity without running the full C emitter.

	Real typed fixtures protect element lifetimes, aliases, header ownership, and
	source evidence. These contracts do not depend on the classifier's dispatch order.
**/
class ArrayClassificationProbe {
	#if macro
	/** Wait until Haxe can resolve the fixture's concrete types. */
	public static function install():Void {
		Context.onAfterInitMacros(check);
	}

	static function require(value:Bool, message:String):Void {
		if (!value)
			throw new haxe.Exception(message);
	}

	/** Check reused specializations and preserve the first Array use and each enum use. */
	static function check():Void {
		final path = "test/differential/array-runtime/classification/ArrayClassificationFixture.hx";
		final first = Context.makePosition({file: path, min: 0, max: 1});
		final second = Context.makePosition({file: path, min: 2, max: 3});
		final fields = switch Context.getType("ArrayClassificationFixture") {
			case TInst(ref, _): ref.get().fields.get();
			case _: throw "fixture";
		};
		final types:Map<String, Type> = [];
		for (field in fields)
			types.set(field.name, field.type);
		final modules:Array<ModuleType> = [
			switch Context.getType("imported.Array") {
				case TInst(reference, _):
					TClassDecl(reference);
				case _:
					throw "import fixture";
			}
		];
		final program = TypedAstNormalizer.normalize(modules, null, null);
		final contract = TypedCContractMacro.collect(modules);
		final registry = new CBodyAggregateRegistry(new CompilationContext(Portable), program, contract, true);
		final fail = (position:Position, message:String) -> {
			throw new haxe.Exception(message);
		};
		function classify(type:Type, position:Position):CBodyValueType {
			return registry.valueType(type, position, "ArrayClassificationFixture", path, fail, "probe");
		}
		function array(type:Type, position:Position):CPreparedBodyArray {
			return switch classify(type, position).kind {
				case CBVKArray(value): value;
				case _: throw "expected core array";
			};
		}
		final names = ["ints", "bools", "nested", "strings", "bytes", "enums", "classes", "functions"];
		for (name in names) {
			final value = array(types.get(name), first);
			final expectedLifecycle = switch value.lifecycle {
				case CBAELTrivial: name == "ints" || name == "bools" || name == "functions";
				case CBAELArray: name == "nested";
				case CBAELString: name == "strings";
				case CBAELBytes: name == "bytes";
				case CBAELEnum(_): name == "enums";
				case CBAELManagedClass(_): name == "classes";
				case _: false;
			};
			require(expectedLifecycle, "lifetime " + name);
			require(value.ownerModule == "ArrayClassificationFixture"
				&& value.source.display() == path + ":1:1-1:2", "source owner " + name);
			require(array(types.get(name), second) == value, "shared specialization " + name);
		}
		require(array(types.get("alias"), second) == array(types.get("ints"), first), "alias identity");
		require(array(types.get("ints"), first) != array(types.get("bools"), first), "element identity");
		for (name in ["imported", "importedAlias"])
			switch classify(types.get(name), first).kind {
				case CBVKImport(_):
				case _:
					throw "lost import precedence";
			}
		switch classify(types.get("namesake"), first).kind {
			case CBVKArray(_):
				throw "noncore Array misclassified";
			case _:
		}
		final enums = registry.canonicalEnums();
		require(enums.length == 1 && enums[0].reasonCount() == 2, "nested enum source ranges");
		final reasons = enums[0].canonicalReasons();
		require(reasons[0].display() == path + ":1:1-1:2" && reasons[1].display() == path + ":1:3-1:4", "enum source coordinates");
		final exact = types.get("ints");
		require(array(TLazy(() -> exact), second) == array(exact, first), "lazy fallback");
		final mono = Context.makeMonomorph();
		require(Context.unify(mono, exact), "bind monomorph");
		require(array(mono, second) == array(exact, first), "monomorph fallback");
		final bad = switch exact {
			case TInst(ref, _): TInst(ref, []);
			case _: throw "exact array";
		};
		var rejected = false;
		try {
			classify(bad, first);
		} catch (error:haxe.Exception) {
			rejected = error.message.indexOf("Array-arity:0") >= 0;
		}
		require(rejected, "invalid arity did not retain rejection");
		Sys.println("ARRAY_CLASSIFICATION_OK");
	}
	#end
}
