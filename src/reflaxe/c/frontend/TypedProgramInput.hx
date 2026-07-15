package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.MetadataEntry;
import haxe.macro.Type.ClassField;
import haxe.macro.Type.EnumField;
import haxe.macro.Type.ModuleType;
import haxe.macro.Type.TypedExpr;

/** A normalized metadata entry whose arguments have stable source rendering. */
typedef TypedAstMetadata = {
	final sourceOrder:Int;
	final name:String;
	final arguments:Array<String>;
	final raw:MetadataEntry;
}

/** A class field or enum constructor retained for later semantic lowering. */
typedef TypedAstField = {
	final sourceOrder:Int;
	final name:String;
	final role:String;
	final kind:String;
	final typeKind:String;
	final typeDisplay:String;
	final isPublic:Bool;
	final isExtern:Bool;
	final metadata:Array<TypedAstMetadata>;
	final expression:Null<TypedExpr>;
	final rawClassField:Null<ClassField>;
	final rawEnumField:Null<EnumField>;
}

/** One Haxe declaration with explicit source-module ownership. */
typedef TypedAstDeclaration = {
	final path:String;
	final name:String;
	final kind:String;
	final ownerModulePath:String;
	final ownership:String;
	final sourcePath:String;
	final isPrivate:Bool;
	final isExtern:Bool;
	final classKind:Null<String>;
	final isInterface:Bool;
	final isFinal:Bool;
	final isAbstract:Bool;
	final representedTypeKind:Null<String>;
	final representedTypeDisplay:Null<String>;
	final metadata:Array<TypedAstMetadata>;
	final fields:Array<TypedAstField>;
	final raw:ModuleType;
}

/** All declarations owned by one logical Haxe source module. */
typedef TypedAstModule = {
	final path:String;
	final sourcePath:String;
	final declarations:Array<TypedAstDeclaration>;
}

/** A root expression and its stable ownership coordinates. */
typedef TypedAstExpressionRoot = {
	final sourceOrder:Int;
	final ownerModulePath:String;
	final ownerDeclarationPath:Null<String>;
	final fieldName:Null<String>;
	final role:String;
	final expression:TypedExpr;
}

/** Entry-point facts kept independently from Reflaxe callback order. */
typedef TypedAstEntryPoint = {
	final modulePath:Null<String>;
	final declarationPath:Null<String>;
	final expression:TypedExpr;
}

/**
	Deterministic, target-owned view of Haxe's typed whole-program input.

	Raw compiler objects remain attached for lowering. Stable reports are built
	from the normalized records and never serialize raw positions or host paths.
**/
class TypedProgramInput {
	public final modules:Array<TypedAstModule>;
	public final declarations:Array<TypedAstDeclaration>;
	public final expressionRoots:Array<TypedAstExpressionRoot>;
	public final entryPoint:Null<TypedAstEntryPoint>;
	public final rawModules:Array<ModuleType>;

	public function new(modules:Array<TypedAstModule>, declarations:Array<TypedAstDeclaration>, expressionRoots:Array<TypedAstExpressionRoot>,
			entryPoint:Null<TypedAstEntryPoint>, rawModules:Array<ModuleType>) {
		this.modules = modules;
		this.declarations = declarations;
		this.expressionRoots = expressionRoots;
		this.entryPoint = entryPoint;
		this.rawModules = rawModules;
	}
}
#else
class TypedProgramInput {
	public function new() {}
}
#end
