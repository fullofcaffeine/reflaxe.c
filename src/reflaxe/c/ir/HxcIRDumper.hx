package reflaxe.c.ir;

import haxe.io.Bytes;
import reflaxe.c.ir.HxcIR;

/** Canonical, source-aware text dump for semantic review and golden tests. */
class HxcIRDumper {
	var output:Array<String> = [];

	public function new() {}

	public function dump(program:HxcIRProgram):String {
		output = ['hxcir schema=${program.schemaVersion}'];
		for (module in sorted(program.modules, item -> item.id)) {
			dumpModule(module);
		}
		return output.join("\n") + "\n";
	}

	function dumpModule(module:HxcIRModule):Void {
		line('module ${quote(module.id)} ${source(module.source)}');
		for (declaration in sorted(module.types, item -> item.id)) {
			dumpType(declaration);
		}
		for (instance in sorted(module.typeInstances, item -> item.id)) {
			line('  instance ${quote(instance.id)} declaration=${quote(instance.declarationId)} arguments=${types(instance.arguments)} representation=${representation(instance.representation)} ${source(instance.source)}');
		}
		for (global in sorted(module.globals, item -> item.id)) {
			line('  global ${quote(global.id)} type=${typeRef(global.type)} mutable=${global.mutable} initialization=${globalInitialization(global.initialization)} ${source(global.source)}');
		}
		for (fn in sorted(module.functions, item -> item.id)) {
			dumpFunction(fn);
		}
		line('end module ${quote(module.id)}');
	}

	function dumpType(declaration:HxcIRTypeDeclaration):Void {
		final kindName = switch declaration.kind {
			case IRTKPrimitive: "primitive";
			case IRTKAggregate(_): "aggregate";
			case IRTKTaggedUnion(_): "tagged-union";
			case IRTKReference: "reference";
			case IRTKFunction: "function";
			case IRTKExtern: "extern";
		}
		line('  type ${quote(declaration.id)} name=${quote(declaration.displayName)} kind=$kindName ${source(declaration.source)}');
		switch declaration.kind {
			case IRTKAggregate(fields):
				for (field in fields) {
					line('    field ${quote(field.name)} type=${typeRef(field.type)} mutable=${field.mutable} ${source(field.source)}');
				}
			case IRTKTaggedUnion(cases):
				for (tag in cases) {
					line('    case ${quote(tag.name)} payload=${types(tag.payload)} ${source(tag.source)}');
				}
			case IRTKPrimitive | IRTKReference | IRTKFunction | IRTKExtern:
		}
	}

	function dumpFunction(fn:HxcIRFunction):Void {
		line('  function ${quote(fn.id)} name=${quote(fn.displayName)} returns=${typeRef(fn.returnType)} entry=${quote(fn.entryBlockId)} ${source(fn.source)}');
		for (parameter in fn.parameters) {
			line('    parameter ${quote(parameter.id)} type=${typeRef(parameter.type)} ${source(parameter.source)}');
		}
		for (local in sorted(fn.locals, item -> item.id)) {
			line('    local ${quote(local.id)} type=${typeRef(local.type)} storage=${localStorage(local.storage)} state=${state(local.initialState)} ${source(local.source)}');
		}
		for (region in sorted(fn.cleanupRegions, item -> item.id)) {
			line('    cleanup ${quote(region.id)} parent=${nullableQuote(region.parentId)} registration-order ${source(region.source)}');
			for (action in region.actions) {
				line('      action ${quote(action.id)} idempotence=${idempotence(action.idempotence)} ${cleanupAction(action.kind)} ${source(action.source)}');
			}
		}
		for (block in sortedBlocks(fn.blocks, fn.entryBlockId)) {
			dumpBlock(block);
		}
		line('  end function ${quote(fn.id)}');
	}

	function dumpBlock(block:HxcIRBlock):Void {
		line('    block ${quote(block.id)} ${source(block.source)}');
		for (parameter in block.parameters) {
			line('      parameter ${quote(parameter.id)} type=${typeRef(parameter.type)} ${source(parameter.source)}');
		}
		for (instruction in block.instructions) {
			final result = instruction.result == null ? "result=-" : 'result=${quote(instruction.result.id)}:${typeRef(instruction.result.type)}';
			line('      instruction ${quote(instruction.id)} $result ${instructionKind(instruction.kind)} ${source(instruction.source)}');
		}
		if (block.terminator == null) {
			line("      terminator <missing>");
		} else {
			line('      terminator ${terminator(block.terminator.kind)} ${source(block.terminator.source)}');
		}
		line('    end block ${quote(block.id)}');
	}

