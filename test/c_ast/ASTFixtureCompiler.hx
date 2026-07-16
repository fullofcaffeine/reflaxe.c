import haxe.macro.Type.AbstractType;
import haxe.macro.Type.ClassType;
import haxe.macro.Type.DefType;
import haxe.macro.Type.EnumType;
import reflaxe.BaseCompiler;
import reflaxe.BaseCompiler.BaseCompilerFileOutputType;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.emit.ReflaxeOutputWriter;
import reflaxe.data.ClassFuncData;
import reflaxe.data.ClassVarData;
import reflaxe.data.EnumOptionData;
import reflaxe.output.DataAndFileInfo;
import reflaxe.output.StringOrBytes;

/** Emits the complete direct-C-AST corpus through Reflaxe output ownership. */
class ASTFixtureCompiler {
	public static function run(outputDirectory:String):Void {
		CASTGolden.verifyModelGuards();
		ExpressionGolden.verifyModelGuards();

		final strict = new CASTPrinter();
		final extensions = new CASTPrinter("  ", GnuC11);
		final files:Array<GeneratedFile> = [
			new GeneratedFile("declarators.c", strict.printTranslationUnit(CASTGolden.buildUnit()), GeneratedFileKind.Source),
			new GeneratedFile("expressions.c", strict.printTranslationUnit(ExpressionGolden.buildUnit()), GeneratedFileKind.Source),
			new GeneratedFile("attributes.c", extensions.printTranslationUnit(buildAttributeUnit()), GeneratedFileKind.Source),
			new GeneratedFile("include/hxc/ast_fixture.h", strict.printHeader(buildHeaderUnit()), GeneratedFileKind.PublicHeader),
			new GeneratedFile("src/ast_fixture.c", strict.printTranslationUnit(buildImplementationUnit()), GeneratedFileKind.Source),
			new GeneratedFile("src/main.c", strict.printTranslationUnit(buildConsumerUnit()), GeneratedFileKind.Source)
		];

		final compiler = new ASTFixtureOutputCompiler(files);
		compiler.setOptions({
			fileOutputType: BaseCompilerFileOutputType.Manual,
			fileOutputExtension: ".c",
			outputDirDefineName: "c_ast_fixture_output",
			deleteOldOutput: true
		});
		compiler.setOutputDir(outputDirectory);
		compiler.generateFiles();
		Sys.println("c-ast-fixture-macro: OK");
	}

	static function buildAttributeUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdio.h", kind: System});
		unit.declarations.push(DVariable({
			storage: [SStatic],
			alignments: [],
			type: nativeInt(),
			declarator: name("hxc_attribute_marker"),
			initializer: IExpr(integer("1")),
			attributes: [AUsed]
		}));
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: nativeInt(),
			declarator: DFunction(name("main"), FPPrototype([], false)),
			body: SBlock([
				SIf(EBinary(NotEqual, identifier("hxc_attribute_marker"), integer("1")), SReturn(integer("1")), null),
				SExpr(ECall(identifier("puts"), [EString("c-ast-attributes: OK")])),
				SReturn(integer("0"))
			]),
			attributes: []
		}));
		return unit;
	}

	static function buildHeaderUnit():CHeaderUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdint.h", kind: System});
		unit.declarations.push(DStruct(id("hxc_ast_pair"), [field(u32(), name("left")), field(u32(), name("right"))], []));
		unit.declarations.push(DPrototype([SExtern], [], u32(),
			DFunction(name("hxc_ast_sum"), FPPrototype([param(type(TStruct(id("hxc_ast_pair")), [QConst]), DPointer(name("pair"), []))], false)), []));
		return new CHeaderUnit(id("HXC_AST_FIXTURE_H"), unit);
	}

	static function buildImplementationUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "hxc/ast_fixture.h", kind: Local});
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: u32(),
			declarator: DFunction(name("hxc_ast_sum"), FPPrototype([param(type(TStruct(id("hxc_ast_pair")), [QConst]), DPointer(name("pair"), []))], false)),
			body: SBlock([
				SReturn(EBinary(Add, EMember(identifier("pair"), id("left"), true), EMember(identifier("pair"), id("right"), true)))
			]),
			attributes: []
		}));
		return unit;
	}

	static function buildConsumerUnit():CTranslationUnit {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdio.h", kind: System});
		unit.includes.push({path: "hxc/ast_fixture.h", kind: Local});
		unit.includes.push({path: "hxc/ast_fixture.h", kind: Local});
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: nativeInt(),
			declarator: DFunction(name("main"), FPPrototype([], false)),
			body: SBlock([
				SDecl({
					storage: [],
					alignments: [],
					type: type(TStruct(id("hxc_ast_pair"))),
					declarator: name("pair"),
					initializer: IList([
						{designators: [DField(id("left"))], value: IExpr(unsignedInteger("19"))},
						{designators: [DField(id("right"))], value: IExpr(unsignedInteger("23"))}
					]),
					attributes: []
				}),
				SIf(EBinary(NotEqual, ECall(identifier("hxc_ast_sum"), [EUnary(AddressOf, identifier("pair"))]), unsignedInteger("42")),
					SReturn(integer("1")), null),
				SExpr(ECall(identifier("puts"), [EString("c-ast-header-source: OK")])),
				SReturn(integer("0"))
			]),
			attributes: []
		}));
		return unit;
	}

	static function id(value:String):CIdentifier
		return new CIdentifier(value);

	static function identifier(value:String):CExpr
		return EIdentifier(id(value));

	static function type(spec:CTypeSpec, ?qualifiers:Array<CQualifier>):CType
		return new CType(spec, qualifiers);

	static function u32():CType
		return type(TInt(32, false));

	static function nativeInt():CType
		return type(TNativeInt(IRInt, true));

	static function integer(value:String):CExpr
		return EInt(CIntegerLiteral.decimal(value));

	static function unsignedInteger(value:String):CExpr
		return EInt(CIntegerLiteral.decimal(value, ISUnsigned));

	static function name(value:String):CDeclarator
		return DName(id(value));

	static function param(type:CType, declarator:CDeclarator):CParam
		return {type: type, declarator: declarator, attributes: []};

	static function field(type:CType, declarator:CDeclarator):CField
		return {
			type: type,
			declarator: declarator,
			bitWidth: null,
			alignments: [],
			attributes: []
		};
}

private class ASTFixtureOutputCompiler extends BaseCompiler {
	final files:Array<GeneratedFile>;

	public function new(files:Array<GeneratedFile>) {
		super();
		this.files = files;
	}

	override public function generateFilesManually():Void {
		final manager = output;
		if (manager == null)
			throw "missing Reflaxe output manager";
		new ReflaxeOutputWriter().write(manager, files);
	}

	public function generateOutputIterator():Iterator<DataAndFileInfo<StringOrBytes>> {
		final empty:Array<DataAndFileInfo<StringOrBytes>> = [];
		return empty.iterator();
	}

	public function compileClass(classType:ClassType, varFields:Array<ClassVarData>, funcFields:Array<ClassFuncData>):Void {}

	public function compileEnum(enumType:EnumType, options:Array<EnumOptionData>):Void {}

	public function compileTypedef(classType:DefType):Void {}

	public function compileAbstract(classType:AbstractType):Void {}
}
