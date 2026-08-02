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

/**
	One closed-world table mapping for an infallible interface-parent upcast.

	Both tables belong to the same concrete class. The source table identifies
	which implementation an interface value currently carries; the target table
	preserves that implementation while changing only the static interface view.
**/
typedef HxcIRInterfaceUpcastTable = {
	final sourceTableId:String;
	final targetTableId:String;
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

	/**
		An immutable UTF-8 Haxe String whose value may keep runtime storage alive.

		It uses the same C `hxc_string` carrier as `IRTString`, but unlike the
		literal-only form it participates in retain/release planning. Keeping the
		distinction in semantic IR prevents a runtime-created value from being
		silently copied under the immortal-literal lifetime rule.
	**/
	IRTManagedString;

	/** Borrowed NUL-terminated bytes backed by stable C string-literal storage. */
	IRTCString;

	/**
		An immutable `const char *` borrowed from one live Haxe String owner.

		The checked `string/borrow-cstring` operation is its only producer. The
		value may be consumed exactly once by a direct native call in the same
		basic block and cannot enter storage or control flow.
	**/
	IRTCallScopedCString;

	/**
		A mutable `char *` borrowed from one live managed Bytes owner.

		This is not a general pointer. Its only legal producer is the checked
		`bytes/borrow-mutable-cstring` runtime operation, and its only legal use is
		one direct imported-C call argument in the same basic block.
	**/
	IRTMutableCStringBuffer;

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

	/**
		A typed function value paired with caller-owned captured storage.

		`parameters` and `result` describe the source-visible call. The concrete
		aggregate also carries an exact adapter whose first argument is an opaque
		context pointer. Keeping the call signature here lets validation prove a
		closure call before C selects the familiar `{ invoke, context }` struct.
	**/
	IRRStackClosure(parameters:Array<HxcIRTypeRef>, result:HxcIRTypeRef);
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

	/** A NUL-free source literal borrowed as a native `const char *`. */
	IRCCStringLiteral(value:String, byteLength:Int);

	/** A header-owned C enum or object-like macro with a typed result. */
	IRCNativeConstant(constantId:String);

	/**
		An absent value whose result type names its exact nullable carrier.

		This includes nullable pointers, tagged direct optionals, and `IRTString`.
		String uses a struct in C, but its null data pointer is the reference-null
		discriminant; a non-null empty String therefore remains distinguishable.
	**/
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

	/** Explicit binary64 -> binary32 round-to-nearest, ties-to-even. */
	IRCNumericRoundBinary32;

	/** Exact binary32 -> binary64 widening, preserving signed zero and class. */
	IRCNumericWidenBinary64;

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

	/**
		The direct target's declaration that owns a returned read-only span.

		This contract is present only on a direct call to a function carrying the
		same declaration. Validation resolves the named target parameter to the
		corresponding live caller argument before C syntax is selected.
	**/
	final ?borrowedSpanReturn:HxcIRBorrowedSpanReturn;
}

/**
	The one admitted origin for a span that crosses a direct function boundary.

	`IRBSRReceiverField` means that the result is a read-only view of fixed-array
	storage embedded in the named receiver parameter. The receiver continues to
	own those bytes; the result may only be consumed during the caller's bounded
	scope. Mutable returns and borrows from locals are deliberately not variants,
	so they fail closed instead of acquiring an accidental C lifetime.
**/
enum HxcIRBorrowedSpanReturn {
	IRBSRReceiverField(receiverParameterId:String);
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
	How one managed carrier acquires an owner before transferring it.

	A fresh result already owns its active payload and can move that owner. A
	borrowed value must retain one independent copy before its original owner may
	leave scope. Branch joins use this distinction to select one owner, while a
	straight-line handoff uses it to make a retained borrow visibly transferable.
	The distinction stays in semantic IR so C emission never guesses ownership
	from syntax.
**/
enum HxcIRManagedCarrierAcquisition {
	IRMCAMoveFresh;
	IRMCARetainBorrowed(implementation:HxcIRImplementation);
}

/**
	Instruction array order is semantic evaluation order. No emitter may compact
	two side-effecting instructions into a C expression with weaker ordering.
 */
enum HxcIRInstructionKind {
	IRIOSequence(label:String);
	IRIOConstant(value:HxcIRConstant);

