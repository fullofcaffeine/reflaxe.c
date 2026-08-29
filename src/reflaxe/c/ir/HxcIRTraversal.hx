package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator.ValidatedHxcIRProgram;

/**
	Walks validated HxcIR without guessing which enum constructors own children.

	The traversal follows structural containment in authored array order. String
	IDs are semantic references, so it reports their owning typed node but never
	follows them into another declaration, function, block, value, or cleanup.
	That rule keeps the walk finite and prevents reference resolution from
	reordering the canonical program.

	Each closed enum switch names every constructor and has no catch-all. Adding a
	constructor therefore makes this module fail to compile until its child policy
	is explicit. Required typedef fields already make fixture construction fail;
	the HxcIR admission checklist separately forbids adding an optional child
	without a traversal sentinel.
**/
/** Independent admission sentinel; a schema bump must review traversal policy. */
final HXC_IR_TRAVERSAL_SCHEMA_VERSION:Int = 27;

/** The nearest source owner for one visited semantic node. */
class HxcIRTraversalSite {
	public final moduleId:Null<String>;
	public final functionId:Null<String>;
	public final blockId:Null<String>;
	public final source:HxcSourceSpan;

	public function new(moduleId:Null<String>, functionId:Null<String>, blockId:Null<String>, source:HxcSourceSpan) {
		this.moduleId = moduleId;
		this.functionId = functionId;
		this.blockId = blockId;
		this.source = source;
	}

	/** Return the same ownership context with a more precise source span. */
	public function at(source:HxcSourceSpan):HxcIRTraversalSite
		return new HxcIRTraversalSite(moduleId, functionId, blockId, source);
}

/**
	Selective typed observations emitted by the authoritative structural walk.

	Subclasses override only the semantic families they analyze. The walker, not
	the visitor, owns recursion, so an ignored parent cannot hide a nested type,
	place, implementation, failure edge, cleanup, or terminator.
**/
class HxcIRTraversalVisitor {
	public function new() {}

	/** Observe the complete validated owner before its children. */
	public function onProgram(program:ValidatedHxcIRProgram):Void {}

	/** Observe one source module before its declarations and functions. */
	public function onModule(module:HxcIRModule, site:HxcIRTraversalSite):Void {}

	/** Observe one declared type before its kind-owned children. */
	public function onTypeDeclaration(declaration:HxcIRTypeDeclaration, site:HxcIRTraversalSite):Void {}

	/** Observe one type reference before nested pointee, payload, or signature types. */
	public function onTypeRef(type:HxcIRTypeRef, site:HxcIRTraversalSite):Void {}

	/** Observe one function before parameters, locals, roots, blocks, and cleanup. */
	public function onFunction(fn:HxcIRFunction, site:HxcIRTraversalSite):Void {}

	/** Observe one basic block before its parameters, instructions, and terminator. */
	public function onBlock(block:HxcIRBlock, site:HxcIRTraversalSite):Void {}

	/** Observe one instruction before its result and kind-owned children. */
	public function onInstruction(instruction:HxcIRInstruction, site:HxcIRTraversalSite):Void {}

	/** Observe one recursively addressable place before its base place. */
	public function onPlace(place:HxcIRPlace, site:HxcIRTraversalSite):Void {}

	/** Observe one implementation selection at the source operation that owns it. */
	public function onImplementation(implementation:HxcIRImplementation, site:HxcIRTraversalSite):Void {}

	/** Observe one failure edge before its target and cleanup path. */
	public function onFailureEdge(edge:HxcIRFailureEdge, site:HxcIRTraversalSite):Void {}

	/** Observe one ordered cleanup reference without resolving its target action. */
	public function onCleanupStep(step:HxcIRCleanupStep, site:HxcIRTraversalSite):Void {}

	/** Observe one control-flow edge before its ordered cleanup references. */
	public function onBlockEdge(edge:HxcIRBlockEdge, site:HxcIRTraversalSite):Void {}

	/** Observe one registered cleanup action before its place and implementation. */
	public function onCleanupAction(action:HxcIRCleanupAction, site:HxcIRTraversalSite):Void {}

	/** Observe one terminator before its structural edges and failures. */
	public function onTerminator(terminator:HxcIRTerminator, site:HxcIRTraversalSite):Void {}

	/** Observe one bounds policy selected by an instruction. */
	public function onBoundsPolicy(policy:HxcIRBoundsPolicy, site:HxcIRTraversalSite):Void {}

