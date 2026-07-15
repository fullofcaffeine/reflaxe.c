package reflaxe.c.ast;

/** A structured seed for C11 syntax. Expand this instead of adding raw strings. */
class CTranslationUnit {
  public final includes:Array<CInclude> = [];
  public final declarations:Array<CDecl> = [];

  public function new() {}
}

enum CIncludeKind {
  System;
  Local;
}

typedef CInclude = {
  final path:String;
  final kind:CIncludeKind;
}

enum abstract CStorage(String) to String {
  var Auto = "auto";
  var Extern = "extern";
  var Static = "static";
  var Register = "register";
  var ThreadLocal = "_Thread_local";
  var Typedef = "typedef";
}

enum abstract CQualifier(String) to String {
  var Const = "const";
  var Restrict = "restrict";
  var Volatile = "volatile";
  var Atomic = "_Atomic";
}

enum CTypeSpec {
  TVoid;
  TBool;
  TChar(signed:Null<Bool>);
  TInt(bits:Int, signed:Bool);
  TFloat;
  TDouble;
  TNamed(name:String);
  TStruct(name:String);
  TUnion(name:String);
  TEnum(name:String);
}

/**
  C declarators must be represented separately from base type specifiers.
  For example, pointer-to-function and function-returning-pointer require
  parentheses that cannot be recovered safely from a flat type string.
*/
enum CDeclarator {
  /** Identifier, or the empty string for an abstract declarator in a type name. */
  DName(name:String);

  /** C grammar-level pointer prefix. Group explicitly when a postfix operator follows. */
  DPointer(inner:CDeclarator, qualifiers:Array<CQualifier>);

  /** C grammar-level array suffix. */
  DArray(inner:CDeclarator, size:Null<CExpr>, isStatic:Bool, qualifiers:Array<CQualifier>);

  /** C grammar-level function suffix. */
  DFunction(inner:CDeclarator, params:Array<CParam>, variadic:Bool);

  /** Explicit declarator parentheses, required for pointer-to-function/array shapes. */
  DGroup(inner:CDeclarator);
}

typedef CType = {
  final spec:CTypeSpec;
  final qualifiers:Array<CQualifier>;
}

typedef CTypedDeclarator = {
  final type:CType;
  final declarator:CDeclarator;
}

typedef CParam = {
  final type:CType;
  final declarator:CDeclarator;
}

typedef CField = {
  final type:CType;
  final declarator:CDeclarator;
  final bitWidth:Null<CExpr>;
}

typedef CEnumerator = {
  final name:String;
  final value:Null<CExpr>;
}

typedef CFunctionDef = {
  final storage:Array<CStorage>;
  final returnType:CType;
  final declarator:CDeclarator;
  final body:CStmt;
  final attributes:Array<String>;
}

typedef CVariableDecl = {
  final storage:Array<CStorage>;
  final type:CType;
  final declarator:CDeclarator;
  final initializer:Null<CInitializer>;
  final attributes:Array<String>;
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
  DField(name:String);
  DIndex(index:CExpr);
}

enum CDecl {
  DComment(text:String);
  DRawCompilerOwned(text:String);
  DStaticAssert(condition:CExpr, message:String);
  DForwardStruct(name:String);
  DForwardUnion(name:String);
  DStruct(name:String, fields:Array<CField>, attributes:Array<String>);
  DUnion(name:String, fields:Array<CField>, attributes:Array<String>);
  DEnum(name:String, values:Array<CEnumerator>, attributes:Array<String>);
  DTypedef(type:CType, declarator:CDeclarator);
  DVariable(variable:CVariableDecl);
  DFunction(function:CFunctionDef);
  DPrototype(storage:Array<CStorage>, returnType:CType, declarator:CDeclarator, attributes:Array<String>);
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
  SGoto(label:String);
  SLabel(label:String, statement:CStmt);
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
  EIdentifier(name:String);
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
  EMember(target:CExpr, field:String, throughPointer:Bool);
  ECast(type:CType, declarator:CDeclarator, value:CExpr);
  ESizeOfType(type:CType, declarator:CDeclarator);
  EAlignOfType(type:CType, declarator:CDeclarator);
  ECompoundLiteral(type:CType, declarator:CDeclarator, initializer:CInitializer);
  EParen(value:CExpr);
  ERawCompilerOwned(text:String);
}
