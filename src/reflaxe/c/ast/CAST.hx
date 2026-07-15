package reflaxe.c.ast;

/** A structured C11 translation unit. Expand this instead of adding raw strings. */
class CTranslationUnit {
	public final includes:Array<CInclude> = [];
	public final declarations:Array<CDecl> = [];

	public function new() {}
}

/**
	One already-finalized C identifier token.

	This validates lexical C11 spelling, not namespace ownership or mangling.
	The symbol registry owns reserved-name and collision policy in E1.T04.
 */
class CIdentifier {
	public final value:String;

	public function new(value:String) {
		if (!isIdentifier(value) || isKeyword(value))
			throw 'Invalid C11 identifier: "$value"';
		this.value = value;
	}

	public function toString():String
		return value;

	static function isIdentifier(value:String):Bool {
		if (value.length == 0 || !isIdentifierStart(value.charCodeAt(0)))
			return false;
		for (index in 1...value.length) {
			if (!isIdentifierContinue(value.charCodeAt(index)))
				return false;
		}
		return true;
	}

	static function isIdentifierStart(code:Null<Int>):Bool
		return code != null && (code == 0x5F || code >= 0x41 && code <= 0x5A || code >= 0x61 && code <= 0x7A);

	static function isIdentifierContinue(code:Null<Int>):Bool
		return isIdentifierStart(code) || code != null && code >= 0x30 && code <= 0x39;

	static function isKeyword(value:String):Bool {
		return switch value {
			case "_Alignas" | "_Alignof" | "_Atomic" | "_Bool" | "_Complex" | "_Generic" | "_Imaginary" | "_Noreturn" | "_Static_assert" | "_Thread_local" |
				"auto" | "break" | "case" | "char" | "const" | "continue" | "default" | "do" | "double" | "else" | "enum" | "extern" | "float" | "for" |
				"goto" | "if" | "inline" | "int" | "long" | "register" | "restrict" | "return" | "short" | "signed" | "sizeof" | "static" | "struct" |
				"switch" | "typedef" | "union" | "unsigned" | "void" | "volatile" | "while":
				true;
			case _:
				false;
		}
	}
}

enum CIncludeKind {
	System;
	Local;
}

typedef CInclude = {
	final path:String;
	final kind:CIncludeKind;
}

enum CStorage {
	SAuto;
	SExtern;
	SStatic;
	SRegister;
	SThreadLocal;
	STypedef;
}

enum CFunctionSpecifier {
	FInline;
	FNoReturn;
}

enum CQualifier {
	QConst;
	QRestrict;
	QVolatile;
	QAtomic;
}

enum CNativeIntegerRank {
	IRShort;
	IRInt;
	IRLong;
	IRLongLong;
}

enum CRealType {
	RFloat;
	RDouble;
	RLongDouble;
}

/** Normative output is StrictC11. Other modes gate explicit extension nodes. */
enum CDialect {
	StrictC11;
	GnuC11;
	ClangC11;
}

enum CCallingConvention {
	CCCdecl;
	CCStdcall;
	CCFastcall;
	CCVectorcall;
}

enum CVisibility {
	VDefault;
	VHidden;
	VProtected;
	VInternal;
}

/** Typed compiler-extension attributes; never user-authored C token strings. */
enum CAttribute {
	APacked;
	AAligned(alignment:Null<CExpr>);
	ASection(name:String);
	AVisibility(visibility:CVisibility);
	ACallingConvention(convention:CCallingConvention);
	AUsed;
	AUnused;
	AWeak;
}

/** Standard C11 alignment specifiers. */
enum CAlignment {
	AlignExpr(alignment:CExpr);
	AlignType(type:CType, declarator:CDeclarator);
}

enum CTypeSpec {
	TVoid;
	TBool;
	TChar(signed:Null<Bool>);

	/** A C short/int/long/long-long type whose width is ABI-defined. */
	TNativeInt(rank:CNativeIntegerRank, signed:Bool);

	/** An exact-width stdint typedef. Supported widths are validated by the printer. */
	TInt(bits:Int, signed:Bool);

