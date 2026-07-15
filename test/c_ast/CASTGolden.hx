import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;

/** Builds the strict-C11 declarator corpus exclusively through structural AST nodes. */
class CASTGolden {
	static function main():Void {
		verifyModelGuards();
		Sys.print(new CASTPrinter().printTranslationUnit(buildUnit()));
	}

	static function buildUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdio.h", kind: System});
		unit.includes.push({path: "stdint.h", kind: System});
		unit.includes.push({path: "stdatomic.h", kind: System});
		unit.includes.push({path: "stddef.h", kind: System});
		unit.includes.push({path: "stdbool.h", kind: System});

		unit.declarations.push(DComment("Declarators are semantic grammar trees, not target-code strings."));
		unit.declarations.push(DForwardStruct(id("hxc_opaque"), []));
		unit.declarations.push(DTypedef(type(TStruct(id("hxc_opaque"))), name("hxc_opaque"), []));

		unit.declarations.push(DTypedef(type(TEnumDefinition(null, [enumerator("HXC_MODE_FAST", EInt("1")), enumerator("HXC_MODE_SAFE", EInt("2"))], [])),
			name("hxc_mode"), []));

		unit.declarations.push(DTypedef(i32(), DFunction(DGroup(DPointer(name("hxc_binary_fn"), [])), FPPrototype([
			param(type(TInt(32, true), [QConst]), DPointer(name("left"), [QRestrict])),
			param(type(TInt(32, true), [QConst]), DPointer(name("right"), [QRestrict]))
		], false)), []));

		unit.declarations.push(DTypedef(i32(), DArray(DGroup(DPointer(name("hxc_matrix_ptr"), [])), ABFixed(EInt("4")), []), []));

		unit.declarations.push(DTypedef(i32(),
			DFunction(DGroup(DPointer(DArray(name("hxc_transform_table"), ABFixed(EInt("3")), []), [])), FPPrototype([param(i32(), name("value"))], false)),
			[]));

		unit.declarations.push(DStruct(id("hxc_dispatch"), [
			field(type(TNamed(id("hxc_binary_fn"))), name("binary")),
			field(i32(), DFunction(DGroup(DPointer(name("transform"), [])), FPPrototype([param(i32(), name("value"))], false))),
			field(i32(), DPointer(DArray(name("items"), ABFixed(EInt("4")), []), [])),
			field(i32(), DArray(DGroup(DPointer(name("matrix"), [])), ABFixed(EInt("4")), [])),
			field(type(TAtomic(i32(), abstractName())), name("counter"), [AlignExpr(EInt("16"))]),
			field(type(TUnionDefinition(null, [
				field(i32(), name("signed_value")),
				field(type(TInt(32, false)), name("unsigned_value"))
			],
				[])),
				abstractName()),
			field(type(TNativeInt(IRLong, false)), name("generation")),
			field(type(TBool), name("enabled")),
			field(type(TInt(8, false)), DArray(name("payload"), ABIncomplete, []))
		], []));

		unit.declarations.push(DPrototype([SExtern], [], type(TVoid), DFunction(name("hxc_visit"), FPPrototype([
			param(type(TNamed(id("size_t"))), name("rows")),
			param(type(TNamed(id("size_t"))), name("columns")),
			param(i32(), DArray(DArray(name("matrix"), ABStaticMinimum(EIdentifier(id("rows"))), [QRestrict]), ABVariable, []))
		], false)), []));