	/** Observe one null-check policy selected by an instruction. */
	public function onNullCheckPolicy(policy:HxcIRNullCheckPolicy, site:HxcIRTraversalSite):Void {}

	/** Observe one tag-check policy selected by a projection instruction. */
	public function onTagCheckPolicy(policy:HxcIRTagCheckPolicy, site:HxcIRTraversalSite):Void {}

	/** Observe one exact managed root and its projection path. */
	public function onManagedRoot(root:HxcIRManagedRoot, site:HxcIRTraversalSite):Void {}

	/** Observe one typed managed-root projection without resolving instance IDs. */
	public function onManagedRootProjection(projection:HxcIRManagedRootProjection, site:HxcIRTraversalSite):Void {}
}

/** Visit every structural child of one validated program in authored order. */
function walkHxcIR(program:ValidatedHxcIRProgram, visitor:HxcIRTraversalVisitor):Void {
	visitor.onProgram(program);
	final source = program.modules.length == 0 ? new HxcSourceSpan("hxcir/program", 1, 1, 1, 1) : program.modules[0].source;
	final root = new HxcIRTraversalSite(null, null, null, source);
	walkDynamicPlan(program.dynamicPlan, root, visitor);
	walkDispatchPlan(program.dispatch, root, visitor);
	for (module in program.modules)
		walkModule(module, visitor);
}

function walkDynamicPlan(plan:HxcIRDynamicPlan, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	for (type in plan.types) {
		final childSite = site.at(type.source);
		if (type.sourceType != null)
			walkTypeRef(type.sourceType, childSite, visitor);
		walkDynamicCategory(type.category);
		walkDynamicStorage(type.storage);
	}
	for (member in plan.members)
		walkDynamicMemberKind(member.kind);
	for (operation in plan.operations)
		walkDynamicOperationKind(operation.kind);
	// Call shapes contain semantic type IDs, not structural HxcIR type children.
}

function walkDynamicCategory(category:HxcIRDynamicCategory):Void {
	switch category {
		case IRDCNull | IRDCBool | IRDCInt | IRDCFloat | IRDCString | IRDCArray | IRDCObject | IRDCEnum | IRDCFunction | IRDCTypeValue:
	}
}

function walkDynamicStorage(storage:HxcIRDynamicStorage):Void {
	switch storage {
		case IRDSInlineNull | IRDSInlineBool | IRDSInlineInt32 | IRDSInlineFloat64 | IRDSManagedReference | IRDSManagedWrapper | IRDSStaticToken:
	}
}

function walkDynamicMemberKind(kind:HxcIRDynamicMemberKind):Void {
	switch kind {
		case IRDMField(_, _) | IRDMMethod(_):
	}
}

function walkDynamicOperationKind(kind:HxcIRDynamicOperationKind):Void {
	switch kind {
		case IRDOKBox(_) | IRDOKUnbox(_) | IRDOKGet(_) | IRDOKSet(_) | IRDOKCall(_, _) | IRDOKInvoke(_, _) | IRDOKEqual(_, _):
	}
}

function walkDispatchPlan(plan:HxcIRDispatchPlan, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	for (layout in plan.layouts) {
		// Layout IDs and slot IDs are semantic references.
	}
	for (slot in plan.slots) {
		final childSite = site.at(slot.source);
		for (type in slot.parameterTypes)
			walkTypeRef(type, childSite, visitor);
		walkTypeRef(slot.returnType, childSite, visitor);
	}
	for (table in plan.tables)
		for (entry in table.entries) {
			// Table entries contain semantic slot/function references only.
		}
}

function walkModule(module:HxcIRModule, visitor:HxcIRTraversalVisitor):Void {
	final site = new HxcIRTraversalSite(module.id, null, null, module.source);
	visitor.onModule(module, site);
	for (declaration in module.types)
		walkTypeDeclaration(declaration, site.at(declaration.source), visitor);
	for (instance in module.typeInstances) {
		final childSite = site.at(instance.source);
		for (argument in instance.arguments)
			walkTypeRef(argument, childSite, visitor);
		walkRepresentation(instance.representation, childSite, visitor);
	}
	for (global in module.globals) {
		final childSite = site.at(global.source);
		walkTypeRef(global.type, childSite, visitor);
		walkGlobalInitialization(global.initialization);
	}
	for (fn in module.functions)
		walkFunction(fn, site, visitor);
}

