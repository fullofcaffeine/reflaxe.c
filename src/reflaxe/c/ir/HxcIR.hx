package reflaxe.c.ir;

/** A complete target-owned semantic program, before any C syntax is chosen. */
typedef HxcIRProgram = {
	final schemaVersion:Int;
	final dispatch:HxcIRDispatchPlan;
	final modules:Array<HxcIRModule>;
}

/** Reachability-selected virtual-dispatch facts; empty arrays mean no object header. */
typedef HxcIRDispatchPlan = {
	final layouts:Array<HxcIRVirtualTableLayout>;
	final slots:Array<HxcIRVirtualSlot>;
	final tables:Array<HxcIRVirtualTable>;
}

/** One hierarchy-wide table shape with UTF-8-ordered reachable slots. */
typedef HxcIRVirtualTableLayout = {
	final id:String;
	final rootInstanceId:String;
	final slotIds:Array<String>;
	final source:HxcSourceSpan;
}

/** One source method signature normalized for indirect C calls. */
typedef HxcIRVirtualSlot = {
	final id:String;
	final ownerInstanceId:String;
	final parameterTypes:Array<HxcIRTypeRef>;
	final returnType:HxcIRTypeRef;
	final source:HxcSourceSpan;
}

/** A null implementation is valid only when the table class cannot receive that slot. */
typedef HxcIRVirtualTableEntry = {
	final slotId:String;
	final implementationFunctionId:Null<String>;
}

/** One selected table for one reachable concrete dynamic class. */
typedef HxcIRVirtualTable = {
	final id:String;
	final layoutId:String;
	final classInstanceId:String;
	final entries:Array<HxcIRVirtualTableEntry>;
	final source:HxcSourceSpan;
}

typedef HxcIRModule = {
	final id:String;
	final types:Array<HxcIRTypeDeclaration>;
	final typeInstances:Array<HxcIRTypeInstance>;
	final globals:Array<HxcIRGlobal>;
	final functions:Array<HxcIRFunction>;
	final source:HxcSourceSpan;
}

/** Target-ABI integer identities whose widths are resolved from native facts. */
enum HxcIRAbiIntegerKind {
	IRAKSize;
	IRAKPtrDiff;
	IRAKIntPtr;
	IRAKUIntPtr;
}

/** The selected storage strategy for a source-level nullable value. */
enum HxcIRNullableRepresentation {
	IRNTagged;
	IRNPointer;
}

enum HxcIRTypeRef {
	IRTBool;
	IRTInt(width:Int, signed:Bool);
	IRTAbiInteger(kind:HxcIRAbiIntegerKind);
	IRTFloat(width:Int);

	/** Immutable valid UTF-8 with Unicode-scalar Haxe indexing semantics. */
	IRTString;

	IRTVoid;
	IRTInstance(instanceId:String);
	IRTPointer(pointee:HxcIRTypeRef, nullable:Bool);
	IRTNullable(value:HxcIRTypeRef, representation:HxcIRNullableRepresentation);
	IRTFunction(parameters:Array<HxcIRTypeRef>, result:HxcIRTypeRef);

	/** Inline storage with a compiler-proven element count and phantom identity. */
	IRTFixedArray(element:HxcIRTypeRef, length:Int, witnessId:String);

	/** Borrowed pointer-and-length view; `mutable` describes pointee access. */
	IRTSpan(element:HxcIRTypeRef, mutable:Bool);

	IRTDynamic;
}

/** Safe source indexing never becomes implicit unchecked pointer arithmetic. */
enum HxcIRBoundsPolicy {
	IRBPCheckedAbort(profile:String, buildMode:String);
	IRBPStaticProof(length:Int, index:Int);
	IRBPLoopGuarded(guardInstructionId:String, indexLocalId:String, length:Int);
}

/** Nullable class references are checked before any object storage is dereferenced. */
enum HxcIRNullCheckPolicy {
	IRNCPCheckedAbort(profile:String, buildMode:String);
}

enum HxcIRTypeKind {
	IRTKPrimitive;
	IRTKAggregate(fields:Array<HxcIRTypeField>);
	IRTKTaggedUnion(cases:Array<HxcIRTagCase>);
	IRTKClass(layout:HxcIRClassLayout);
	IRTKReference;
	IRTKFunction;
	IRTKExtern;
}

/** Metadata is absent unless a later semantic/runtime pass proves it is needed. */
enum HxcIRClassHeader {
	IRCHNone;
	IRCHVirtual(layoutId:String);
	IRCHRuntime(featureId:String);
}

/** A concrete class instance owns one optional base prefix plus source-order fields. */
typedef HxcIRClassLayout = {
	final baseInstanceId:Null<String>;
	final fields:Array<HxcIRTypeField>;
	final header:HxcIRClassHeader;
}

typedef HxcIRTypeField = {
	final name:String;
	final type:HxcIRTypeRef;
	final mutable:Bool;
	final source:HxcSourceSpan;
}

