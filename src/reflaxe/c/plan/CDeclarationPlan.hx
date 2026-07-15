package reflaxe.c.plan;

/** Visibility of one generated header after contract normalization. */
enum CPlanHeaderVisibility {
	PHVPublic;
	PHVPrivate;
}

/** Include spelling is structural so emitters never reconstruct delimiters. */
enum CPlanIncludeKind {
	PIKSystem;
	PIKLocal;
}

/** Every planned include retains the fact or dependency that selected it. */
enum CPlanIncludeReason {
	PIRBuildFact(ownerModulePath:String);
	PIRCompleteType(consumerModulePath:String, targetModulePath:String);
	PIRExternalType(consumerModulePath:String, targetModulePath:String);
}

/** The declaration category required before a pointer-only use. */
enum CPlanForwardKind {
	PFKStruct;
	PFKUnion;
	PFKOpaque;
}

/** What the later emitter must do with a declaration at its planned location. */
enum CPlanDeclarationEmission {
	PDEComplete;
	PDEOpaque;
	PDEExternal;
}

typedef CPlannedInclude = {
	final path:String;
	final kind:CPlanIncludeKind;
	final reasons:Array<CPlanIncludeReason>;
}

typedef CPlannedForwardDeclaration = {
	final modulePath:String;
	final cName:String;
	final kind:CPlanForwardKind;
}

typedef CPlannedDeclaration = {
	final modulePath:String;
	final cName:Null<String>;
	final layout:Null<String>;
	final emission:CPlanDeclarationEmission;
}

typedef CHeaderPlan = {
	final path:String;
	final visibility:CPlanHeaderVisibility;
	final guard:String;
	final includes:Array<CPlannedInclude>;
	final forwardDeclarations:Array<CPlannedForwardDeclaration>;
	final declarations:Array<CPlannedDeclaration>;
}

/**
	Deterministic declaration/header plan consumed by later C AST emission.

	`source*` describes the provisional shared implementation translation unit;
	E1.T07 may split it without changing the dependency decisions recorded here.
 */
typedef CDeclarationPlan = {
	final schemaVersion:Int;
	final inputSchemaVersion:Int;
	final headers:Array<CHeaderPlan>;
	final sourceIncludes:Array<CPlannedInclude>;
	final sourceForwardDeclarations:Array<CPlannedForwardDeclaration>;
	final sourceDeclarations:Array<CPlannedDeclaration>;
	final buildIncludes:Array<CPlannedInclude>;
}