function walkTypeDeclaration(declaration:HxcIRTypeDeclaration, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onTypeDeclaration(declaration, site);
	switch declaration.kind {
		case IRTKPrimitive | IRTKReference | IRTKFunction | IRTKExtern:
		case IRTKAggregate(fields):
			for (field in fields)
				walkTypeRef(field.type, site.at(field.source), visitor);
		case IRTKTaggedUnion(cases):
			for (tagCase in cases)
				for (payload in tagCase.payload)
					walkTypeRef(payload.type, site.at(payload.source), visitor);
		case IRTKClass(layout):
			walkClassHeader(layout.header);
			for (field in layout.fields)
				walkTypeRef(field.type, site.at(field.source), visitor);
	}
}

function walkClassHeader(header:HxcIRClassHeader):Void {
	switch header {
		case IRCHNone | IRCHVirtual(_) | IRCHRuntime(_):
	}
}

function walkRepresentation(representation:HxcIRRepresentation, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	switch representation {
		case IRRDirect | IRRTagged | IRROpaqueHandle | IRRManaged(_):
		case IRRStackClosure(parameters, result):
			for (parameter in parameters)
				walkTypeRef(parameter, site, visitor);
			walkTypeRef(result, site, visitor);
	}
}

function walkGlobalInitialization(initialization:HxcIRGlobalInitialization):Void {
	switch initialization {
		case IRGIUninitialized | IRGIDeferred(_):
		case IRGIConstant(value):
			walkConstant(value);
	}
}

function walkConstant(value:HxcIRConstant):Void {
	switch value {
		case IRCInt(_) | IRCFloat(_) | IRCBool(_) | IRCString(_, _) | IRCCStringLiteral(_, _) | IRCNativeConstant(_) | IRCNull:
	}
}

function walkFunction(fn:HxcIRFunction, moduleSite:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	final site = new HxcIRTraversalSite(moduleSite.moduleId, fn.id, null, fn.source);
	visitor.onFunction(fn, site);
	for (parameter in fn.parameters)
		walkTypeRef(parameter.type, site.at(parameter.source), visitor);
	for (local in fn.locals) {
		walkTypeRef(local.type, site.at(local.source), visitor);
		walkLocalStorage(local.storage);
		walkInitializationState(local.initialState);
	}
	walkTypeRef(fn.returnType, site, visitor);
	if (fn.borrowedSpanReturn != null)
		walkBorrowedSpanReturn(fn.borrowedSpanReturn);
	walkFunctionFailureConvention(fn.failureConvention);
	if (fn.managedRoots != null)
		for (root in fn.managedRoots)
			walkManagedRoot(root, site.at(root.source), visitor);
	if (fn.exceptionStrategy != null)
		walkExceptionStrategy(fn.exceptionStrategy);
	if (fn.exceptionCleanups != null)
		for (cleanup in fn.exceptionCleanups) {
			final childSite = site.at(cleanup.source);
			walkPlace(cleanup.place, childSite, visitor);
			walkImplementation(cleanup.implementation, childSite, visitor);
		}
	// Exception regions contain storage/value references but no structural child.
	for (block in fn.blocks)
		walkBlock(block, site, visitor);
	for (region in fn.cleanupRegions)
		for (action in region.actions)
			walkCleanupAction(action, site.at(action.source), visitor);
}

function walkLocalStorage(storage:HxcIRLocalStorage):Void {
	switch storage {
		case IRLSAutomatic | IRLSStatic | IRLSFrame | IRLSRegion(_):
	}
}

function walkInitializationState(state:HxcIRInitializationState):Void {
	switch state {
		case IRISUninitialized | IRISInitializing | IRISInitialized | IRISMoved | IRISDestroyed:
	}
}

function walkBorrowedSpanReturn(value:HxcIRBorrowedSpanReturn):Void {
	switch value {
		case IRBSRReceiverField(_):
	}
}

function walkFunctionFailureConvention(value:HxcIRFunctionFailureConvention):Void {
	switch value {
		case IRFCInfallible:
		case IRFCStatus(kind):
			walkFailureKind(kind);
	}
}

function walkFailureKind(kind:HxcIRFailureKind):Void {
	switch kind {
		case IRFException | IRFResultError | IRFAllocationFailure | IRFNativeStatus:
	}
}