typedef HxcIRTagPayload = {
	final name:String;
	final type:HxcIRTypeRef;
	final source:HxcSourceSpan;
}

typedef HxcIRTagCase = {
	final name:String;
	final tagValue:Int;
	final payload:Array<HxcIRTagPayload>;
	final source:HxcSourceSpan;
}

typedef HxcIRTypeDeclaration = {
	final id:String;
	final displayName:String;
	final kind:HxcIRTypeKind;
	final source:HxcSourceSpan;
}

enum HxcIRRepresentation {
	IRRDirect;
	IRRTagged;
	IRROpaqueHandle;
	IRRManaged(runtimeFeature:String);
}

typedef HxcIRTypeInstance = {
	final id:String;
	final declarationId:String;
	final arguments:Array<HxcIRTypeRef>;
	final representation:HxcIRRepresentation;
	final source:HxcSourceSpan;
}

enum HxcIRConstant {
	IRCInt(value:String);
	IRCFloat(value:String);
	IRCBool(value:Bool);

	/** Source text plus its independently validated UTF-8 byte length. */
	IRCString(value:String, byteLength:Int);

	IRCNull;
}

enum HxcIRGlobalInitialization {
	IRGIUninitialized;
	IRGIConstant(value:HxcIRConstant);
	IRGIDeferred(initializerFunctionId:String);
}

typedef HxcIRGlobal = {
	final id:String;
	final type:HxcIRTypeRef;
	final mutable:Bool;
	final initialization:HxcIRGlobalInitialization;
	final source:HxcSourceSpan;
}

enum HxcIRInitializationState {
	IRISUninitialized;
	IRISInitializing;
	IRISInitialized;
	IRISMoved;
	IRISDestroyed;
}

enum HxcIRLocalStorage {
	IRLSAutomatic;
	IRLSStatic;
	IRLSFrame;
	IRLSRegion(regionId:String);
}

typedef HxcIRParameter = {
	final id:String;
	final type:HxcIRTypeRef;
	final source:HxcSourceSpan;
}

typedef HxcIRLocal = {
	final id:String;
	final type:HxcIRTypeRef;
	final storage:HxcIRLocalStorage;
	final initialState:HxcIRInitializationState;
	final source:HxcSourceSpan;
}

/** Mutable addressable storage is always distinct from an immutable value ID. */
enum HxcIRPlace {
	IRPLocal(localId:String);
	IRPGlobal(globalId:String);
	IRPDereference(pointerValueId:String);
	IRPField(base:HxcIRPlace, fieldName:String);
	IRPIndex(base:HxcIRPlace, indexValueId:String);
}

enum HxcIRImplementation {
	IRIStatic;
	IRIProgramLocal(helperId:String);
	IRIRuntime(featureId:String);
}

/** A payload projection remains checked before C syntax is selected. */
enum HxcIRTagCheckPolicy {
	IRTCPCheckedAbort(profile:String, buildMode:String);
}

enum HxcIRConversionKind {
	IRCNumericExact;
	IRCNumericWrapping;
	IRCNumericSaturating;
	IRCNumericChecked;
	IRCNullableInject;
	IRCNullableUnwrap;
	IRCPointer;
	IRCBox;
	IRCUnbox;
	IRCRepresentation;
}

enum HxcIRAllocationIntent {
	IRAStack;
	IRAOwned;
	IRAShared;
	IRAArena(arenaId:String);
}

enum HxcIRFailureKind {
	IRFException;
	IRFResultError;
	IRFAllocationFailure;
	IRFNativeStatus;
}

/** How a generated C function reports a semantic failure to its caller. */
enum HxcIRFunctionFailureConvention {
	IRFCInfallible;
	IRFCStatus(kind:HxcIRFailureKind);
}

enum HxcIRFailureTarget {
	IRFTBlock(blockId:String);
	IRFTPropagate;
	IRFTAbort;
}

typedef HxcIRCleanupStep = {
	final regionId:String;
	final actionId:String;
}

typedef HxcIRFailureEdge = {
	final kind:HxcIRFailureKind;
	final target:HxcIRFailureTarget;
	final arguments:Array<String>;
	final cleanup:Array<HxcIRCleanupStep>;
}

enum HxcIRCallDispatch {
	IRCDDirect(functionId:String);
	IRCDVirtual(slotId:String, receiverValueId:String);
	IRCDInterface(interfaceTypeId:String, slotId:String, receiverValueId:String);
	IRCDClosure(callableValueId:String);
	IRCDNative(symbol:String);
	IRCDRuntime(featureId:String, operationId:String);
	IRCDIntrinsic(intrinsicId:String);
}

typedef HxcIRCall = {
	final dispatch:HxcIRCallDispatch;
	final arguments:Array<String>;
	final returnType:HxcIRTypeRef;
	final failure:Null<HxcIRFailureEdge>;
}

typedef HxcIRNamedValue = {
	final name:String;
	final valueId:String;
}

typedef HxcIRResult = {
	final id:String;
	final type:HxcIRTypeRef;
}