	TFloat;
	TDouble;
	TLongDouble;
	TComplex(realType:CRealType);
	TNamed(name:CIdentifier);
	TStruct(name:CIdentifier);
	TUnion(name:CIdentifier);
	TEnum(name:CIdentifier);

	/** Complete, optionally tagged aggregate specifiers usable inside a declaration. */
	TStructDefinition(name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>);

	TUnionDefinition(name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>);
	TEnumDefinition(name:Null<CIdentifier>, values:Array<CEnumerator>, attributes:Array<CAttribute>);

	/** The C11 `_Atomic(type-name)` form, distinct from the `_Atomic` qualifier. */
	TAtomic(type:CType, declarator:CDeclarator);
}

class CType {
	public final spec:CTypeSpec;
	public final qualifiers:Array<CQualifier>;

	public function new(spec:CTypeSpec, ?qualifiers:Array<CQualifier>) {
		this.spec = spec;
		this.qualifiers = qualifiers == null ? [] : qualifiers.copy();
	}
}

enum CArrayBound {
	/** `[]`, including a flexible array member in field context. */
	ABIncomplete;

	/** `[*]`, valid only for a variably modified array at function prototype scope. */
	ABVariable;

	/** `[expression]`. */
	ABFixed(size:CExpr);

	/** `[static expression]` in a function parameter declaration. */
	ABStaticMinimum(size:CExpr);
}

enum CFunctionParameters {
	/** A prototype; an empty parameter array prints `(void)`. */
	FPPrototype(params:Array<CParam>, variadic:Bool);

	/** The legacy C11 non-prototype `()` form. Generated target APIs should not use it. */
	FPUnspecified;

	/** A legacy identifier-list declarator, retained so the grammar is structural. */
	FPIdentifierList(names:Array<CIdentifier>);
}

/**
	C declarators are grammar trees, separate from normalized base type specifiers.
	`DGroup` is the C parenthesized-declarator production, not a printer hint.
 */
enum CDeclarator {
	/** A declared identifier, or null for an abstract declarator/type name. */
	DName(name:Null<CIdentifier>);

	/** C grammar-level pointer prefix. */
	DPointer(inner:CDeclarator, qualifiers:Array<CQualifier>);

	/** C grammar-level array suffix. */
	DArray(inner:CDeclarator, bound:CArrayBound, qualifiers:Array<CQualifier>);

	/** C grammar-level function suffix. */
	DFunction(inner:CDeclarator, parameters:CFunctionParameters);

	/** The parenthesized-declarator grammar production. */
	DGroup(inner:CDeclarator);

	/** A dialect-gated attribute at an exact declarator position. */
	DAttributed(inner:CDeclarator, attributes:Array<CAttribute>);
}

typedef CTypedDeclarator = {
	final type:CType;
	final declarator:CDeclarator;
}

typedef CParam = {
	final type:CType;
	final declarator:CDeclarator;
	final attributes:Array<CAttribute>;
}

typedef CField = {
	final type:CType;
	final declarator:CDeclarator;
	final bitWidth:Null<CExpr>;
	final alignments:Array<CAlignment>;
	final attributes:Array<CAttribute>;
}

typedef CEnumerator = {
	final name:CIdentifier;
	final value:Null<CExpr>;
	final attributes:Array<CAttribute>;
}

typedef CFunctionDef = {
	final storage:Array<CStorage>;
	final functionSpecifiers:Array<CFunctionSpecifier>;
	final returnType:CType;
	final declarator:CDeclarator;
	final body:CStmt;
	final attributes:Array<CAttribute>;
}

typedef CVariableDecl = {
	final storage:Array<CStorage>;
	final alignments:Array<CAlignment>;
	final type:CType;
	final declarator:CDeclarator;
	final initializer:Null<CInitializer>;
	final attributes:Array<CAttribute>;
}

enum CInitializer {
	IExpr(expr:CExpr);
	IList(items:Array<CInitializerItem>);
}