	function instructionKind(kind:HxcIRInstructionKind):String {
		return switch kind {
			case IRIOSequence(label): 'sequence label=${quote(label)}';
			case IRIOConstant(value): 'constant value=${constant(value)}';
			case IRIOLoad(place): 'load place=${renderPlace(place)}';
			case IRIOStore(place, valueId): 'store place=${renderPlace(place)} value=${quote(valueId)}';
			case IRIOAddress(place): 'address place=${renderPlace(place)}';
			case IRIOUnary(operationId, valueId, selected):
				'unary operation=${quote(operationId)} value=${quote(valueId)} implementation=${implementation(selected)}';
			case IRIOBinary(operationId, leftValueId, rightValueId, selected):
				'binary operation=${quote(operationId)} left=${quote(leftValueId)} right=${quote(rightValueId)} implementation=${implementation(selected)}';
			case IRIOConvert(valueId, kind, targetType, selected, failure):
				'convert value=${quote(valueId)} kind=${conversion(kind)} target=${typeRef(targetType)} implementation=${implementation(selected)} failure=${failure == null ? "none" : failureEdge(failure)}';
			case IRIOCall(call): renderCall(call);
			case IRIOConstructAggregate(instanceId, fields):
				'construct-aggregate instance=${quote(instanceId)} fields=[${fields.map(field -> quote(field.name) + "=" + quote(field.valueId)).join(",")}]';
			case IRIOProject(valueId, fieldName): 'project value=${quote(valueId)} field=${quote(fieldName)}';
			case IRIOConstructTag(instanceId, tagName, payload):
				'construct-tag instance=${quote(instanceId)} tag=${quote(tagName)} payload=${strings(payload)}';
			case IRIOMatchTag(valueId, tagName): 'match-tag value=${quote(valueId)} tag=${quote(tagName)}';
			case IRIOAllocate(type, intent, selected, failure):
				'allocate type=${typeRef(type)} intent=${allocation(intent)} implementation=${implementation(selected)} failure=${failure == null ? "none" : failureEdge(failure)}';
			case IRIODeallocate(place, selected): 'deallocate place=${renderPlace(place)} implementation=${implementation(selected)}';
			case IRIORetain(place, selected): 'retain place=${renderPlace(place)} implementation=${implementation(selected)}';
			case IRIOTrace(place, selected): 'trace place=${renderPlace(place)} implementation=${implementation(selected)}';
			case IRIOInitialize(place, valueId, from, to):
				'initialize place=${renderPlace(place)} value=${quote(valueId)} transition=${state(from)}->${state(to)}';
			case IRIOInitializeFixedArray(place, values, from, to):
				'initialize-fixed-array place=${renderPlace(place)} values=${strings(values)} transition=${state(from)}->${state(to)}';
			case IRIOInitializeSpan(place, sourceArray, from, to):
				'initialize-span place=${renderPlace(place)} source=${renderPlace(sourceArray)} transition=${state(from)}->${state(to)}';
			case IRIOBoundsCheck(collection, indexValueId, policy):
				'bounds-check collection=${renderPlace(collection)} index=${quote(indexValueId)} policy=${boundsPolicy(policy)}';
			case IRIOLifetime(place, from, to, reason):
				'lifetime place=${renderPlace(place)} transition=${state(from)}->${state(to)} reason=${quote(reason)}';
		}
	}

	function renderCall(call:HxcIRCall):String {
		return
			'call dispatch=${dispatch(call.dispatch)} arguments=${strings(call.arguments)} returns=${typeRef(call.returnType)} failure=${call.failure == null ? "none" : failureEdge(call.failure)}';
	}

	function dispatch(value:HxcIRCallDispatch):String {
		return switch value {
			case IRCDDirect(functionId): 'direct(${quote(functionId)})';
			case IRCDVirtual(slotId, receiverValueId): 'virtual(slot=${quote(slotId)},receiver=${quote(receiverValueId)})';
			case IRCDInterface(interfaceTypeId, slotId, receiverValueId):
				'interface(type=${quote(interfaceTypeId)},slot=${quote(slotId)},receiver=${quote(receiverValueId)})';
			case IRCDClosure(callableValueId): 'closure(${quote(callableValueId)})';
			case IRCDNative(symbol): 'native(${quote(symbol)})';
			case IRCDRuntime(featureId, operationId): 'runtime(feature=${quote(featureId)},operation=${quote(operationId)})';
			case IRCDIntrinsic(intrinsicId): 'intrinsic(${quote(intrinsicId)})';
		}
	}