		unit.declarations.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(type(TStruct(id("hxc_dispatch"))), abstractName()), EInt("16")),
			"aligned atomic field raises aggregate alignment"));
		unit.declarations.push(DStaticAssert(EBinary(Greater, ESizeOfType(i32(), DArray(DGroup(DPointer(abstractName(), [])), ABFixed(EInt("4")), [])),
			EInt("0")),
			"abstract pointer-to-array declarator is complete"));

		unit.declarations.push(DFunction({
			storage: [SStatic],
			functionSpecifiers: [FInline],
			returnType: i32(),
			declarator: DPointer(DFunction(name("hxc_select"), FPPrototype([
				param(type(TNamed(id("hxc_binary_fn"))), name("callback")),
				param(i32(), DPointer(name("left"), []))
			], false)), []),
			body: SBlock([
				SExpr(ECast(type(TVoid), abstractName(), EIdentifier(id("callback")))),
				SReturn(EIdentifier(id("left")))
			]),
			attributes: []
		}));

		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: type(TNativeInt(IRInt, true)),
			declarator: DFunction(name("main"), FPPrototype([], false)),
			body: SBlock([
				SDecl({
					storage: [],
					alignments: [],
					type: type(TStruct(id("hxc_dispatch"))),
					declarator: name("dispatch"),
					initializer: IList([
						{designators: [DField(id("counter"))], value: IExpr(EInt("0"))},
						{designators: [DField(id("enabled"))], value: IExpr(EBool(true))}
					]),
					attributes: []
				}),
				SExpr(ECall(EIdentifier(id("hxc_select")),
					[
						EMember(EIdentifier(id("dispatch")), id("binary"), false),
						EIndex(EMember(EIdentifier(id("dispatch")), id("items"), false), EInt("0"))
					])),
				SExpr(ECast(type(TVoid), abstractName(), EIdentifier(id("dispatch")))),
				SExpr(ECall(EIdentifier(id("puts")), [EString("c-ast-golden: OK")])),
				SReturn(EInt("0"))
			]),
			attributes: []
		}));

		return unit;
	}

	static function verifyModelGuards():Void {
		final strict = new CASTPrinter();
		assertEqual("int32_t (*callback)(int32_t value)",
			strict.printTypedDeclarator(i32(), DFunction(DGroup(DPointer(name("callback"), [])), FPPrototype([param(i32(), name("value"))], false))));
		assertEqual("int32_t legacy()", strict.printTypedDeclarator(i32(), DFunction(name("legacy"), FPUnspecified)));
		assertEqual("int32_t legacy(value)", strict.printTypedDeclarator(i32(), DFunction(name("legacy"), FPIdentifierList([id("value")]))));
		assertEqual("long double _Complex", strict.printType(type(TComplex(RLongDouble))));

		expectFailure("keyword identifier", () -> new CIdentifier("while"));
		expectFailure("punctuated identifier", () -> new CIdentifier("value;"));
		expectFailure("strict extension attribute", () -> strict.printDecl(DVariable({
			storage: [SStatic],
			alignments: [],
			type: i32(),
			declarator: name("fast_counter"),
			initializer: null,
			attributes: [ASection("fast"), AUsed]
		})));
		expectFailure("named atomic type-name", () -> strict.printType(type(TAtomic(i32(), name("not_abstract")))));
		expectFailure("duplicate qualifier", () -> strict.printType(type(TInt(32, true), [QConst, QConst])));
		expectFailure("invalid variadic prototype", () -> strict.printDeclarator(DFunction(name("invalid"), FPPrototype([], true))));

		final extensions = new CASTPrinter("  ", GnuC11);
		assertEqual("int32_t (__attribute__((cdecl)) *callback)(int32_t value)",
			extensions.printTypedDeclarator(i32(),
				DFunction(DGroup(DAttributed(DPointer(name("callback"), []), [ACallingConvention(CCCdecl)])),
					FPPrototype([param(i32(), name("value"))], false))));
		assertEqual('static int32_t fast_counter __attribute__((section("fast"), used));', extensions.printDecl(DVariable({
			storage: [SStatic],
			alignments: [],
			type: i32(),
			declarator: name("fast_counter"),
			initializer: null,
			attributes: [ASection("fast"), AUsed]
		})));
	}

	static function id(value:String):CIdentifier
		return new CIdentifier(value);

	static function type(spec:CTypeSpec, ?qualifiers:Array<CQualifier>):CType
		return new CType(spec, qualifiers);

	static function i32():CType
		return type(TInt(32, true));

	static function name(value:String):CDeclarator
		return DName(id(value));

	static function abstractName():CDeclarator
		return DName(null);

	static function param(type:CType, declarator:CDeclarator):CParam
		return {type: type, declarator: declarator, attributes: []};

	static function field(type:CType, declarator:CDeclarator, ?alignments:Array<CAlignment>):CField
		return {
			type: type,
			declarator: declarator,
			bitWidth: null,
			alignments: alignments == null ? [] : alignments,
			attributes: []
		};

	static function enumerator(name:String, value:CExpr):CEnumerator
		return {name: id(name), value: value, attributes: []};

	static function assertEqual(expected:String, actual:String):Void {
		if (actual != expected)
			throw 'C AST assertion failed\nexpected: $expected\nactual:   $actual';
	}

	static function expectFailure(label:String, action:Void->Dynamic):Void {
		var failed = false;
		try {
			action();
		} catch (_:Dynamic) {
			failed = true;
		}
		if (!failed)
			throw 'Expected C AST failure: $label';
	}
}