/**
	Instruction array order is semantic evaluation order. No emitter may compact
	two side-effecting instructions into a C expression with weaker ordering.
 */
enum HxcIRInstructionKind {
	IRIOSequence(label:String);
	IRIOConstant(value:HxcIRConstant);
	IRIOLoad(place:HxcIRPlace);
	IRIOStore(place:HxcIRPlace, valueId:String);
	IRIOAddress(place:HxcIRPlace);
	IRIOUnary(operationId:String, valueId:String, implementation:HxcIRImplementation);
	IRIOBinary(operationId:String, leftValueId:String, rightValueId:String, implementation:HxcIRImplementation);
	IRIOConvert(valueId:String, kind:HxcIRConversionKind, targetType:HxcIRTypeRef, implementation:HxcIRImplementation, failure:Null<HxcIRFailureEdge>);
	IRIOCall(call:HxcIRCall);
	IRIOConstructAggregate(instanceId:String, fields:Array<HxcIRNamedValue>);
	IRIOProject(valueId:String, fieldName:String);
	IRIOConstructTag(instanceId:String, tagName:String, payload:Array<String>);
	IRIOMatchTag(valueId:String, tagName:String);
	IRIOProjectTag(valueId:String, tagName:String, payloadIndex:Int, check:HxcIRTagCheckPolicy);
	IRIOAllocate(type:HxcIRTypeRef, intent:HxcIRAllocationIntent, implementation:HxcIRImplementation, failure:Null<HxcIRFailureEdge>);
	IRIODeallocate(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRIORetain(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRIOTrace(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRIODefaultInitialize(place:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitialize(place:HxcIRPlace, valueId:String, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitializeFixedArray(place:HxcIRPlace, values:Array<String>, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitializeSpan(place:HxcIRPlace, sourceArray:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOBindVirtualTable(place:HxcIRPlace, tableId:String);
	IRIOBoundsCheck(collection:HxcIRPlace, indexValueId:String, policy:HxcIRBoundsPolicy);
	IRIONullCheck(valueId:String, policy:HxcIRNullCheckPolicy);
	IRIOLifetime(place:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState, reason:String);
}

typedef HxcIRInstruction = {
	final id:String;
	final result:Null<HxcIRResult>;
	final kind:HxcIRInstructionKind;
	final source:HxcSourceSpan;
}

enum HxcIRCleanupIdempotence {
	IRCExactlyOnce;
	IRCIdempotent;
}

enum HxcIRCleanupActionKind {
	IRCADestroy(place:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRCARelease(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRCADeallocate(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRCAFinally(blockId:String);
}

typedef HxcIRCleanupAction = {
	final id:String;
	final idempotence:HxcIRCleanupIdempotence;
	final kind:HxcIRCleanupActionKind;
	final source:HxcSourceSpan;
}

/** Actions are registered in array order and execute in reverse on an edge. */
typedef HxcIRCleanupRegion = {
	final id:String;
	final parentId:Null<String>;
	final actions:Array<HxcIRCleanupAction>;
	final source:HxcSourceSpan;
}

typedef HxcIRBlockEdge = {
	final targetBlockId:String;
	final arguments:Array<String>;
	final cleanup:Array<HxcIRCleanupStep>;
}

typedef HxcIRSwitchCase = {
	final value:HxcIRConstant;
	final edge:HxcIRBlockEdge;
}

typedef HxcIRTagSwitchCase = {
	final tagName:String;
	final edge:HxcIRBlockEdge;
}

enum HxcIRTerminatorKind {
	IRTJump(edge:HxcIRBlockEdge);
	IRTBranch(conditionValueId:String, whenTrue:HxcIRBlockEdge, whenFalse:HxcIRBlockEdge);
	IRTSwitch(valueId:String, cases:Array<HxcIRSwitchCase>, defaultEdge:HxcIRBlockEdge);
	IRTTagSwitch(valueId:String, cases:Array<HxcIRTagSwitchCase>, defaultEdge:Null<HxcIRBlockEdge>);
	IRTReturn(valueId:Null<String>, cleanup:Array<HxcIRCleanupStep>);
	IRTThrow(valueId:String, edge:HxcIRFailureEdge);
	IRTUnreachable;
}

typedef HxcIRTerminator = {
	final kind:HxcIRTerminatorKind;
	final source:HxcSourceSpan;
}

typedef HxcIRBlock = {
	final id:String;
	final parameters:Array<HxcIRParameter>;
	final instructions:Array<HxcIRInstruction>;
	final terminator:Null<HxcIRTerminator>;
	final source:HxcSourceSpan;
}

typedef HxcIRFunction = {
	final id:String;
	final displayName:String;
	final parameters:Array<HxcIRParameter>;
	final locals:Array<HxcIRLocal>;
	final returnType:HxcIRTypeRef;
	final failureConvention:HxcIRFunctionFailureConvention;
	final entryBlockId:String;
	final blocks:Array<HxcIRBlock>;
	final cleanupRegions:Array<HxcIRCleanupRegion>;
	final source:HxcSourceSpan;
}