function walkExceptionStrategy(strategy:HxcIRExceptionStrategy):Void {
	switch strategy {
		case IRESClosedResult | IRESContainedRuntime:
	}
}

function walkManagedRoot(root:HxcIRManagedRoot, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onManagedRoot(root, site);
	for (projection in root.projections) {
		visitor.onManagedRootProjection(projection, site);
		switch projection {
			case IRMRPAggregateField(_, _) | IRMRPTagPayload(_, _, _) | IRMRPNullablePayload | IRMRPDynamicPayload:
		}
	}
}

function walkBlock(block:HxcIRBlock, functionSite:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	final site = new HxcIRTraversalSite(functionSite.moduleId, functionSite.functionId, block.id, block.source);
	visitor.onBlock(block, site);
	for (parameter in block.parameters)
		walkTypeRef(parameter.type, site.at(parameter.source), visitor);
	for (instruction in block.instructions)
		walkInstruction(instruction, site.at(instruction.source), visitor);
	if (block.terminator != null)
		walkTerminator(block.terminator, site.at(block.terminator.source), visitor);
}

function walkInstruction(instruction:HxcIRInstruction, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onInstruction(instruction, site);
	if (instruction.result != null)
		walkTypeRef(instruction.result.type, site, visitor);
	final failure = instructionFailureEdge(instruction.kind);
	if (failure != null)
		walkFailureEdge(failure, site, visitor);
	switch instruction.kind {
		case IRIOSequence(_):
		case IRIOConstant(value):
			walkConstant(value);
		case IRIOFunctionReference(_):
		case IRIOLoad(place) | IRIOAddress(place) | IRIOBorrowClassField(place) | IRIOMoveManagedCarrier(place) | IRIODeclareUninitialized(place):
			walkPlace(place, site, visitor);
		case IRIOStore(place, _) | IRIOBindVirtualTable(place, _):
			walkPlace(place, site, visitor);
		case IRIODeallocate(place, implementation) | IRIORetain(place, implementation) | IRIORelease(place, implementation) |
			IRIOTrace(place, implementation) | IRIODeclareManagedCarrier(place, implementation):
			walkPlace(place, site, visitor);
			walkImplementation(implementation, site, visitor);
		case IRIOAcquireManagedCarrier(place, _, acquisition):
			walkPlace(place, site, visitor);
			switch acquisition {
				case IRMCAMoveFresh:
				case IRMCARetainBorrowed(implementation): walkImplementation(implementation, site, visitor);
			}
		case IRIODefaultInitialize(place, from, to) | IRIOInitialize(place, _, from, to) | IRIOInitializeFixedArray(place, _, from, to) |
			IRIOZeroInitializeFixedArray(place, from, to) | IRIOLifetime(place, from, to, _):
			walkPlace(place, site, visitor);
			walkInitializationState(from);
			walkInitializationState(to);
		case IRIOUnary(_, _, implementation) | IRIOBinary(_, _, _, implementation):
			walkImplementation(implementation, site, visitor);
		case IRIOConvert(_, kind, targetType, implementation, _):
			walkConversionKind(kind);
			walkTypeRef(targetType, site, visitor);
			walkImplementation(implementation, site, visitor);
		case IRIODynamic(operation):
			walkDynamicInstruction(operation);
		case IRIOException(operation):
			walkExceptionInstruction(operation);
		case IRIOCall(call):
			walkCall(call, site, visitor);
		case IRIOConstructAggregate(_, _) | IRIOZeroAggregate(_) | IRIOConstructInterface(_, _, _) | IRIOUpcastInterface(_, _, _, _) | IRIOProject(_, _) |
			IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _):
		case IRIOProjectTag(_, _, _, check):
			walkTagCheckPolicy(check, site, visitor);
		case IRIOAllocate(type, intent, implementation, _):
			walkTypeRef(type, site, visitor);
			walkAllocationIntent(intent);
			walkImplementation(implementation, site, visitor);
		case IRIOInitializeSpan(place, sourceArray, from, to):
			walkPlace(place, site, visitor);
			walkPlace(sourceArray, site, visitor);
			walkInitializationState(from);
			walkInitializationState(to);
		case IRIOBorrowSpan(sourceArray):
			walkPlace(sourceArray, site, visitor);
		case IRIOBoundsCheck(collection, _, policy):
			walkPlace(collection, site, visitor);
			walkBoundsPolicy(policy, site, visitor);
		case IRIONullCheck(_, policy):
			walkNullCheckPolicy(policy, site, visitor);
	}
}

