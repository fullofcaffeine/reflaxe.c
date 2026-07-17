package reflaxe.c.ast;

/** A structured C11 translation unit. Expand this instead of adding raw strings. */
class CTranslationUnit {
	public final includes:Array<CInclude> = [];
	public final declarations:Array<CDecl> = [];

	public function new() {}
}

/** A guarded C header whose preprocessor identifier is already finalized. */
class CHeaderUnit {
	public final guard:CIdentifier;
	public final translationUnit:CTranslationUnit;

	public function new(guard:CIdentifier, translationUnit:CTranslationUnit) {
		this.guard = guard;
		this.translationUnit = translationUnit;
	}
}

/**
	One already-finalized C identifier token.

	This validates lexical C11 spelling, not namespace ownership or mangling.
	`CSymbolRegistry` owns reserved-name and collision policy before AST creation.
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

	/** The standard `size_t` typedef from `<stddef.h>`. */
	TSizeT;

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

/** A structured standard `#line` directive. The mapped line applies to the next source line. */
typedef CLineDirective = {
	final line:Int;
	final file:Null<String>;
}

enum CDecl {
	DComment(text:String);
	DLineDirective(directive:CLineDirective);
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
	SLineDirective(directive:CLineDirective);
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

enum CIntegerBase {
	IBDecimal;
	IBOctal;
	IBHexadecimal;
}

enum CIntegerSuffix {
	ISNone;
	ISUnsigned;
	ISLong;
	ISUnsignedLong;
	ISLongLong;
	ISUnsignedLongLong;
}

/** A validated integer value plus typed C representation choices. The sign is a unary operator. */
class CIntegerLiteral {
	public final base:CIntegerBase;
	public final digits:String;
	public final suffix:CIntegerSuffix;

	public function new(base:CIntegerBase, digits:String, suffix:CIntegerSuffix = ISNone) {
		final radix = switch base {
			case IBDecimal: 10;
			case IBOctal: 8;
			case IBHexadecimal: 16;
		}
		if (!validDigits(digits, radix))
			throw 'Invalid base-$radix C integer magnitude: "$digits"';
		if (digits.length > 1 && digits.charCodeAt(0) == 0x30)
			throw 'C integer magnitudes must use canonical leading-zero spelling: "$digits"';
		this.base = base;
		this.digits = base == IBHexadecimal ? digits.toUpperCase() : digits;
		this.suffix = suffix;
	}

	public static function decimal(digits:String, suffix:CIntegerSuffix = ISNone):CIntegerLiteral
		return new CIntegerLiteral(IBDecimal, digits, suffix);

	static function validDigits(value:String, radix:Int):Bool {
		if (value.length == 0)
			return false;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null)
				return false;
			final digit = if (code >= 0x30 && code <= 0x39) code - 0x30 else if (code >= 0x41 && code <= 0x46) code - 0x41 + 10 else if (code >= 0x61
				&& code <= 0x66) code
				- 0x61 + 10 else -1;
			if (digit < 0 || digit >= radix)
				return false;
		}
		return true;
	}
}

enum CFloatRepresentation {
	FRDecimal(wholeDigits:String, fractionalDigits:String, exponent:Null<Int>);
	FRHexadecimal(wholeDigits:String, fractionalDigits:String, binaryExponent:Int);
}

enum CFloatSuffix {
	FSNone;
	FSFloat;
	FSLongDouble;
}

/** A structured floating value. The printer supplies canonical separators and exponent markers. */
class CFloatLiteral {
	public final representation:CFloatRepresentation;
	public final suffix:CFloatSuffix;

	public function new(representation:CFloatRepresentation, suffix:CFloatSuffix = FSNone) {
		switch representation {
			case FRDecimal(whole, fraction, _):
				validateParts(whole, fraction, 10);
			case FRHexadecimal(whole, fraction, _):
				validateParts(whole, fraction, 16);
		}
		this.representation = representation;
		this.suffix = suffix;
	}

	static function validateParts(whole:String, fraction:String, radix:Int):Void {
		if (whole == "" && fraction == "")
			throw "A C floating literal requires at least one significand digit";
		if (whole != "" && !validDigits(whole, radix) || fraction != "" && !validDigits(fraction, radix))
			throw 'Invalid base-$radix C floating significand: "$whole.$fraction"';
	}

	static function validDigits(value:String, radix:Int):Bool {
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null)
				return false;
			final digit = if (code >= 0x30 && code <= 0x39) code - 0x30 else if (code >= 0x41 && code <= 0x46) code - 0x41 + 10 else if (code >= 0x61
				&& code <= 0x66) code
				- 0x61 + 10 else -1;
			if (digit < 0 || digit >= radix)
				return false;
		}
		return true;
	}
}

/** A type association, or `type == null` for the one optional default association. */
typedef CGenericAssociation = {
	final type:Null<CTypedDeclarator>;
	final expression:CExpr;
}

enum CExpr {
	EIdentifier(name:CIdentifier);
	EInt(value:CIntegerLiteral);
	EFloat(value:CFloatLiteral);
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
	EOffsetOf(type:CType, declarator:CDeclarator, member:CIdentifier);
	ECompoundLiteral(type:CType, declarator:CDeclarator, initializer:CInitializer);
	EGenericSelection(control:CExpr, associations:Array<CGenericAssociation>);
	EParen(value:CExpr);
}