	function terminator(value:HxcIRTerminatorKind):String {
		return switch value {
			case IRTJump(edge): 'jump ${blockEdge(edge)}';
			case IRTBranch(conditionValueId, whenTrue, whenFalse):
				'branch condition=${quote(conditionValueId)} true=${blockEdge(whenTrue)} false=${blockEdge(whenFalse)}';
			case IRTSwitch(valueId, cases, defaultEdge):
				'switch value=${quote(valueId)} cases=[${cases.map(item -> constant(item.value) + "=>" + blockEdge(item.edge)).join(",")}] default=${blockEdge(defaultEdge)}';
			case IRTReturn(valueId, cleanup): 'return value=${nullableQuote(valueId)} cleanup=${cleanupPath(cleanup)}';
			case IRTThrow(valueId, edge): 'throw value=${quote(valueId)} edge=${failureEdge(edge)}';
			case IRTUnreachable: "unreachable";
		}
	}

	function blockEdge(edge:HxcIRBlockEdge):String
		return 'edge(target=${quote(edge.targetBlockId)},arguments=${strings(edge.arguments)},cleanup=${cleanupPath(edge.cleanup)})';

	function failureEdge(edge:HxcIRFailureEdge):String {
		return
			'failure(kind=${failureKind(edge.kind)},target=${failureTarget(edge.target)},arguments=${strings(edge.arguments)},cleanup=${cleanupPath(edge.cleanup)})';
	}

	function failureTarget(value:HxcIRFailureTarget):String {
		return switch value {
			case IRFTBlock(blockId): 'block(${quote(blockId)})';
			case IRFTPropagate: "propagate";
			case IRFTAbort: "abort";
		}
	}

	function cleanupAction(value:HxcIRCleanupActionKind):String {
		return switch value {
			case IRCADestroy(place, from, to): 'destroy place=${renderPlace(place)} transition=${state(from)}->${state(to)}';
			case IRCARelease(place, selected): 'release place=${renderPlace(place)} implementation=${implementation(selected)}';
			case IRCADeallocate(place, selected): 'deallocate place=${renderPlace(place)} implementation=${implementation(selected)}';
			case IRCAFinally(blockId): 'finally block=${quote(blockId)}';
		}
	}

	function renderPlace(value:HxcIRPlace):String {
		return switch value {
			case IRPLocal(localId): 'local(${quote(localId)})';
			case IRPGlobal(globalId): 'global(${quote(globalId)})';
			case IRPDereference(pointerValueId): 'dereference(${quote(pointerValueId)})';
			case IRPField(base, fieldName): 'field(${renderPlace(base)},${quote(fieldName)})';
			case IRPIndex(base, indexValueId): 'index(${renderPlace(base)},${quote(indexValueId)})';
		}
	}

	function typeRef(value:HxcIRTypeRef):String {
		return switch value {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTAbiInteger(kind): 'abi-int(${abiIntegerKind(kind)})';
			case IRTFloat(width): 'f$width';
			case IRTString: "string-utf8";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance(${quote(instanceId)})';
			case IRTPointer(pointee, nullable): 'pointer(${nullable ? "nullable" : "nonnull"},${typeRef(pointee)})';
			case IRTNullable(inner, representation): 'nullable(${nullableRepresentation(representation)},${typeRef(inner)})';
			case IRTFunction(parameters, result): 'function(${parameters.map(typeRef).join(",")})->${typeRef(result)}';
			case IRTFixedArray(element, length, witnessId):
				'fixed-array(length=$length,witness=${quote(witnessId)},element=${typeRef(element)})';
			case IRTSpan(element, mutable): 'span(${mutable ? "mutable" : "const"},${typeRef(element)})';
			case IRTDynamic: "dynamic";
		}
	}

	function boundsPolicy(value:HxcIRBoundsPolicy):String {
		return switch value {
			case IRBPCheckedAbort(profile, buildMode): 'checked-abort(profile=${quote(profile)},build=${quote(buildMode)})';
			case IRBPStaticProof(length, index): 'static-proof(length=$length,index=$index)';
			case IRBPLoopGuarded(guardInstructionId, indexLocalId, length):
				'loop-guarded(guard=${quote(guardInstructionId)},index-local=${quote(indexLocalId)},length=$length)';
		}
	}

	function abiIntegerKind(value:HxcIRAbiIntegerKind):String {
		return switch value {
			case IRAKSize: "size";
			case IRAKPtrDiff: "ptrdiff";
			case IRAKIntPtr: "intptr";
			case IRAKUIntPtr: "uintptr";
		}
	}

	function nullableRepresentation(value:HxcIRNullableRepresentation):String {
		return switch value {
			case IRNTagged: "tagged";
			case IRNPointer: "pointer";
		}
	}

