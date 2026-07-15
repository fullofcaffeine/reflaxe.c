package reflaxe.c.contract;

/** Structural type reference consumed by declaration planning; never target code. */
typedef TypedCTypeRef = {
	final kind:String;
	final name:Null<String>;
	final arguments:Array<TypedCTypeRef>;
}

/** One typed field, function, or constant in a C-facing declaration. */
typedef TypedCContractField = {
	final name:String;
	final cName:Null<String>;
	final kind:String;
	final type:TypedCTypeRef;
	final bitWidth:Null<Int>;
	final align:Null<Int>;
	final exported:Bool;
	final linkage:Null<String>;
	final callingConvention:Null<String>;
	final visibility:Null<String>;
	final section:Null<String>;
}

/** Header-planning edge derived from typed use of another C declaration. */
typedef TypedCDependency = {
	final modulePath:String;
	final requirement:String;
}

/** One validated, source-authored C declaration contract. */
typedef TypedCDeclaration = {
	final modulePath:String;
	final sourceKind:String;
	final layout:Null<String>;
	final cName:Null<String>;
	final headerPath:Null<String>;
	final headerVisibility:Null<String>;
	final pack:Null<Int>;
	final align:Null<Int>;
	final exported:Bool;
	final linkage:Null<String>;
	final callingConvention:Null<String>;
	final visibility:Null<String>;
	final section:Null<String>;
	final dependencies:Array<TypedCDependency>;
	final fields:Array<TypedCContractField>;
}

/** A normalized external build input. Values are literal, never shell fragments. */
typedef TypedCBuildFact = {
	final kind:String;
	final name:String;
	final value:Null<String>;
	final valueKind:Null<String>;
}

/** Explicit effects of the M0 metadata/contract expansion. */
typedef TypedCContractEffects = {
	final allocation:String;
	final ownership:String;
	final unsafe:String;
	final portability:String;
	final runtimeFeatures:Array<String>;
}

/** Deterministic output of the typed C contract collector. */
typedef TypedCContractSnapshot = {
	final schemaVersion:Int;
	final status:String;
	final declarations:Array<TypedCDeclaration>;
	final buildFacts:Array<TypedCBuildFact>;
	final effects:TypedCContractEffects;
}