typedef CInitializerItem = {
	final designators:Array<CDesignator>;
	final value:CInitializer;
}

enum CDesignator {
	DField(name:CIdentifier);
	DIndex(index:CExpr);
}

enum CDecl {
	DComment(text:String);
	DRawCompilerOwned(text:String);
	DStaticAssert(condition:CExpr, message:String);
	DForwardStruct(name:CIdentifier, attributes:Array<CAttribute>);
	DForwardUnion(name:CIdentifier, attributes:Array<CAttribute>);
	DStruct(name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>);
	DUnion(name:Null<CIdentifier>, fields:Array<CField>, attributes:Array<CAttribute>);
	DEnum(name:Null<CIdentifier>, values:Array<CEnumerator>, attributes:Array<CAttribute>);
	DTypedef(type:CType, declarator:CDeclarator, attributes:Array<CAttribute>);
	DVariable(variable:CVariableDecl);
	DFunction(definition:CFunctionDef);
	DPrototype(storage:Array<CStorage>, functionSpecifiers:Array<CFunctionSpecifier>, returnType:CType, declarator:CDeclarator, attributes:Array<CAttribute>);
}

enum CStmt {
	SEmpty;
	SComment(text:String);
	SExpr(expr:CExpr);
	SDecl(decl:CVariableDecl);
	SBlock(statements:Array<CStmt>);
	SIf(condition:CExpr, thenBranch:CStmt, elseBranch:Null<CStmt>);
	SWhile(condition:CExpr, body:CStmt);
	SDoWhile(body:CStmt, condition:CExpr);
	SFor(init:Null<CStmt>, condition:Null<CExpr>, step:Null<CExpr>, body:CStmt);
	SSwitch(value:CExpr, cases:Array<CCase>);
	SReturn(value:Null<CExpr>);
	SBreak;
	SContinue;
	SGoto(label:CIdentifier);
	SLabel(label:CIdentifier, statement:CStmt);
}

typedef CCase = {
	final values:Array<CExpr>;
	final isDefault:Bool;
	final body:Array<CStmt>;
}

enum CUnaryOp {
	AddressOf;
	Dereference;
	Plus;
	Minus;
	LogicalNot;
	BitwiseNot;
	PreIncrement;
	PreDecrement;
	PostIncrement;
	PostDecrement;
	SizeOfExpr;
}

enum CBinaryOp {
	Multiply;
	Divide;
	Modulo;
	Add;
	Subtract;
	ShiftLeft;
	ShiftRight;
	Less;
	LessEqual;
	Greater;
	GreaterEqual;
	Equal;
	NotEqual;
	BitAnd;
	BitXor;
	BitOr;
	LogicalAnd;
	LogicalOr;
	Assign;
	AddAssign;
	SubtractAssign;
	MultiplyAssign;
	DivideAssign;
	ModuloAssign;
	ShiftLeftAssign;
	ShiftRightAssign;
	BitAndAssign;
	BitXorAssign;
	BitOrAssign;
	Comma;
}

enum CExpr {
	EIdentifier(name:CIdentifier);
	EInt(value:String);
	EFloat(value:String);
	EString(value:String);
	EChar(value:Int);
	EBool(value:Bool);
	ENull;
	EUnary(op:CUnaryOp, value:CExpr);
	EBinary(op:CBinaryOp, left:CExpr, right:CExpr);
	EConditional(condition:CExpr, thenValue:CExpr, elseValue:CExpr);
	ECall(callee:CExpr, args:Array<CExpr>);
	EIndex(target:CExpr, index:CExpr);
	EMember(target:CExpr, field:CIdentifier, throughPointer:Bool);
	ECast(type:CType, declarator:CDeclarator, value:CExpr);
	ESizeOfType(type:CType, declarator:CDeclarator);
	EAlignOfType(type:CType, declarator:CDeclarator);
	ECompoundLiteral(type:CType, declarator:CDeclarator, initializer:CInitializer);
	EParen(value:CExpr);
	ERawCompilerOwned(text:String);
}