	function representation(value:HxcIRRepresentation):String {
		return switch value {
			case IRRDirect: "direct";
			case IRRTagged: "tagged";
			case IRROpaqueHandle: "opaque-handle";
			case IRRManaged(runtimeFeature): 'managed(${quote(runtimeFeature)})';
		}
	}

	function globalInitialization(value:HxcIRGlobalInitialization):String {
		return switch value {
			case IRGIUninitialized: "uninitialized";
			case IRGIConstant(item): 'constant(${constant(item)})';
			case IRGIDeferred(initializerFunctionId): 'deferred(${quote(initializerFunctionId)})';
		}
	}

	function implementation(value:HxcIRImplementation):String {
		return switch value {
			case IRIStatic: "static";
			case IRIProgramLocal(helperId): 'program-local(${quote(helperId)})';
			case IRIRuntime(featureId): 'runtime(${quote(featureId)})';
		}
	}

	function conversion(value:HxcIRConversionKind):String {
		return switch value {
			case IRCNumericExact: "numeric-exact";
			case IRCNumericWrapping: "numeric-wrapping";
			case IRCNumericSaturating: "numeric-saturating";
			case IRCNumericChecked: "numeric-checked";
			case IRCNullableInject: "nullable-inject";
			case IRCNullableUnwrap: "nullable-unwrap";
			case IRCPointer: "pointer";
			case IRCBox: "box";
			case IRCUnbox: "unbox";
			case IRCRepresentation: "representation";
		}
	}

	function allocation(value:HxcIRAllocationIntent):String {
		return switch value {
			case IRAStack: "stack";
			case IRAOwned: "owned";
			case IRAShared: "shared";
			case IRAArena(arenaId): 'arena(${quote(arenaId)})';
		}
	}

	function localStorage(value:HxcIRLocalStorage):String {
		return switch value {
			case IRLSAutomatic: "automatic";
			case IRLSStatic: "static";
			case IRLSFrame: "frame";
			case IRLSRegion(regionId): 'region(${quote(regionId)})';
		}
	}

	function state(value:HxcIRInitializationState):String {
		return switch value {
			case IRISUninitialized: "uninitialized";
			case IRISInitializing: "initializing";
			case IRISInitialized: "initialized";
			case IRISMoved: "moved";
			case IRISDestroyed: "destroyed";
		}
	}

	function failureKind(value:HxcIRFailureKind):String {
		return switch value {
			case IRFException: "exception";
			case IRFResultError: "result-error";
			case IRFAllocationFailure: "allocation-failure";
			case IRFNativeStatus: "native-status";
		}
	}

	function idempotence(value:HxcIRCleanupIdempotence):String {
		return switch value {
			case IRCExactlyOnce: "exactly-once";
			case IRCIdempotent: "idempotent";
		}
	}

	function constant(value:HxcIRConstant):String {
		return switch value {
			case IRCInt(text): 'int($text)';
			case IRCFloat(text): 'float($text)';
			case IRCBool(flag): 'bool($flag)';
			case IRCString(text, byteLength): 'string-utf8(bytes=$byteLength,value=${quote(text)})';
			case IRCNull: "null";
		}
	}

	function cleanupPath(steps:Array<HxcIRCleanupStep>):String
		return '[${steps.map(step -> quote(step.regionId) + "." + quote(step.actionId)).join(",")}]';

	function types(values:Array<HxcIRTypeRef>):String
		return '[${values.map(typeRef).join(",")}]';

	function strings(values:Array<String>):String
		return '[${values.map(quote).join(",")}]';

	function nullableQuote(value:Null<String>):String
		return value == null ? "none" : quote(value);

	function source(value:HxcSourceSpan):String
		return '@${quote(value.file)}:${value.startLine}:${value.startColumn}-${value.endLine}:${value.endColumn}';

	function line(value:String):Void
		output.push(value);

	static function quote(value:String):String
		return HxcJsonString.quote(value);

	static function sorted<T>(values:Array<T>, key:T->String):Array<T> {
		final copy = values.copy();
		copy.sort((left, right) -> compareUtf8(key(left), key(right)));
		return copy;
	}

	static function sortedBlocks(values:Array<HxcIRBlock>, entryBlockId:String):Array<HxcIRBlock> {
		final copy = values.copy();
		copy.sort((left, right) -> {
			if (left.id == entryBlockId)
				return right.id == entryBlockId ? 0 : -1;
			if (right.id == entryBlockId)
				return 1;
			return compareUtf8(left.id, right.id);
		});
		return copy;
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}