/**
	Return the one structurally owned failure edge of an instruction, when present.

	Control-flow analysis runs before the final freeze boundary, while the full
	walker runs after it. Sharing this exhaustive projection prevents those two
	phases from independently guessing which instruction constructors may fail.
**/
function instructionFailureEdge(kind:HxcIRInstructionKind):Null<HxcIRFailureEdge> {
	return switch kind {
		case IRIOConvert(_, _, _, _, failure) | IRIOAllocate(_, _, _, failure): failure;
		case IRIOCall(call): call.failure;
		case IRIODynamic(operation): switch operation {
				case IRDUnbox(_, _,
					failure) | IRDGet(_, _, failure) | IRDSet(_, _, _, failure) | IRDCall(_, _, _, failure) | IRDInvoke(_, _, _, failure): failure;
				case IRDBox(_, _) | IRDBoxNull(_) | IRDBoxTypeToken(_) | IRDEqual(_, _, _): null;
			};
		case IRIOSequence(_) | IRIOConstant(_) | IRIOFunctionReference(_) | IRIOLoad(_) | IRIOStore(_, _) | IRIOAddress(_) | IRIOBorrowClassField(_) |
			IRIOUnary(_, _, _) | IRIOBinary(_, _, _, _) | IRIOException(_) | IRIOConstructAggregate(_, _) | IRIOZeroAggregate(_) |
			IRIOConstructInterface(_, _, _) | IRIOUpcastInterface(_, _, _, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _) |
			IRIOProjectTag(_, _, _, _) | IRIODeallocate(_, _) | IRIORetain(_, _) | IRIORelease(_, _) | IRIOTrace(_, _) | IRIODeclareUninitialized(_) |
			IRIODeclareManagedCarrier(_, _) | IRIOAcquireManagedCarrier(_, _, _) | IRIOMoveManagedCarrier(_) | IRIODefaultInitialize(_, _, _) |
			IRIOInitialize(_, _, _, _) | IRIOInitializeFixedArray(_, _, _, _) | IRIOZeroInitializeFixedArray(_, _, _) | IRIOInitializeSpan(_, _, _, _) |
			IRIOBorrowSpan(_) | IRIOBindVirtualTable(_, _) | IRIOBoundsCheck(_, _, _) | IRIONullCheck(_, _) | IRIOLifetime(_, _, _, _): null;
	};
}

function walkConversionKind(kind:HxcIRConversionKind):Void {
	switch kind {
		case IRCNumericExact | IRCNumericRoundBinary32 | IRCNumericWidenBinary64 | IRCNumericWrapping | IRCNumericSaturating | IRCNumericChecked |
			IRCNullableInject | IRCNullableUnwrap | IRCPointer | IRCBox | IRCUnbox | IRCRepresentation:
	}
}

function walkAllocationIntent(intent:HxcIRAllocationIntent):Void {
	switch intent {
		case IRAStack | IRAOwned | IRAShared | IRAArena(_):
	}
}

function walkCall(call:HxcIRCall, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	switch call.dispatch {
		case IRCDDirect(_) | IRCDVirtual(_, _) | IRCDInterface(_, _, _) | IRCDClosure(_) | IRCDNative(_) | IRCDRuntime(_, _) | IRCDIntrinsic(_):
	}
	walkTypeRef(call.returnType, site, visitor);
	if (call.borrowedSpanReturn != null)
		walkBorrowedSpanReturn(call.borrowedSpanReturn);
}

function walkDynamicInstruction(operation:HxcIRDynamicInstruction):Void {
	switch operation {
		case IRDBox(_, _) | IRDBoxNull(_) | IRDBoxTypeToken(_) | IRDEqual(_, _, _):
		case IRDUnbox(_, _, _) | IRDGet(_, _, _) | IRDSet(_, _, _, _) | IRDCall(_, _, _, _) | IRDInvoke(_, _, _, _):
	}
}

function walkExceptionInstruction(operation:HxcIRExceptionInstruction):Void {
	switch operation {
		case IREFramePush(_) | IREFrameSetJmp(_) | IREFramePayload(_) | IREFramePop(_) | IRECleanupPush(_) | IRECleanupRun(_) | IRECleanupDiscard(_):
	}
}