	/** Materialize one reachable, signature-compatible non-capturing function. */
	IRIOFunctionReference(functionId:String);

	IRIOLoad(place:HxcIRPlace);
	IRIOStore(place:HxcIRPlace, valueId:String);
	IRIOAddress(place:HxcIRPlace);

	/** Name an embedded class subobject without acquiring its parent's lifetime. */
	IRIOBorrowClassField(place:HxcIRPlace);

	IRIOUnary(operationId:String, valueId:String, implementation:HxcIRImplementation);
	IRIOBinary(operationId:String, leftValueId:String, rightValueId:String, implementation:HxcIRImplementation);
	IRIOConvert(valueId:String, kind:HxcIRConversionKind, targetType:HxcIRTypeRef, implementation:HxcIRImplementation, failure:Null<HxcIRFailureEdge>);
	IRIOCall(call:HxcIRCall);
	IRIOConstructAggregate(instanceId:String, fields:Array<HxcIRNamedValue>);

	/**
		Construct one direct aggregate with C's complete zero-initialization rule.

		The typed surface currently admits only header-owned structs. Keeping the
		operation explicit lets validation prove that no managed owner is
		fabricated and lets C emission use structural `(struct T){0}` syntax.
	**/
	IRIOZeroAggregate(instanceId:String);

	/**
		Pair a concrete Haxe object reference with the interface table selected for
		its runtime class. Keeping this as semantic IR lets validation reject a
		wrong object/table/interface combination before C chooses a two-pointer
		struct spelling.
	**/
	IRIOConstructInterface(interfaceInstanceId:String, objectValueId:String, tableId:String);

	/**
		Change a child-interface value into one declared parent interface.

		The object pointer is preserved. `tables` is the complete closed-world
		mapping from every reachable source table to the corresponding target table
		for the same concrete class, so C emission never guesses or uses an
		unchecked struct-pointer cast.
	**/
	IRIOUpcastInterface(valueId:String, sourceInterfaceInstanceId:String, targetInterfaceInstanceId:String, tables:Array<HxcIRInterfaceUpcastTable>);

	IRIOProject(valueId:String, fieldName:String);
	IRIOConstructTag(instanceId:String, tagName:String, payload:Array<String>);
	IRIOMatchTag(valueId:String, tagName:String);
	IRIOProjectTag(valueId:String, tagName:String, payloadIndex:Int, check:HxcIRTagCheckPolicy);
	IRIOAllocate(type:HxcIRTypeRef, intent:HxcIRAllocationIntent, implementation:HxcIRImplementation, failure:Null<HxcIRFailureEdge>);
	IRIODeallocate(place:HxcIRPlace, implementation:HxcIRImplementation);
	IRIORetain(place:HxcIRPlace, implementation:HxcIRImplementation);

	/** Release one live managed place at its exact lexical or control-flow boundary. */
	IRIORelease(place:HxcIRPlace, implementation:HxcIRImplementation);

	IRIOTrace(place:HxcIRPlace, implementation:HxcIRImplementation);

	/**
		Declare storage without fabricating a source-language value.

		Validation admits this only for unmanaged direct values whose structured
		control flow assigns every path before the first load. It lets a
		conditional join become ordinary C such as `Point selected; if (...)`.
	**/
	IRIODeclareUninitialized(place:HxcIRPlace);

	/**
		Declare storage for one explicitly acquired managed owner.

		The destroy plan identifies the matching String/Array runtime lifecycle or
		the exact managed tagged-enum family. In a control-flow join, every path
		that reaches the move acquires the carrier once. A straight-line retain may
		acquire and move immediately. Both forms transfer exactly one owner and
		reject duplicate acquisition, duplicate movement, or an owned exit.
	**/
	IRIODeclareManagedCarrier(place:HxcIRPlace, destroyImplementation:HxcIRImplementation);

	/** Move or retain one selected branch value into a declared managed carrier. */
	IRIOAcquireManagedCarrier(place:HxcIRPlace, valueId:String, acquisition:HxcIRManagedCarrierAcquisition);

