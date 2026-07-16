package reflaxe.c.plan;

enum abstract CStaticInitializationStrategy(String) to String {
	var EagerHaxeTypeOrder = "eager-haxe-type-order";
}

enum abstract CStaticInitializationPhase(String) to String {
	var ClassInitializers = "class-initializers";
	var StaticFieldInitializers = "static-field-initializers";
	var Entry = "entry";
}

enum abstract CStaticInitializationDependencyReason(String) to String {
	var Inheritance = "inheritance";
	var Interface = "interface";
	var TypeReference = "type-reference";
	var StaticFieldReference = "static-field-reference";
	var StaticFunctionReference = "static-function-reference";
	var ConstructorReference = "constructor-reference";
}

enum abstract CStaticInitializerKind(String) to String {
	var ClassInitializer = "class-initializer";
	var StaticFieldInitializer = "static-field-initializer";
}

enum abstract CStaticInitializationCyclePolicy(String) to String {
	var RejectCrossTypeCycles = "reject-cross-type-cycles-with-canonical-path";
}

enum abstract CStaticInitializationOncePolicy(String) to String {
	var HostedExecutableEntry = "one compiler-owned initialization call from the hosted executable entry when the plan is non-empty";
}

/** Stable source coordinates used by the initialization-plan sidecar. */
typedef CStaticInitializationSource = {
	final file:String;
	final startLine:Int;
	final startColumn:Int;
	final endLine:Int;
	final endColumn:Int;
}

/** One reason a type must be ordered after another type. */
typedef CStaticInitializationDependency = {
	final dependentTypeId:String;
	final prerequisiteTypeId:String;
	final reason:CStaticInitializationDependencyReason;
	final source:CStaticInitializationSource;
}

/** One compiler-owned initializer in the eager executable phase. */
typedef CStaticInitializerRecord = {
	final id:String;
	final functionId:String;
	final modulePath:String;
	final typeId:String;
	final kind:CStaticInitializerKind;
	final fieldName:Null<String>;
	final sourceOrder:Int;
	final source:CStaticInitializationSource;
}

/** Inspectable, deterministic contract for one request-local initialization plan. */
typedef CStaticInitializationSnapshot = {
	final schemaVersion:Int;
	final strategy:CStaticInitializationStrategy;
	final phaseOrder:Array<CStaticInitializationPhase>;
	final entryFunctionId:String;
	final typeOrder:Array<String>;
	final dependencies:Array<CStaticInitializationDependency>;
	final initializers:Array<CStaticInitializerRecord>;
	final executionOrder:Array<String>;
	final cyclePolicy:CStaticInitializationCyclePolicy;
	final oncePolicy:CStaticInitializationOncePolicy;
	final runtimeFeatures:Array<String>;
}