function walkPlace(place:HxcIRPlace, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onPlace(place, site);
	switch place {
		case IRPLocal(_) | IRPGlobal(_) | IRPDereference(_):
		case IRPField(base, _) | IRPIndex(base, _):
			walkPlace(base, site, visitor);
	}
}

function walkImplementation(implementation:HxcIRImplementation, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onImplementation(implementation, site);
	switch implementation {
		case IRIStatic | IRIProgramLocal(_) | IRIRuntime(_):
	}
}

function walkFailureEdge(edge:HxcIRFailureEdge, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onFailureEdge(edge, site);
	walkFailureKind(edge.kind);
	switch edge.target {
		case IRFTBlock(_) | IRFTPropagate | IRFTUnwind | IRFTAbort:
	}
	for (step in edge.cleanup) {
		visitor.onCleanupStep(step, site);
		// Cleanup steps are semantic region/action references, not nested actions.
	}
}

function walkCleanupAction(action:HxcIRCleanupAction, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onCleanupAction(action, site);
	switch action.idempotence {
		case IRCExactlyOnce | IRCIdempotent:
	}
	switch action.kind {
		case IRCADestroy(place, from, to):
			walkPlace(place, site, visitor);
			walkInitializationState(from);
			walkInitializationState(to);
		case IRCARelease(place, implementation) | IRCADeallocate(place, implementation):
			walkPlace(place, site, visitor);
			walkImplementation(implementation, site, visitor);
		case IRCAFinally(_):
	}
}

function walkTerminator(terminator:HxcIRTerminator, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onTerminator(terminator, site);
	switch terminator.kind {
		case IRTJump(edge):
			walkBlockEdge(edge, site, visitor);
		case IRTBranch(_, whenTrue, whenFalse):
			walkBlockEdge(whenTrue, site, visitor);
			walkBlockEdge(whenFalse, site, visitor);
		case IRTSwitch(_, cases, defaultEdge):
			for (item in cases) {
				walkConstant(item.value);
				walkBlockEdge(item.edge, site, visitor);
			}
			walkBlockEdge(defaultEdge, site, visitor);
		case IRTTagSwitch(_, cases, defaultEdge):
			for (item in cases)
				walkBlockEdge(item.edge, site, visitor);
			if (defaultEdge != null)
				walkBlockEdge(defaultEdge, site, visitor);
		case IRTReturn(_, cleanup):
			for (step in cleanup) {
				visitor.onCleanupStep(step, site);
				// Cleanup steps remain semantic references.
			}
		case IRTThrow(_, edge):
			walkFailureEdge(edge, site, visitor);
		case IRTUnreachable:
	}
}

function walkBlockEdge(edge:HxcIRBlockEdge, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onBlockEdge(edge, site);
	for (step in edge.cleanup) {
		visitor.onCleanupStep(step, site);
		// Cleanup steps remain semantic references.
	}
}

function walkBoundsPolicy(policy:HxcIRBoundsPolicy, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onBoundsPolicy(policy, site);
	switch policy {
		case IRBPCheckedAbort(_, _) | IRBPStaticProof(_, _) | IRBPLoopGuarded(_, _, _):
	}
}

function walkNullCheckPolicy(policy:HxcIRNullCheckPolicy, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onNullCheckPolicy(policy, site);
	switch policy {
		case IRNCPCheckedAbort(_, _):
	}
}

function walkTagCheckPolicy(policy:HxcIRTagCheckPolicy, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onTagCheckPolicy(policy, site);
	switch policy {
		case IRTCPCheckedAbort(_, _):
	}
}

function walkTypeRef(type:HxcIRTypeRef, site:HxcIRTraversalSite, visitor:HxcIRTraversalVisitor):Void {
	visitor.onTypeRef(type, site);
	switch type {
		case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTManagedString | IRTCString | IRTCallScopedCString |
			IRTMutableCStringBuffer | IRTVoid | IRTInstance(_) | IRTDynamic:
		case IRTPointer(pointee, _) | IRTNullable(pointee, _) | IRTFixedArray(pointee, _, _) | IRTSpan(pointee, _):
			walkTypeRef(pointee, site, visitor);
		case IRTFunction(parameters, result):
			for (parameter in parameters)
				walkTypeRef(parameter, site, visitor);
			walkTypeRef(result, site, visitor);
	}
}
