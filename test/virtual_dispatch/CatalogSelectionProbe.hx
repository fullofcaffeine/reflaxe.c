#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.frontend.TypedAstNormalizer;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchCatalog;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
#end

/**
	Checks dispatch discovery through its public catalog, without target emission.
	Real normalized classes supply hierarchy and method facts; explicit discovery
	steps independently specify which implementations must become reachable.
**/
class CatalogSelectionProbe {
	#if macro
	/** Wait until macro initialization permits ordinary source typing. */
	public static function run():Void {
		Context.onAfterInitMacros(check);
	}

	/** Alternate new classes and slots, including repeated negative answers. */
	static function check():Void {
		final declarations:Array<ModuleType> = [
			for (type in Context.getModule("CatalogSubject"))
				switch type {
					case TInst(reference, _):
						TClassDecl(reference);
					case _:
						throw new haxe.Exception("catalog fixture contains a non-class declaration");
				}
		];
		final program = TypedAstNormalizer.normalize(declarations, null, null);
		final catalog = fresh(program);
		expect(construct(catalog, "CatalogInherited"), []);
		final first = call(catalog, "baseCall");
		expect(first, ["method.CatalogBase.value"]);
		first.resize(0);
		expect(call(catalog, "baseCall"), ["method.CatalogBase.value"]);
		expect(construct(catalog, "CatalogUnrelated"), ["method.CatalogBase.value"]);
		expect(construct(catalog, "CatalogLeaf"), ["method.CatalogBase.value", "method.CatalogLeaf.value"]);
		expect(call(catalog, "leftCall"), ["method.CatalogBase.value", "method.CatalogLeaf.value"]);
		expect(construct(catalog, "CatalogBoth"), ["method.CatalogBase.value", "method.CatalogLeaf.value"]);
		expect(call(catalog, "rightCall"), ["method.CatalogBase.value", "method.CatalogLeaf.value"]);
		final all = ["method.CatalogBase.value", "method.CatalogLater.value", "method.CatalogLeaf.value"];
		expect(construct(catalog, "CatalogLater"), all);
		expect(construct(catalog, "CatalogLeaf"), all);
		final graph = catalog.finish();
		final slots = [for (slot in graph.slots) slot.id];
		if (slots.join("|") != "interface-slot.CatalogLeft.value|interface-slot.CatalogRight.value|slot.CatalogBase.value")
			throw new haxe.Exception("same-named interface slots lost their owners");
		for (slot in graph.slots) {
			final implementations = [for (id in slot.implementationIds.keys()) id];
			implementations.sort((left, right) -> left < right ? -1 : left > right ? 1 : 0);
			final expected = slot.id == "slot.CatalogBase.value" ? all : ["method.CatalogLeaf.value"];
			if (implementations.join("|") != expected.join("|"))
				throw new haxe.Exception('slot ${slot.id} lost an implementation');
		}
		for (owner in ["CatalogBase", "CatalogLeft", "CatalogRight"]) {
			final slot = graph.slotForMethodId('method.$owner.value');
			final expected = owner == "CatalogBase" ? "slot.CatalogBase.value" : 'interface-slot.$owner.value';
			if (slot == null || slot.id != expected)
				throw new haxe.Exception('method $owner lost its slot mapping');
		}
		// CatalogLeaf follows CatalogBoth and restores the ordinary class slot.
		final shared = graph.slotForMethodId("method.CatalogLeaf.value");
		if (shared == null || shared.id != "slot.CatalogBase.value")
			throw new haxe.Exception('shared implementation selected ${shared == null ? "no slot" : shared.id}');
		checkMissingMethod(declarations, program);
		Sys.println("CATALOG_SELECTION_OK");
	}

	/**
		A partial normalized input can refer to a base whose body is not indexed.
		Later unrelated discovery must not invent that missing method; a new full
		catalog must find it, using the same class and slot identities.
	**/
	static function checkMissingMethod(declarations:Array<ModuleType>, complete:TypedProgramInput):Void {
		final withoutBase = declarations.filter(declaration -> switch declaration {
			case TClassDecl(reference): reference.get().name != "CatalogBase";
			case _: true;
		});
		final missing = fresh(TypedAstNormalizer.normalize(withoutBase, null, null));
		expect(construct(missing, "CatalogInherited"), []);
		expect(call(missing, "baseCall"), []);
		expect(construct(missing, "CatalogUnrelated"), []);
		expect(call(missing, "leftCall"), []);
		expect(call(missing, "baseCall"), []);
		final next = fresh(complete);
		expect(construct(next, "CatalogInherited"), []);
		expect(call(next, "baseCall"), ["method.CatalogBase.value"]);
	}

	static function fresh(program:TypedProgramInput):CBodyDispatchCatalog
		return new CBodyDispatchCatalog(new CompilationContext(reflaxe.c.CProfile.Portable), program);

	static function reference(name:String):Ref<ClassType>
		return switch Context.getType("CatalogSubject." + name) {
			case TInst(reference, []): reference;
			case _: throw new haxe.Exception('missing non-generic fixture class $name');
		};

	static function construct(catalog:CBodyDispatchCatalog, name:String):Array<CBodyFunctionInput> {
		final type = reference(name);
		return catalog.markConstructed(type, [], null, type.get().pos, "CatalogSubject.hx");
	}

	/** Use the authored call tree, preserving its field reference and source position. */
	static function call(catalog:CBodyDispatchCatalog, name:String):Array<CBodyFunctionInput> {
		final owner = switch Context.getType("CatalogSubject") {
			case TInst(reference, _): reference.get();
			case _: throw new haxe.Exception("missing fixture owner");
		};
		final field = [for (field in owner.statics.get()) if (field.name == name) field][0];
		final expression = field.expr();
		if (expression == null)
			throw new haxe.Exception('missing authored call $name');
		var found:Null<TypedExpr> = null;
		function visit(expression:TypedExpr):Void {
			switch expression.expr {
				case TCall(_, _):
					if (found != null)
						throw new haxe.Exception("fixture call must contain one invocation");
					found = expression;
				case _:
			}
			TypedExprTools.iter(expression, visit);
		}
		visit(expression);
		if (found == null)
			throw new haxe.Exception('no authored invocation in $name');
		return catalog.collectCall(found, 'function.CatalogSubject.$name', "CatalogSubject.hx", null,
			(_, _, _) -> throw new haxe.Exception("non-generic catalog fixture requested specialization"));
	}

	static function expect(actual:Array<CBodyFunctionInput>, expected:Array<String>):Void {
		final names = [for (input in actual) 'method.${input.declarationPath}.${input.fieldName}'];
		if (names.join("|") != expected.join("|"))
			throw new haxe.Exception('selection mismatch: expected $expected, got $names');
	}
	#end
}