	/** Consume a managed carrier's one owner and produce the joined value. */
	IRIOMoveManagedCarrier(place:HxcIRPlace);

	IRIODefaultInitialize(place:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitialize(place:HxcIRPlace, valueId:String, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitializeFixedArray(place:HxcIRPlace, values:Array<String>, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOZeroInitializeFixedArray(place:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);
	IRIOInitializeSpan(place:HxcIRPlace, sourceArray:HxcIRPlace, from:HxcIRInitializationState, to:HxcIRInitializationState);

	/**
		Borrow a pointer and fixed length from inline array storage without copying.

		Unlike `IRIOInitializeSpan`, this operation produces a semantic span value
		that may be returned under an explicit receiver-owned return contract.
	**/
	IRIOBorrowSpan(sourceArray:HxcIRPlace);

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

/** One typed step from an inline HxcIR value to an embedded managed reference. */
enum HxcIRManagedRootProjection {
	/** Select one field from a direct closed-record instance. */
	IRMRPAggregateField(instanceId:String, fieldName:String);

	/** Select one payload field, but only while the tagged enum has this case. */
	IRMRPTagPayload(instanceId:String, tagName:String, payloadIndex:Int);

	/** Select a tagged optional's payload, but only while it is present. */
	IRMRPNullablePayload;
}

/**
	One exact managed reference kept alive by a generated function root frame.

	The root starts at an immutable HxcIR value. An empty projection list means
	the value itself is one collector-managed pointer. A non-empty list walks a
	closed record, active enum payload, or present optional until it reaches that
	pointer. The C emitter publishes the resulting pointer—or null when a tag or
	presence guard does not match—after the value is defined.

	Keeping the path in semantic IR is important: validation can prove that every
	step names a real field and ends at an exact managed base pointer before C
	syntax is chosen. It also lets by-value records and enums remain readable C
	structs instead of boxing them merely to satisfy the garbage collector.
**/
typedef HxcIRManagedRoot = {
	final id:String;
	final valueId:String;
	final projections:Array<HxcIRManagedRootProjection>;
	final source:HxcSourceSpan;
}

typedef HxcIRFunction = {
	final id:String;
	final displayName:String;
	final parameters:Array<HxcIRParameter>;

	/** Class-reference parameters whose storage remains owned by the caller. */
	final borrowedClassParameterIds:Array<String>;

	/**
		Interface-value parameters whose referenced object remains caller-owned.

		The interface value itself is copied by value, but its object pointer is
		only valid for the call. This optional field is an additive compatibility
		bridge for older hand-built HxcIR fixtures; compiler-produced functions
		always supply it, including an empty list.
	**/
	final ?borrowedInterfaceParameterIds:Array<String>;

	/** Automatic pointer locals that only rename caller- or parent-owned class storage. */
	final borrowedClassLocalIds:Array<String>;

	/**
		Automatic interface-value locals whose object pointer remains caller-owned.

		The interface pair itself is copied into the local, but that copy does not
		extend the lifetime of the concrete object behind it. This optional field
		keeps older hand-built HxcIR fixtures compatible; compiler-produced
		functions always supply it, including an empty list.
	**/
	final ?borrowedInterfaceLocalIds:Array<String>;

	/**
		Exact managed parameters and instruction results kept alive until return.

		This field is optional only so older hand-built test/program adapters fail
		closed through validation without requiring an untyped migration shim. New
		compiler-produced HxcIR always supplies it, including an empty list.
	**/
	final ?managedRoots:Array<HxcIRManagedRoot>;

	final locals:Array<HxcIRLocal>;
	final returnType:HxcIRTypeRef;

	/**
		How this function may lend a read-only span across its return boundary.

		The optional field preserves compatibility with older hand-built HxcIR
		fixtures. Compiler-produced schema-23 functions always supply either the
		closed receiver-field contract or `null`.
	**/
	final ?borrowedSpanReturn:HxcIRBorrowedSpanReturn;

	final failureConvention:HxcIRFunctionFailureConvention;
	final entryBlockId:String;
	final blocks:Array<HxcIRBlock>;
	final cleanupRegions:Array<HxcIRCleanupRegion>;
	final source:HxcSourceSpan;
}
