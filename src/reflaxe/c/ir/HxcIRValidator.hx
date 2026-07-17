package reflaxe.c.ir;

import haxe.io.Bytes;
import reflaxe.c.ir.HxcIR;

private typedef HxcIRInstructionSite = {
	final instruction:HxcIRInstruction;
	final block:HxcIRBlock;
}

/** Validates the semantic invariants required before any HxcIR reaches C AST lowering. */
class HxcIRValidator {
	public static inline final SCHEMA_VERSION = 4;

	public function new() {}

	public function validate(program:HxcIRProgram, profile:String):Array<HxcIRDiagnostic>
		return new HxcIRValidationState(program, profile).validate();

	public function requireValid(program:HxcIRProgram, profile:String):Void {
		final diagnostics = validate(program, profile);
		if (diagnostics.length > 0) {
			throw new HxcIRValidationError(diagnostics);
		}
	}
}

private class HxcIRValidationState {
	final program:HxcIRProgram;
	final profile:String;
	final diagnostics:Array<HxcIRDiagnostic> = [];
	final typeDeclarations:Map<String, HxcIRTypeDeclaration> = [];
	final typeInstances:Map<String, HxcIRTypeInstance> = [];
	final globals:Map<String, HxcIRGlobal> = [];
	final functions:Map<String, HxcIRFunction> = [];

	public function new(program:HxcIRProgram, profile:String) {
		this.program = program;
		this.profile = profile;
	}

	public function validate():Array<HxcIRDiagnostic> {
		if (program.schemaVersion != HxcIRValidator.SCHEMA_VERSION) {
			add("program", 'schema version ${program.schemaVersion} is unsupported; expected ${HxcIRValidator.SCHEMA_VERSION}', programSource());
		}
		indexProgram();
		validateProgramContents();
		validateFiniteDirectLayouts();
		diagnostics.sort(compareDiagnostics);
		return diagnostics;
	}

	function programSource():HxcSourceSpan {
		if (program.modules.length > 0) {
			return program.modules[0].source;
		}
		return new HxcSourceSpan("hxcir/program", 1, 1, 1, 1);
	}

	function indexProgram():Void {
		final moduleIds:Map<String, Bool> = [];
		for (module in sorted(program.modules, item -> item.id)) {
			final path = 'module:${module.id}';
			validateStableId(module.id, '${path}.id', module.source);
			validateSpan(module.source, '${path}.source');
			if (moduleIds.exists(module.id)) {
				add(path, 'duplicate module ID `${module.id}`', module.source);
			} else {
				moduleIds.set(module.id, true);
			}

			for (declaration in sorted(module.types, item -> item.id)) {
				indexUnique(typeDeclarations, declaration.id, declaration, '$path.type:${declaration.id}', declaration.source, "type declaration");
			}
			for (instance in sorted(module.typeInstances, item -> item.id)) {
				indexUnique(typeInstances, instance.id, instance, '$path.instance:${instance.id}', instance.source, "type instance");
			}
			for (global in sorted(module.globals, item -> item.id)) {
				indexUnique(globals, global.id, global, '$path.global:${global.id}', global.source, "global");
			}
			for (fn in sorted(module.functions, item -> item.id)) {
				indexUnique(functions, fn.id, fn, '$path.function:${fn.id}', fn.source, "function");
			}
		}
	}

	function indexUnique<T>(index:Map<String, T>, id:String, value:T, path:String, source:HxcSourceSpan, label:String):Void {
		validateStableId(id, '$path.id', source);
		if (index.exists(id)) {
			add(path, 'duplicate $label ID `$id`', source);
		} else {
			index.set(id, value);
		}
	}

	function validateProgramContents():Void {
		for (module in sorted(program.modules, item -> item.id)) {
			final path = 'module:${module.id}';
			for (declaration in sorted(module.types, item -> item.id)) {
				validateTypeDeclaration(declaration, '$path.type:${declaration.id}');
			}
			for (instance in sorted(module.typeInstances, item -> item.id)) {
				validateTypeInstance(instance, '$path.instance:${instance.id}');
			}
			for (global in sorted(module.globals, item -> item.id)) {
				validateGlobal(global, '$path.global:${global.id}');
			}
			for (fn in sorted(module.functions, item -> item.id)) {
				validateFunction(fn, '$path.function:${fn.id}');
			}
		}
	}

	function validateTypeDeclaration(declaration:HxcIRTypeDeclaration, path:String):Void {
		validateSpan(declaration.source, '$path.source');
		validateText(declaration.displayName, '$path.displayName', declaration.source);
		switch declaration.kind {
			case IRTKPrimitive | IRTKReference | IRTKFunction | IRTKExtern:
			case IRTKAggregate(fields):
				final names:Map<String, Bool> = [];
				for (index => field in fields) {
					final fieldPath = '$path.field:$index:${field.name}';
					validateStableId(field.name, '$fieldPath.name', field.source);
					validateSpan(field.source, '$fieldPath.source');
					validateTypeRef(field.type, '$fieldPath.type', field.source, false);
					if (names.exists(field.name)) {
						add(fieldPath, 'duplicate aggregate field `${field.name}`', field.source);
					} else {
						names.set(field.name, true);
					}
				}
			case IRTKTaggedUnion(cases):
				final names:Map<String, Bool> = [];
				final values:Map<Int, Bool> = [];
				for (index => tag in cases) {
					final tagPath = '$path.case:$index:${tag.name}';
					validateStableId(tag.name, '$tagPath.name', tag.source);
					validateSpan(tag.source, '$tagPath.source');
					if (names.exists(tag.name)) {
						add(tagPath, 'duplicate tagged-union case `${tag.name}`', tag.source);
					} else {
						names.set(tag.name, true);
					}
					if (tag.tagValue < 0 || values.exists(tag.tagValue)) {
						add(tagPath, 'tagged-union case `${tag.name}` has invalid or duplicate discriminant `${tag.tagValue}`', tag.source);
					} else {
						values.set(tag.tagValue, true);
					}
					if (tag.tagValue != index) {
						add(tagPath, 'tagged-union case `${tag.name}` must retain source discriminant $index, found ${tag.tagValue}', tag.source);
					}
					final payloadNames:Map<String, Bool> = [];
					for (payloadIndex => payload in tag.payload) {
						final payloadPath = '$tagPath.payload:$payloadIndex:${payload.name}';
						validateStableId(payload.name, '$payloadPath.name', payload.source);
						validateSpan(payload.source, '$payloadPath.source');
						validateTypeRef(payload.type, '$payloadPath.type', payload.source, false);
						if (payloadNames.exists(payload.name)) {
							add(payloadPath, 'duplicate payload name `${payload.name}` in tagged-union case `${tag.name}`', payload.source);
						} else {
							payloadNames.set(payload.name, true);
						}
					}
				}
		}
	}

	function validateTypeInstance(instance:HxcIRTypeInstance, path:String):Void {
		validateSpan(instance.source, '$path.source');
		final declaration = typeDeclarations.get(instance.declarationId);
		if (declaration == null) {
			add(path, 'type instance `${instance.id}` refers to unknown declaration `${instance.declarationId}`', instance.source);
		} else {
			switch declaration.kind {
				case IRTKTaggedUnion(cases) if (instance.representation == IRRDirect):
					for (tagCase in cases) {
						if (tagCase.payload.length > 0) {
							add(path, 'direct native-enum instance `${instance.id}` cannot contain payload case `${tagCase.name}`', instance.source);
							break;
						}
					}
				case _:
			}
		}
		for (index => argument in instance.arguments) {
			validateTypeRef(argument, '$path.argument:$index', instance.source, false);
		}
		switch instance.representation {
			case IRRManaged(runtimeFeature):
				validateStableId(runtimeFeature, '$path.runtimeFeature', instance.source);
			case IRRDirect | IRRTagged | IRROpaqueHandle:
		}
	}

	function validateGlobal(global:HxcIRGlobal, path:String):Void {
		validateSpan(global.source, '$path.source');
		validateTypeRef(global.type, '$path.type', global.source, false);
		switch global.initialization {
			case IRGIUninitialized:
			case IRGIConstant(value):
				validateConstant(value, '$path.initialization', global.source);
			case IRGIDeferred(initializerFunctionId):
				final initializer = functions.get(initializerFunctionId);
				if (initializer == null) {
					add(path, 'global `${global.id}` refers to unknown initializer function `$initializerFunctionId`', global.source);
				} else {
					if (initializer.parameters.length != 0 || initializer.returnType != IRTVoid) {
						add(path, 'global `${global.id}` initializer `$initializerFunctionId` must have signature `():Void`', global.source);
					}
					var initializeCount = 0;
					for (block in initializer.blocks) {
						for (instruction in block.instructions) {
							switch instruction.kind {
								case IRIOInitialize(IRPGlobal(globalId), _, IRISUninitialized, IRISInitialized) if (globalId == global.id):
									initializeCount++;
								case _:
							}
						}
					}
					if (initializeCount != 1) {
						add(path, 'global `${global.id}` initializer `$initializerFunctionId` must initialize it exactly once; found $initializeCount',
							global.source);
					}
				}
		}
	}

	function validateFunction(fn:HxcIRFunction, path:String):Void {
		validateSpan(fn.source, '$path.source');
		validateText(fn.displayName, '$path.displayName', fn.source);
		validateTypeRef(fn.returnType, '$path.returnType', fn.source, true);
		validateStableId(fn.entryBlockId, '$path.entryBlockId', fn.source);

		final locals:Map<String, HxcIRLocal> = [];
		final values:Map<String, HxcIRTypeRef> = [];
		for (index => parameter in fn.parameters) {
			final parameterPath = '$path.parameter:$index:${parameter.id}';
			validateParameter(parameter, parameterPath);
			indexValue(values, parameter.id, parameter.type, parameterPath, parameter.source);
		}
		for (local in sorted(fn.locals, item -> item.id)) {
			final localPath = '$path.local:${local.id}';
			validateStableId(local.id, '$localPath.id', local.source);
			validateSpan(local.source, '$localPath.source');
			validateTypeRef(local.type, '$localPath.type', local.source, false);
			if (locals.exists(local.id)) {
				add(localPath, 'duplicate local place ID `${local.id}`', local.source);
			} else {
				locals.set(local.id, local);
			}
			switch local.initialState {
				case IRISUninitialized | IRISInitialized:
				case IRISInitializing | IRISMoved | IRISDestroyed:
					add(localPath, 'local `${local.id}` begins in invalid state ${stateName(local.initialState)}', local.source);
			}
			switch local.storage {
				case IRLSRegion(regionId):
					validateStableId(regionId, '$localPath.storage.region', local.source);
				case IRLSAutomatic | IRLSStatic | IRLSFrame:
			}
		}

		final blocks:Map<String, HxcIRBlock> = [];
		final instructionIds:Map<String, Bool> = [];
		final instructionSites:Map<String, HxcIRInstructionSite> = [];
		final valueSites:Map<String, HxcIRInstructionSite> = [];
		for (block in sorted(fn.blocks, item -> item.id)) {
			final blockPath = '$path.block:${block.id}';
			validateStableId(block.id, '$blockPath.id', block.source);
			validateSpan(block.source, '$blockPath.source');
			if (blocks.exists(block.id)) {
				add(blockPath, 'duplicate basic block ID `${block.id}`', block.source);
			} else {
				blocks.set(block.id, block);
			}
			for (index => parameter in block.parameters) {
				final parameterPath = '$blockPath.parameter:$index:${parameter.id}';
				validateParameter(parameter, parameterPath);
				indexValue(values, parameter.id, parameter.type, parameterPath, parameter.source);
			}
			for (index => instruction in block.instructions) {
				final instructionPath = '$blockPath.instruction:$index:${instruction.id}';
				validateStableId(instruction.id, '$instructionPath.id', instruction.source);
				validateSpan(instruction.source, '$instructionPath.source');
				if (instructionIds.exists(instruction.id)) {
					add(instructionPath, 'duplicate instruction ID `${instruction.id}`', instruction.source);
				} else {
					instructionIds.set(instruction.id, true);
					instructionSites.set(instruction.id, {instruction: instruction, block: block});
				}
				if (instruction.result != null) {
					validateStableId(instruction.result.id, '$instructionPath.result.id', instruction.source);
					validateTypeRef(instruction.result.type, '$instructionPath.result.type', instruction.source, false);
					indexValue(values, instruction.result.id, instruction.result.type, '$instructionPath.result', instruction.source);
					if (!valueSites.exists(instruction.result.id)) {
						valueSites.set(instruction.result.id, {instruction: instruction, block: block});
					}
				}
			}
		}

		if (!blocks.exists(fn.entryBlockId)) {
			add(path, 'function `${fn.id}` entry block `${fn.entryBlockId}` does not exist', fn.source);
		}

		final regions:Map<String, HxcIRCleanupRegion> = [];
		for (region in sorted(fn.cleanupRegions, item -> item.id)) {
			final regionPath = '$path.cleanup:${region.id}';
			validateStableId(region.id, '$regionPath.id', region.source);
			validateSpan(region.source, '$regionPath.source');
			if (regions.exists(region.id)) {
				add(regionPath, 'duplicate cleanup region ID `${region.id}`', region.source);
			} else {
				regions.set(region.id, region);
			}
		}
		validateCleanupRegions(fn, path, locals, blocks, regions);
		for (local in fn.locals) {
			switch local.storage {
				case IRLSRegion(regionId) if (!regions.exists(regionId)):
					add('$path.local:${local.id}', 'local `${local.id}` refers to unknown storage region `$regionId`', local.source);
				case _:
			}
		}

		for (block in sorted(fn.blocks, item -> item.id)) {
			validateBlock(fn, block, '$path.block:${block.id}', locals, blocks, regions, instructionSites, valueSites);
		}
	}

	function validateParameter(parameter:HxcIRParameter, path:String):Void {
		validateStableId(parameter.id, '$path.id', parameter.source);
		validateSpan(parameter.source, '$path.source');
		validateTypeRef(parameter.type, '$path.type', parameter.source, false);
	}

	function indexValue(index:Map<String, HxcIRTypeRef>, id:String, type:HxcIRTypeRef, path:String, source:HxcSourceSpan):Void {
		if (index.exists(id)) {
			add(path, 'duplicate immutable value ID `$id`', source);
		} else {
			index.set(id, type);
		}
	}

	function validateCleanupRegions(fn:HxcIRFunction, functionPath:String, locals:Map<String, HxcIRLocal>, blocks:Map<String, HxcIRBlock>,
			regions:Map<String, HxcIRCleanupRegion>):Void {
		for (region in sorted(fn.cleanupRegions, item -> item.id)) {
			final path = '$functionPath.cleanup:${region.id}';
			if (region.parentId != null && !regions.exists(region.parentId)) {
				add(path, 'cleanup region `${region.id}` has unknown parent `${region.parentId}`', region.source);
			}
			final actionIds:Map<String, Bool> = [];
			for (index => action in region.actions) {
				final actionPath = '$path.action:$index:${action.id}';
				validateStableId(action.id, '$actionPath.id', action.source);
				validateSpan(action.source, '$actionPath.source');
				if (actionIds.exists(action.id)) {
					add(actionPath, 'duplicate cleanup action ID `${action.id}` in region `${region.id}`', action.source);
				} else {
					actionIds.set(action.id, true);
				}
				switch action.kind {
					case IRCADestroy(place, from, to):
						validateStableCleanupPlace(place, actionPath, action.source, locals);
						validateTransition(from, to, '$actionPath.transition', action.source);
						if (to != IRISDestroyed) {
							add(actionPath, "destroy cleanup must end in destroyed state", action.source);
						}
					case IRCARelease(place, implementation) | IRCADeallocate(place, implementation):
						validateStableCleanupPlace(place, actionPath, action.source, locals);
						validateImplementation(implementation, '$actionPath.implementation', action.source);
					case IRCAFinally(blockId):
						if (!blocks.exists(blockId)) {
							add(actionPath, 'finally cleanup refers to unknown block `$blockId`', action.source);
						}
				}
			}
		}

		for (region in sorted(fn.cleanupRegions, item -> item.id)) {
			final seen:Map<String, Bool> = [];
			var current:Null<HxcIRCleanupRegion> = region;
			while (current != null) {
				if (seen.exists(current.id)) {
					add('$functionPath.cleanup:${region.id}', 'cleanup parent cycle reaches `${current.id}`', region.source);
					break;
				}
				seen.set(current.id, true);
				current = current.parentId == null ? null : regions.get(current.parentId);
			}
		}
	}

	function validateBlock(fn:HxcIRFunction, block:HxcIRBlock, path:String, locals:Map<String, HxcIRLocal>, blocks:Map<String, HxcIRBlock>,
			regions:Map<String, HxcIRCleanupRegion>, instructionSites:Map<String, HxcIRInstructionSite>, valueSites:Map<String, HxcIRInstructionSite>):Void {
		final available:Map<String, HxcIRTypeRef> = [];
		for (parameter in fn.parameters) {
			available.set(parameter.id, parameter.type);
		}
		for (parameter in block.parameters) {
			available.set(parameter.id, parameter.type);
		}

		final boundsProofs:Map<String, Bool> = [];
		for (index => instruction in block.instructions) {
			final instructionPath = '$path.instruction:$index:${instruction.id}';
			validateInstruction(instruction, instructionPath, block, available, locals, blocks, regions, instructionSites, valueSites, boundsProofs);
			if (instruction.result != null) {
				available.set(instruction.result.id, instruction.result.type);
			}
		}

		if (block.terminator == null) {
			add(path, 'basic block `${block.id}` has no terminator', block.source);
			return;
		}
		validateSpan(block.terminator.source, '$path.terminator.source');
		validateTerminator(fn, block.terminator.kind, '$path.terminator', block.terminator.source, available, blocks, regions);
	}

	function validateInstruction(instruction:HxcIRInstruction, path:String, block:HxcIRBlock, available:Map<String, HxcIRTypeRef>,
			locals:Map<String, HxcIRLocal>, blocks:Map<String, HxcIRBlock>, regions:Map<String, HxcIRCleanupRegion>,
			instructionSites:Map<String, HxcIRInstructionSite>, valueSites:Map<String, HxcIRInstructionSite>, boundsProofs:Map<String, Bool>):Void {
		final resultExpected = instructionProducesValue(instruction.kind);
		if (resultExpected && instruction.result == null) {
			add(path, "value-producing instruction has no result", instruction.source);
		} else if (!resultExpected && instruction.result != null) {
			add(path, 'side-effect-only instruction unexpectedly defines `${instruction.result.id}`', instruction.source);
		}

		switch instruction.kind {
			case IRIOSequence(label):
				validateText(label, '$path.label', instruction.source);
			case IRIOConstant(value):
				validateConstant(value, '$path.constant', instruction.source);
				if (instruction.result != null && !constantMatchesType(value, instruction.result.type)) {
					add(path, "constant result type does not match its literal family", instruction.source);
				}
			case IRIOLoad(place):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals);
				final loadedType = knownPlaceType(place, available, locals);
				if (instruction.result != null && loadedType != null && typeKey(instruction.result.type) != typeKey(loadedType)) {
					add(path, "load result type does not match its place type", instruction.source);
				}
			case IRIOAddress(place):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals);
				final addressedType = knownPlaceType(place, available, locals);
				if (isRootPlace(place) && addressedType != null && isCollectionType(addressedType)) {
					add(path, "taking the address of fixed-array/span storage is outside the admitted bounds-proof model", instruction.source);
				}
				if (instruction.result != null && addressedType != null) {
					switch instruction.result.type {
						case IRTPointer(pointee, false) if (typeKey(pointee) == typeKey(addressedType)):
						case _:
							add(path, "address result must be a non-null pointer to its place type", instruction.source);
					}
				}
			case IRIOStore(place, valueId):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals);
				final storedType = requireValue(valueId, '$path.value', instruction.source, available);
				final storePlaceType = knownPlaceType(place, available, locals);
				if (isRootPlace(place) && storePlaceType != null && isCollectionType(storePlaceType)) {
					add(path, "whole fixed-array/span assignment is outside the admitted bounds-proof model", instruction.source);
				}
				if (storedType != null && storePlaceType != null && typeKey(storedType) != typeKey(storePlaceType)) {
					add(path, "stored value type does not match its place type", instruction.source);
				}
			case IRIOUnary(operationId, valueId, implementation):
				validateStableId(operationId, '$path.operation', instruction.source);
				requireValue(valueId, '$path.value', instruction.source, available);
				validateImplementation(implementation, '$path.implementation', instruction.source);
			case IRIOBinary(operationId, leftValueId, rightValueId, implementation):
				validateStableId(operationId, '$path.operation', instruction.source);
				requireValue(leftValueId, '$path.left', instruction.source, available);
				requireValue(rightValueId, '$path.right', instruction.source, available);
				validateImplementation(implementation, '$path.implementation', instruction.source);
			case IRIOConvert(valueId, kind, targetType, implementation, failure):
				final sourceType = requireValue(valueId, '$path.value', instruction.source, available);
				validateTypeRef(targetType, '$path.targetType', instruction.source, false);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (sourceType != null) {
					validateConversion(kind, sourceType, targetType, implementation, failure, path, instruction.source);
				}
				if (failure != null) {
					validateFailureEdge(failure, '$path.failure', instruction.source, available, blocks, regions);
				}
				if (instruction.result != null && typeKey(instruction.result.type) != typeKey(targetType)) {
					add(path, "conversion result type does not match its target type", instruction.source);
				}
			case IRIOCall(call):
				validateCall(call, path, instruction.source, available, blocks, regions);
				if (instruction.result != null && typeKey(instruction.result.type) != typeKey(call.returnType)) {
					add(path, "call result type does not match the call return type", instruction.source);
				}
			case IRIOConstructAggregate(instanceId, fields):
				requireInstance(instanceId, path, instruction.source);
				final expectedFields = directAggregateFields(instanceId, path, instruction.source);
				if (instruction.result != null) {
					switch instruction.result.type {
						case IRTInstance(resultInstanceId) if (resultInstanceId == instanceId):
						case _:
							add(path, "aggregate construction result must use the constructed instance type", instruction.source);
					}
				}
				final names:Map<String, Bool> = [];
				for (index => field in fields) {
					validateStableId(field.name, '$path.field:$index.name', instruction.source);
					final valueType = requireValue(field.valueId, '$path.field:$index.value', instruction.source, available);
					if (names.exists(field.name)) {
						add(path, 'aggregate construction repeats field `${field.name}`', instruction.source);
					} else {
						names.set(field.name, true);
					}
					if (expectedFields != null) {
						final expected = findAggregateField(expectedFields, field.name);
						if (expected == null) {
							add(path, 'aggregate construction names unknown field `${field.name}`', instruction.source);
						} else if (valueType != null && typeKey(valueType) != typeKey(expected.type)) {
							add(path, 'aggregate field `${field.name}` value type does not match its declaration', instruction.source);
						}
					}
				}
				if (expectedFields != null) {
					for (field in expectedFields) {
						if (!names.exists(field.name)) {
							add(path, 'aggregate construction omits required field `${field.name}`', instruction.source);
						}
					}
					if (fields.length == expectedFields.length) {
						for (index in 0...fields.length) {
							if (fields[index].name != expectedFields[index].name) {
								add(path, "aggregate construction fields must follow declaration order", instruction.source);
								break;
							}
						}
					}
				}
			case IRIOProject(valueId, fieldName):
				final valueType = requireValue(valueId, '$path.value', instruction.source, available);
				validateStableId(fieldName, '$path.field', instruction.source);
				if (valueType != null) {
					switch valueType {
						case IRTInstance(instanceId):
							final fields = directAggregateFields(instanceId, path, instruction.source);
							if (fields != null) {
								final field = findAggregateField(fields, fieldName);
								if (field == null) {
									add(path, 'aggregate projection names unknown field `$fieldName`', instruction.source);
								} else if (instruction.result != null && typeKey(instruction.result.type) != typeKey(field.type)) {
									add(path, 'aggregate projection result type does not match field `$fieldName`', instruction.source);
								}
							}
						case _:
							add(path, "aggregate projection requires a direct aggregate instance value", instruction.source);
					}
				}
			case IRIOConstructTag(instanceId, tagName, payload):
				final tagCase = requireTagCase(instanceId, tagName, path, instruction.source);
				if (instruction.result != null) {
					switch instruction.result.type {
						case IRTInstance(resultInstanceId) if (resultInstanceId == instanceId):
						case _:
							add(path, "tag construction result must use the constructed instance type", instruction.source);
					}
				}
				if (tagCase != null && payload.length != tagCase.payload.length) {
					add(path, 'tag construction provides ${payload.length} payload value(s) for ${tagCase.payload.length} field(s)', instruction.source);
				}
				for (index => valueId in payload) {
					final payloadType = requireValue(valueId, '$path.payload:$index', instruction.source, available);
					if (payloadType != null
						&& tagCase != null
						&& index < tagCase.payload.length
						&& typeKey(payloadType) != typeKey(tagCase.payload[index].type)) {
						add(path, 'tag payload value $index does not match `${tagCase.payload[index].name}`', instruction.source);
					}
				}
			case IRIOMatchTag(valueId, tagName):
				final valueType = requireValue(valueId, '$path.value', instruction.source, available);
				if (valueType != null) {
					requireTagCaseForType(valueType, tagName, path, instruction.source);
				}
				if (instruction.result != null && typeKey(instruction.result.type) != "bool") {
					add(path, "tag match result must have Bool type", instruction.source);
				}
			case IRIOProjectTag(valueId, tagName, payloadIndex, check):
				final valueType = requireValue(valueId, '$path.value', instruction.source, available);
				final tagCase = valueType == null ? null : requireTagCaseForType(valueType, tagName, path, instruction.source);
				if (payloadIndex < 0 || tagCase != null && payloadIndex >= tagCase.payload.length) {
					add(path, 'tag payload index $payloadIndex is outside case `$tagName`', instruction.source);
				} else if (tagCase != null
					&& instruction.result != null
					&& typeKey(instruction.result.type) != typeKey(tagCase.payload[payloadIndex].type)) {
					add(path, 'tag payload projection result does not match `${tagCase.payload[payloadIndex].name}`', instruction.source);
				}
				validateTagCheck(check, '$path.check', instruction.source);
			case IRIOAllocate(type, _, implementation, failure):
				validateTypeRef(type, '$path.type', instruction.source, false);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (failure != null) {
					validateFailureEdge(failure, '$path.failure', instruction.source, available, blocks, regions);
					if (failure.kind != IRFAllocationFailure) {
						add(path, "allocation failure edge must use allocation-failure kind", instruction.source);
					}
				}
			case IRIODeallocate(place, implementation) | IRIORetain(place, implementation) | IRIOTrace(place, implementation):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validateImplementation(implementation, '$path.implementation', instruction.source);
			case IRIOInitialize(place, valueId, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				final initializedType = requireValue(valueId, '$path.value', instruction.source, available);
				final initializePlaceType = knownPlaceType(place, available, locals);
				if (initializedType != null && initializePlaceType != null && typeKey(initializedType) != typeKey(initializePlaceType)) {
					add(path, "initializer value type does not match its place type", instruction.source);
				}
				validateTransition(from, to, '$path.transition', instruction.source);
				if (to != IRISInitialized) {
					add(path, "initialize instruction must end in initialized state", instruction.source);
				}
			case IRIOInitializeFixedArray(place, values, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				final arrayType = knownPlaceType(place, available, locals);
				switch arrayType {
					case IRTFixedArray(element, length, _):
						if (values.length != length) {
							add(path, 'fixed-array initializer provides ${values.length} values for length $length', instruction.source);
						}
						for (index => valueId in values) {
							final valueType = requireValue(valueId, '$path.value:$index', instruction.source, available);
							if (valueType != null && typeKey(valueType) != typeKey(element)) {
								add(path, 'fixed-array initializer value $index does not match the element type', instruction.source);
							}
						}
					case _:
						add(path, "fixed-array initialization requires a fixed-array place", instruction.source);
				}
				validateInitializeTransition(from, to, path, instruction.source);
			case IRIOInitializeSpan(place, sourceArray, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validatePlace(sourceArray, '$path.sourceArray', instruction.source, available, locals);
				final spanType = knownPlaceType(place, available, locals);
				final sourceType = knownPlaceType(sourceArray, available, locals);
				switch [spanType, sourceType] {
					case [IRTSpan(spanElement, _), IRTFixedArray(arrayElement, _, _)] if (typeKey(spanElement) == typeKey(arrayElement)):
					case [IRTSpan(_, _), IRTFixedArray(_, _, _)]:
						add(path, "span element type does not match its fixed-array source", instruction.source);
					case _:
						add(path, "span initialization requires a span place and fixed-array source", instruction.source);
				}
				validateInitializeTransition(from, to, path, instruction.source);
			case IRIOBoundsCheck(collection, indexValueId, policy):
				validatePlace(collection, '$path.collection', instruction.source, available, locals);
				switch knownPlaceType(collection, available, locals) {
					case IRTFixedArray(_, _, _) | IRTSpan(_, _):
					case _:
						add(path, "bounds check requires a fixed array or span", instruction.source);
				}
				final indexType = requireValue(indexValueId, '$path.index', instruction.source, available);
				final expectedIndexType = switch policy {
					case IRBPLoopGuarded(_, _, _): "abi:size";
					case IRBPCheckedAbort(_, _) | IRBPStaticProof(_, _): "i32";
				};
				if (indexType != null && typeKey(indexType) != expectedIndexType) {
					add(path,
						expectedIndexType == "abi:size" ? "compiler span-loop index must have size_t representation" : "safe source index must have Haxe Int representation",
						instruction.source);
				}
				validateBoundsPolicy(policy, '$path.policy', instruction.source, collection, indexValueId, instruction.id, block, locals, instructionSites,
					valueSites);
				final proofKey = collectionProofKey(collection, indexValueId);
				if (proofKey != null) {
					boundsProofs.set(proofKey, true);
				}
			case IRIOLifetime(place, from, to, reason):
				validatePlace(place, '$path.place', instruction.source, available, locals);
				validateTransition(from, to, '$path.transition', instruction.source);
				validateText(reason, '$path.reason', instruction.source);
		}
	}

	function instructionProducesValue(kind:HxcIRInstructionKind):Bool {
		return switch kind {
			case IRIOConstant(_) | IRIOLoad(_) | IRIOAddress(_) | IRIOUnary(_, _, _) | IRIOBinary(_, _, _, _) | IRIOConvert(_, _, _, _, _) |
				IRIOConstructAggregate(_, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _) | IRIOProjectTag(_, _, _, _) |
				IRIOAllocate(_, _, _, _):
				true;
			case IRIOCall(call):
				call.returnType != IRTVoid;
			case IRIOSequence(_) | IRIOStore(_, _) | IRIODeallocate(_, _) | IRIORetain(_, _) | IRIOTrace(_, _) | IRIOInitialize(_, _, _, _) |
				IRIOInitializeFixedArray(_, _, _, _) | IRIOInitializeSpan(_, _, _, _) | IRIOBoundsCheck(_, _, _) | IRIOLifetime(_, _, _, _):
				false;
		}
	}

	function validateCall(call:HxcIRCall, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>, blocks:Map<String, HxcIRBlock>,
			regions:Map<String, HxcIRCleanupRegion>):Void {
		validateTypeRef(call.returnType, '$path.returnType', source, true);
		final argumentTypes:Array<Null<HxcIRTypeRef>> = [];
		for (index => argument in call.arguments) {
			argumentTypes.push(requireValue(argument, '$path.argument:$index', source, available));
		}
		switch call.dispatch {
			case IRCDDirect(functionId):
				final target = functions.get(functionId);
				if (target == null) {
					add(path, 'direct call refers to unknown function `$functionId`', source);
				} else {
					validateKnownCallSignature(call, argumentTypes, target.parameters, target.returnType, path, source);
				}
			case IRCDVirtual(slotId, receiverValueId):
				validateStableId(slotId, '$path.virtualSlot', source);
				requireValue(receiverValueId, '$path.receiver', source, available);
			case IRCDInterface(interfaceTypeId, slotId, receiverValueId):
				requireInstance(interfaceTypeId, path, source);
				validateStableId(slotId, '$path.interfaceSlot', source);
				requireValue(receiverValueId, '$path.receiver', source, available);
			case IRCDClosure(callableValueId):
				final callableType = requireValue(callableValueId, '$path.callable', source, available);
				if (callableType != null) {
					switch callableType {
						case IRTFunction(parameters, result):
							final signatureParameters:Array<HxcIRParameter> = [];
							for (index => parameter in parameters) {
								signatureParameters.push({id: 'closure.argument.$index', type: parameter, source: source});
							}
							validateKnownCallSignature(call, argumentTypes, signatureParameters, result, path, source);
						case _:
							add(path, 'closure call value `$callableValueId` does not have a function type', source);
					}
				}
			case IRCDNative(symbol):
				validateStableId(symbol, '$path.nativeSymbol', source);
			case IRCDRuntime(featureId, operationId):
				validateStableId(featureId, '$path.runtimeFeature', source);
				validateStableId(operationId, '$path.runtimeOperation', source);
				if (featureId == "io") {
					validateHostedOutputCall(call, argumentTypes, path, source);
				}
			case IRCDIntrinsic(intrinsicId):
				validateStableId(intrinsicId, '$path.intrinsic', source);
		}
		if (call.failure != null) {
			validateFailureEdge(call.failure, '$path.failure', source, available, blocks, regions);
		}
	}

	function validateHostedOutputCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("io", value): value;
			case _: return;
		};
		if (operationId != "sys-println-literal" && operationId != "trace-literal") {
			add(path, 'io runtime call names unsupported operation `$operationId`', source);
		}
		if (call.returnType != IRTVoid || argumentTypes.length != 1 || argumentTypes[0] != IRTString) {
			add(path, "literal hosted output requires exactly one UTF-8 String argument and a Void semantic result", source);
		}
		final failure = call.failure;
		if (failure == null) {
			add(path, "hosted output requires an explicit native-status failure edge", source);
			return;
		}
		if (failure.kind != IRFNativeStatus || failure.target != IRFTAbort || failure.arguments.length != 0 || failure.cleanup.length != 0) {
			add(path, "the admitted hosted output policy requires a cleanup-free native-status abort edge", source);
		}
	}

	function validateKnownCallSignature(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, parameters:Array<HxcIRParameter>, returnType:HxcIRTypeRef,
			path:String, source:HxcSourceSpan):Void {
		if (argumentTypes.length != parameters.length) {
			add(path, 'call provides ${argumentTypes.length} arguments for ${parameters.length} parameters', source);
		}
		for (index => argumentType in argumentTypes) {
			if (argumentType != null && index < parameters.length && typeKey(argumentType) != typeKey(parameters[index].type)) {
				add(path, 'call argument $index type does not match parameter `${parameters[index].id}`', source);
			}
		}
		if (typeKey(call.returnType) != typeKey(returnType)) {
			add(path, "call return type does not match its known target signature", source);
		}
	}

	function validateTerminator(fn:HxcIRFunction, kind:HxcIRTerminatorKind, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>,
			blocks:Map<String, HxcIRBlock>, regions:Map<String, HxcIRCleanupRegion>):Void {
		switch kind {
			case IRTJump(edge):
				validateBlockEdge(edge, path, source, available, blocks, regions);
			case IRTBranch(conditionValueId, whenTrue, whenFalse):
				final conditionType = requireValue(conditionValueId, '$path.condition', source, available);
				if (conditionType != null && typeKey(conditionType) != "bool") {
					add(path, "branch condition must have Bool type", source);
				}
				validateBlockEdge(whenTrue, '$path.true', source, available, blocks, regions);
				validateBlockEdge(whenFalse, '$path.false', source, available, blocks, regions);
			case IRTSwitch(valueId, cases, defaultEdge):
				final switchType = requireValue(valueId, '$path.value', source, available);
				final values:Map<String, Bool> = [];
				for (index => item in cases) {
					validateConstant(item.value, '$path.case:$index.value', source);
					if (switchType != null && !constantMatchesType(item.value, switchType)) {
						add(path, 'switch case $index literal family does not match its subject type', source);
					}
					final key = constantKey(item.value);
					if (values.exists(key)) {
						add(path, 'switch repeats case value `$key`', source);
					} else {
						values.set(key, true);
					}
					validateBlockEdge(item.edge, '$path.case:$index.edge', source, available, blocks, regions);
				}
				validateBlockEdge(defaultEdge, '$path.default', source, available, blocks, regions);
			case IRTTagSwitch(valueId, cases, defaultEdge):
				final switchType = requireValue(valueId, '$path.value', source, available);
				final expectedCases = switchType == null ? null : taggedUnionCasesForType(switchType, path, source);
				final names:Map<String, Bool> = [];
				for (index => item in cases) {
					validateStableId(item.tagName, '$path.case:$index.tag', source);
					if (names.exists(item.tagName)) {
						add(path, 'tag switch repeats case `${item.tagName}`', source);
					} else {
						names.set(item.tagName, true);
					}
					if (expectedCases != null && findTagCase(expectedCases, item.tagName) == null) {
						add(path, 'tag switch names unknown case `${item.tagName}`', source);
					}
					validateBlockEdge(item.edge, '$path.case:$index.edge', source, available, blocks, regions);
				}
				var exhaustive = expectedCases != null;
				if (expectedCases != null) {
					for (tagCase in expectedCases) {
						if (!names.exists(tagCase.name)) {
							exhaustive = false;
							if (defaultEdge == null) {
								add(path, 'tag switch without a default omits `${tagCase.name}`', source);
							}
						}
					}
				}
				if (defaultEdge != null) {
					if (exhaustive) {
						add(path, "exhaustive tag switch must not carry a default edge", source);
					}
					validateBlockEdge(defaultEdge, '$path.default', source, available, blocks, regions);
				}
			case IRTReturn(valueId, cleanup):
				if (fn.returnType == IRTVoid) {
					if (valueId != null) {
						add(path, "void function return carries a value", source);
					}
				} else if (valueId == null) {
					add(path, "non-void function return omits its value", source);
				} else {
					final valueType = requireValue(valueId, '$path.value', source, available);
					if (valueType != null && typeKey(valueType) != typeKey(fn.returnType)) {
						add(path, "return value type does not match the function return type", source);
					}
				}
				validateCleanupPath(cleanup, '$path.cleanup', source, regions);
			case IRTThrow(valueId, edge):
				requireValue(valueId, '$path.value', source, available);
				validateFailureEdge(edge, '$path.failure', source, available, blocks, regions);
				if (edge.kind != IRFException) {
					add(path, "throw terminator must use an exception failure edge", source);
				}
			case IRTUnreachable:
		}
	}

	function validateBlockEdge(edge:HxcIRBlockEdge, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>, blocks:Map<String, HxcIRBlock>,
			regions:Map<String, HxcIRCleanupRegion>):Void {
		final target = blocks.get(edge.targetBlockId);
		if (target == null) {
			add(path, 'control edge targets unknown block `${edge.targetBlockId}`', source);
		} else {
			validateEdgeArguments(edge.arguments, target.parameters, path, source, available);
		}
		validateCleanupPath(edge.cleanup, '$path.cleanup', source, regions);
	}

	function validateFailureEdge(edge:HxcIRFailureEdge, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>,
			blocks:Map<String, HxcIRBlock>, regions:Map<String, HxcIRCleanupRegion>):Void {
		switch edge.target {
			case IRFTBlock(blockId):
				final target = blocks.get(blockId);
				if (target == null) {
					add(path, 'failure edge targets unknown block `$blockId`', source);
				} else {
					validateEdgeArguments(edge.arguments, target.parameters, path, source, available);
				}
			case IRFTPropagate | IRFTAbort:
				if (edge.arguments.length > 0) {
					add(path, "propagate/abort failure edge cannot carry block arguments", source);
				}
		}
		validateCleanupPath(edge.cleanup, '$path.cleanup', source, regions);
	}

	function validateEdgeArguments(arguments:Array<String>, parameters:Array<HxcIRParameter>, path:String, source:HxcSourceSpan,
			available:Map<String, HxcIRTypeRef>):Void {
		if (arguments.length != parameters.length) {
			add(path, 'edge provides ${arguments.length} arguments for ${parameters.length} block parameters', source);
		}
		for (index => argument in arguments) {
			final argumentType = requireValue(argument, '$path.argument:$index', source, available);
			if (argumentType != null && index < parameters.length && typeKey(argumentType) != typeKey(parameters[index].type)) {
				add(path, 'edge argument $index type does not match block parameter `${parameters[index].id}`', source);
			}
		}
	}

	function validateCleanupPath(steps:Array<HxcIRCleanupStep>, path:String, source:HxcSourceSpan, regions:Map<String, HxcIRCleanupRegion>):Void {
		final seen:Map<String, Bool> = [];
		var previousRegion:Null<HxcIRCleanupRegion> = null;
		var previousActionIndex = -1;
		for (index => step in steps) {
			final stepPath = '$path:$index';
			final region = regions.get(step.regionId);
			if (region == null) {
				add(stepPath, 'cleanup step refers to unknown region `${step.regionId}`', source);
				continue;
			}
			var actionIndex = -1;
			for (candidateIndex => action in region.actions) {
				if (action.id == step.actionId) {
					actionIndex = candidateIndex;
					break;
				}
			}
			if (actionIndex == -1) {
				add(stepPath, 'cleanup step refers to unknown action `${step.actionId}` in region `${step.regionId}`', source);
				continue;
			}
			final key = '${step.regionId}:${step.actionId}';
			if (seen.exists(key)) {
				add(stepPath, 'cleanup action `$key` appears more than once on one edge', source);
			} else {
				seen.set(key, true);
			}

			if (previousRegion != null) {
				if (previousRegion.id == region.id) {
					if (actionIndex >= previousActionIndex) {
						add(stepPath, 'cleanup actions in region `${region.id}` must execute in reverse registration order', source);
					}
				} else if (previousRegion.parentId != region.id) {
					add(stepPath, 'cleanup path must move from inner region `${previousRegion.id}` to its parent, not `${region.id}`', source);
				}
			}
			previousRegion = region;
			previousActionIndex = actionIndex;
		}
	}

	function validatePlace(place:HxcIRPlace, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>):Void {
		switch place {
			case IRPLocal(localId):
				if (!locals.exists(localId)) {
					add(path, 'place refers to unknown local `$localId`', source);
				}
			case IRPGlobal(globalId):
				if (!globals.exists(globalId)) {
					add(path, 'place refers to unknown global `$globalId`', source);
				}
			case IRPDereference(pointerValueId):
				requireValue(pointerValueId, '$path.pointer', source, available);
			case IRPField(base, fieldName):
				validatePlace(base, '$path.base', source, available, locals);
				validateStableId(fieldName, '$path.field', source);
				final baseType = knownPlaceType(base, available, locals);
				if (baseType != null) {
					switch baseType {
						case IRTInstance(instanceId):
							final fields = directAggregateFields(instanceId, path, source);
							if (fields != null && findAggregateField(fields, fieldName) == null) {
								add(path, 'aggregate place names unknown field `$fieldName`', source);
							}
						case _:
							add(path, "field place requires a direct aggregate instance base", source);
					}
				}
			case IRPIndex(base, indexValueId):
				validatePlace(base, '$path.base', source, available, locals);
				final indexType = requireValue(indexValueId, '$path.index', source, available);
				if (indexType != null && !isInteger(indexType)) {
					add(path, "index place requires an integer value", source);
				}
		}
	}

	function knownPlaceType(place:HxcIRPlace, available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>):Null<HxcIRTypeRef> {
		return switch place {
			case IRPLocal(localId):
				final local = locals.get(localId);
				local == null ? null : local.type;
			case IRPGlobal(globalId):
				final global = globals.get(globalId);
				global == null ? null : global.type;
			case IRPDereference(pointerValueId):
				switch available.get(pointerValueId) {
					case IRTPointer(pointee, _): pointee;
					case _: null;
				}
			case IRPField(base, fieldName):
				final baseType = knownPlaceType(base, available, locals);
				baseType == null ? null : aggregateFieldType(baseType, fieldName);
			case IRPIndex(base, _):
				switch knownPlaceType(base, available, locals) {
					case IRTFixedArray(element, _, _) | IRTSpan(element, _): element;
					case _: null;
				}
		};
	}

	function validateStableCleanupPlace(place:HxcIRPlace, path:String, source:HxcSourceSpan, locals:Map<String, HxcIRLocal>):Void {
		switch place {
			case IRPLocal(localId):
				if (!locals.exists(localId)) {
					add(path, 'cleanup place refers to unknown local `$localId`', source);
				}
			case IRPGlobal(globalId):
				if (!globals.exists(globalId)) {
					add(path, 'cleanup place refers to unknown global `$globalId`', source);
				}
			case IRPField(base, fieldName):
				validateStableCleanupPlace(base, '$path.base', source, locals);
				validateStableId(fieldName, '$path.field', source);
			case IRPDereference(_) | IRPIndex(_, _):
				add(path, "cleanup actions require a stable local/global place; materialize dereference/index storage in a local first", source);
		}
	}

	function requireValue(id:String, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>):Null<HxcIRTypeRef> {
		if (!available.exists(id)) {
			add(path, 'value `$id` is not available at this instruction; preserve order or pass it through a block parameter', source);
			return null;
		}
		return available.get(id);
	}

	function requireInstance(id:String, path:String, source:HxcSourceSpan):Void {
		if (!typeInstances.exists(id)) {
			add(path, 'operation refers to unknown type instance `$id`', source);
		}
	}

	function requireTagCase(instanceId:String, tagName:String, path:String, source:HxcSourceSpan):Null<HxcIRTagCase> {
		validateStableId(tagName, '$path.tag', source);
		final cases = taggedUnionCases(instanceId, path, source);
		if (cases == null) {
			return null;
		}
		final tagCase = findTagCase(cases, tagName);
		if (tagCase == null) {
			add(path, 'tag operation names unknown case `$tagName` for instance `$instanceId`', source);
		}
		return tagCase;
	}

	function requireTagCaseForType(type:HxcIRTypeRef, tagName:String, path:String, source:HxcSourceSpan):Null<HxcIRTagCase> {
		return switch type {
			case IRTInstance(instanceId): requireTagCase(instanceId, tagName, path, source);
			case _:
				add(path, "tag operation requires a tagged-union instance value", source);
				null;
		};
	}

	function taggedUnionCasesForType(type:HxcIRTypeRef, path:String, source:HxcSourceSpan):Null<Array<HxcIRTagCase>> {
		return switch type {
			case IRTInstance(instanceId): taggedUnionCases(instanceId, path, source);
			case _:
				add(path, "tag switch requires a tagged-union instance value", source);
				null;
		};
	}

	function taggedUnionCases(instanceId:String, path:String, source:HxcSourceSpan):Null<Array<HxcIRTagCase>> {
		final instance = typeInstances.get(instanceId);
		if (instance == null) {
			add(path, 'tag operation refers to unknown instance `$instanceId`', source);
			return null;
		}
		switch instance.representation {
			case IRRDirect | IRRTagged:
			case IRROpaqueHandle | IRRManaged(_):
				add(path, 'tag operation requires direct or tagged representation for instance `$instanceId`', source);
				return null;
		}
		final declaration = typeDeclarations.get(instance.declarationId);
		if (declaration == null) {
			return null;
		}
		return switch declaration.kind {
			case IRTKTaggedUnion(cases):
				cases;
			case _:
				add(path, 'tag operation requires a tagged-union declaration for instance `$instanceId`', source);
				null;
		};
	}

	static function findTagCase(cases:Array<HxcIRTagCase>, tagName:String):Null<HxcIRTagCase> {
		for (tagCase in cases) {
			if (tagCase.name == tagName) {
				return tagCase;
			}
		}
		return null;
	}

	function validateTagCheck(check:HxcIRTagCheckPolicy, path:String, source:HxcSourceSpan):Void {
		switch check {
			case IRTCPCheckedAbort(policyProfile, buildMode):
				if (policyProfile != "portable" && policyProfile != "metal") {
					add(path, 'tag-check policy has unknown profile `$policyProfile`', source);
				} else if (policyProfile != profile) {
					add(path, 'tag-check policy profile `$policyProfile` does not match validator profile `$profile`', source);
				}
				if (buildMode != "debug" && buildMode != "release" && buildMode != "minsizerel") {
					add(path, 'tag-check policy has unknown build mode `$buildMode`', source);
				}
		}
	}

	function directAggregateFields(instanceId:String, path:String, source:HxcSourceSpan):Null<Array<HxcIRTypeField>> {
		final instance = typeInstances.get(instanceId);
		if (instance == null) {
			return null;
		}
		if (instance.representation != IRRDirect) {
			add(path, 'aggregate operation requires direct representation for instance `$instanceId`', source);
			return null;
		}
		if (instance.arguments.length != 0) {
			add(path, 'direct aggregate instance `$instanceId` must be specialized before aggregate operations', source);
			return null;
		}
		final declaration = typeDeclarations.get(instance.declarationId);
		if (declaration == null) {
			return null;
		}
		return switch declaration.kind {
			case IRTKAggregate(fields): fields;
			case _:
				add(path, 'aggregate operation requires an aggregate declaration for instance `$instanceId`', source);
				null;
		};
	}

	function aggregateFieldType(type:HxcIRTypeRef, fieldName:String):Null<HxcIRTypeRef> {
		return switch type {
			case IRTInstance(instanceId):
				final instance = typeInstances.get(instanceId);
				final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
				if (instance == null || instance.representation != IRRDirect || instance.arguments.length != 0 || declaration == null) {
					null;
				} else {
					switch declaration.kind {
						case IRTKAggregate(fields):
							final field = findAggregateField(fields, fieldName);
							field == null ? null : field.type;
						case _: null;
					}
				}
			case _: null;
		};
	}

	static function findAggregateField(fields:Array<HxcIRTypeField>, fieldName:String):Null<HxcIRTypeField> {
		for (field in fields) {
			if (field.name == fieldName) {
				return field;
			}
		}
		return null;
	}

	function validateFiniteDirectLayouts():Void {
		final state:Map<String, Int> = [];
		for (instance in sortedMapValues(typeInstances, item -> item.id)) {
			validateFiniteDirectLayout(instance, state, []);
		}
	}

	function validateFiniteDirectLayout(instance:HxcIRTypeInstance, state:Map<String, Int>, stack:Array<String>):Void {
		final existing = state.get(instance.id);
		if (existing == 2) {
			return;
		}
		if (existing == 1) {
			final cycle = stack.concat([instance.id]);
			add('type-layout:${instance.id}',
				'direct by-value type layout is recursive: ${cycle.join(" -> ")}; insert an explicit pointer or managed boundary', instance.source);
			return;
		}
		state.set(instance.id, 1);
		final nextStack = stack.concat([instance.id]);
		for (dependencyId in directLayoutDependencies(instance)) {
			final dependency = typeInstances.get(dependencyId);
			if (dependency != null) {
				validateFiniteDirectLayout(dependency, state, nextStack);
			}
		}
		state.set(instance.id, 2);
	}

	function directLayoutDependencies(instance:HxcIRTypeInstance):Array<String> {
		final result:Array<String> = [];
		switch instance.representation {
			case IRRDirect | IRRTagged:
				final declaration = typeDeclarations.get(instance.declarationId);
				if (declaration != null) {
					switch declaration.kind {
						case IRTKAggregate(fields):
							for (field in fields) {
								collectDirectLayoutDependencies(field.type, result);
							}
						case IRTKTaggedUnion(cases):
							for (tagCase in cases) {
								for (payload in tagCase.payload) {
									collectDirectLayoutDependencies(payload.type, result);
								}
							}
						case IRTKPrimitive | IRTKReference | IRTKFunction | IRTKExtern:
					}
				}
			case IRROpaqueHandle | IRRManaged(_):
		}
		result.sort(compareUtf8);
		return result;
	}

	function collectDirectLayoutDependencies(type:HxcIRTypeRef, result:Array<String>):Void {
		switch type {
			case IRTInstance(instanceId):
				if (result.indexOf(instanceId) == -1) {
					result.push(instanceId);
				}
			case IRTNullable(inner, IRNTagged) | IRTFixedArray(inner, _, _):
				collectDirectLayoutDependencies(inner, result);
			case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTVoid | IRTPointer(_, _) | IRTNullable(_, IRNPointer) |
				IRTFunction(_, _) | IRTSpan(_, _) | IRTDynamic:
		}
	}

	function validateImplementation(implementation:HxcIRImplementation, path:String, source:HxcSourceSpan):Void {
		switch implementation {
			case IRIStatic:
			case IRIProgramLocal(helperId):
				validateStableId(helperId, '$path.helper', source);
			case IRIRuntime(featureId):
				validateStableId(featureId, '$path.runtimeFeature', source);
		}
	}

	function validateConversion(kind:HxcIRConversionKind, sourceType:HxcIRTypeRef, targetType:HxcIRTypeRef, implementation:HxcIRImplementation,
			failure:Null<HxcIRFailureEdge>, path:String, source:HxcSourceSpan):Void {
		final requiresFailure = kind == IRCNullableUnwrap || kind == IRCNumericChecked;
		if (requiresFailure && failure == null) {
			add(path, "checked or nullable-unwrapping conversion requires an explicit failure edge", source);
		} else if (!requiresFailure && failure != null && isPrimitiveConversion(kind)) {
			add(path, "non-failing primitive conversion must not carry a failure edge", source);
		}

		if (isPrimitiveConversion(kind)) {
			switch implementation {
				case IRIRuntime(featureId):
					add(path, 'primitive conversion `$kind` must use direct C or a program-local helper, not runtime feature `$featureId`', source);
				case IRIStatic | IRIProgramLocal(_):
			}
		}

		switch kind {
			case IRCNumericExact:
				if (!isNumeric(sourceType) || !isNumeric(targetType)) {
					add(path, "numeric-exact conversion requires numeric source and target types", source);
				}
			case IRCNumericWrapping:
				if (!isInteger(sourceType) || !isInteger(targetType)) {
					add(path, "numeric-wrapping conversion requires integer source and target types", source);
				}
			case IRCNumericSaturating:
				if (!isFloat(sourceType) || !isInteger(targetType)) {
					add(path, "numeric-saturating conversion requires a floating source and integer target", source);
				}
			case IRCNumericChecked:
				if (!isNumeric(sourceType) || !isInteger(targetType)) {
					add(path, "numeric-checked conversion requires a numeric source and integer target", source);
				}
			case IRCNullableInject:
				if (!isNullablePair(sourceType, targetType)) {
					add(path, "nullable injection target must be the nullable representation of its source type", source);
				}
			case IRCNullableUnwrap:
				if (!isNullablePair(targetType, sourceType)) {
					add(path, "nullable unwrap target must match the nullable payload type", source);
				}
			case IRCPointer | IRCBox | IRCUnbox | IRCRepresentation:
		}
	}

	static function isPrimitiveConversion(kind:HxcIRConversionKind):Bool {
		return switch kind {
			case IRCNumericExact | IRCNumericWrapping | IRCNumericSaturating | IRCNumericChecked | IRCNullableInject | IRCNullableUnwrap:
				true;
			case IRCPointer | IRCBox | IRCUnbox | IRCRepresentation:
				false;
		}
	}

	static function isNumeric(type:HxcIRTypeRef):Bool
		return isInteger(type) || isFloat(type);

	static function isInteger(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTInt(_, _) | IRTAbiInteger(_): true;
			case _: false;
		}
	}

	static function isFloat(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTFloat(_): true;
			case _: false;
		}
	}

	static function isNullablePair(value:HxcIRTypeRef, nullable:HxcIRTypeRef):Bool {
		return switch nullable {
			case IRTNullable(inner, _): typeKey(value) == typeKey(inner);
			case IRTPointer(pointee, true):
				switch value {
					case IRTPointer(valuePointee, false): typeKey(valuePointee) == typeKey(pointee);
					case _: false;
				}
			case _: false;
		}
	}

	function validateTransition(from:HxcIRInitializationState, to:HxcIRInitializationState, path:String, source:HxcSourceSpan):Void {
		final valid = switch [from, to] {
			case [IRISUninitialized, IRISInitializing] | [IRISUninitialized, IRISInitialized] | [IRISInitializing, IRISInitialized] |
				[IRISInitializing, IRISDestroyed] | [IRISInitialized, IRISMoved] | [IRISInitialized, IRISDestroyed] | [IRISMoved, IRISInitialized] |
				[IRISMoved, IRISDestroyed] | [IRISDestroyed, IRISInitialized]:
				true;
			case _:
				false;
		}
		if (!valid) {
			add(path, 'invalid lifetime transition ${stateName(from)} -> ${stateName(to)}', source);
		}
	}

	function validateInitializeTransition(from:HxcIRInitializationState, to:HxcIRInitializationState, path:String, source:HxcSourceSpan):Void {
		validateTransition(from, to, '$path.transition', source);
		if (to != IRISInitialized) {
			add(path, "initialize instruction must end in initialized state", source);
		}
	}

	function validateCollectionAccessProof(place:HxcIRPlace, boundsProofs:Map<String, Bool>, path:String, source:HxcSourceSpan,
			available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>):Void {
		switch place {
			case IRPIndex(base, indexValueId):
				switch knownPlaceType(base, available, locals) {
					case IRTFixedArray(_, _, _) | IRTSpan(_, _):
						final key = collectionProofKey(base, indexValueId);
						if (key == null || !boundsProofs.exists(key)) {
							add(path, "fixed-array/span access requires a preceding bounds policy for the same collection and index", source);
						}
					case _:
				}
				validateCollectionAccessProof(base, boundsProofs, path, source, available, locals);
			case IRPField(base, _):
				validateCollectionAccessProof(base, boundsProofs, path, source, available, locals);
			case IRPLocal(_) | IRPGlobal(_) | IRPDereference(_):
		}
	}

	static function collectionProofKey(collection:HxcIRPlace, indexValueId:String):Null<String> {
		return switch collection {
			case IRPLocal(localId): 'local:$localId\x00$indexValueId';
			case IRPGlobal(globalId): 'global:$globalId\x00$indexValueId';
			case _: null;
		};
	}

	function validateBoundsPolicy(policy:HxcIRBoundsPolicy, path:String, source:HxcSourceSpan, collection:HxcIRPlace, indexValueId:String,
			boundsInstructionId:String, currentBlock:HxcIRBlock, locals:Map<String, HxcIRLocal>, instructionSites:Map<String, HxcIRInstructionSite>,
			valueSites:Map<String, HxcIRInstructionSite>):Void {
		final knownLength = knownCollectionLength(collection, locals, instructionSites);
		switch policy {
			case IRBPCheckedAbort(policyProfile, buildMode):
				if (policyProfile != "portable" && policyProfile != "metal") {
					add(path, 'bounds policy has unknown profile `$policyProfile`', source);
				} else if (policyProfile != profile) {
					add(path, 'bounds policy profile `$policyProfile` does not match validator profile `$profile`', source);
				}
				if (buildMode != "debug" && buildMode != "release" && buildMode != "minsizerel") {
					add(path, 'bounds policy has unknown build mode `$buildMode`', source);
				}
			case IRBPStaticProof(length, index):
				if (length <= 0 || index < 0 || index >= length) {
					add(path, 'invalid static bounds proof for index $index and length $length', source);
				}
				if (knownLength == null) {
					add(path, "static bounds proof cannot establish the collection's compiler-owned length", source);
				} else if (knownLength != length) {
					add(path, 'static bounds proof length $length does not match collection length $knownLength', source);
				}
				if (!isIntegerConstant(valueSites.get(indexValueId), index, currentBlock.id, "i32")) {
					add(path, "static bounds proof index must be the exact in-block Haxe Int constant it claims", source);
				}
			case IRBPLoopGuarded(guardInstructionId, indexLocalId, length):
				validateStableId(guardInstructionId, '$path.guardInstruction', source);
				validateStableId(indexLocalId, '$path.indexLocal', source);
				if (length <= 0) {
					add(path, "loop-guarded bounds proof requires a positive length", source);
				}
				if (knownLength == null) {
					add(path, "loop bounds proof cannot establish the collection's compiler-owned length", source);
				} else if (knownLength != length) {
					add(path, 'loop bounds length $length does not match collection length $knownLength', source);
				}
				final indexLocal = locals.get(indexLocalId);
				if (indexLocal == null || typeKey(indexLocal.type) != "abi:size") {
					add(path, "loop bounds proof index local must have size_t representation", source);
				}
				final guardSite = instructionSites.get(guardInstructionId);
				if (guardSite == null) {
					add(path, 'loop bounds proof refers to unknown guard instruction `$guardInstructionId`', source);
				} else {
					final guardResult = guardSite.instruction.result;
					final shapeValid = switch guardSite.instruction.kind {
						case IRIOBinary("hxc.size.less.span-index", leftValueId, rightValueId, IRIStatic): isLocalLoad(valueSites.get(leftValueId),
								indexLocalId, guardSite.block.id,
								"abi:size") && isIntegerConstant(valueSites.get(rightValueId), length, guardSite.block.id, "abi:size");
						case _: false;
					};
					if (guardResult == null || typeKey(guardResult.type) != "bool" || !shapeValid) {
						add(path, "loop bounds proof guard must be the exact static size_t index-local < length comparison", source);
					} else {
						final branchValid = guardSite.block.terminator != null && switch guardSite.block.terminator.kind {
							case IRTBranch(conditionValueId, whenTrue, _): conditionValueId == guardResult.id && whenTrue.targetBlockId == currentBlock.id;
							case _: false;
						};
						if (!branchValid) {
							add(path, "loop bounds proof guard must branch directly to the checked body on true", source);
						}
					}
				}
				final indexSite = valueSites.get(indexValueId);
				if (!isLocalLoad(indexSite, indexLocalId, currentBlock.id, "abi:size")
					|| !immediatelyPrecedes(indexSite, boundsInstructionId, currentBlock)) {
					add(path, "loop bounds proof index must immediately reload the guarded size_t local in the checked body", source);
				}
		}
	}

	static function knownCollectionLength(collection:HxcIRPlace, locals:Map<String, HxcIRLocal>, instructionSites:Map<String, HxcIRInstructionSite>):Null<Int> {
		return switch collection {
			case IRPLocal(localId):
				final local = locals.get(localId);
				if (local == null) {
					null;
				} else {
					switch local.type {
						case IRTFixedArray(_, length, _): length;
						case IRTSpan(_, _):
							var sourceLength:Null<Int> = null;
							var initializerCount = 0;
							for (site in instructionSites) {
								switch site.instruction.kind {
									case IRIOInitializeSpan(IRPLocal(targetId), IRPLocal(sourceId), _, _) if (targetId == localId):
										initializerCount++;
										final sourceLocal = locals.get(sourceId);
										sourceLength = sourceLocal == null ? null : switch sourceLocal.type {
											case IRTFixedArray(_, length, _): length;
											case _: null;
										};
									case _:
								}
							}
							initializerCount == 1 ? sourceLength : null;
						case _: null;
					}
				}
			case IRPGlobal(_) | IRPField(_, _) | IRPIndex(_, _) | IRPDereference(_): null;
		};
	}

	static function immediatelyPrecedes(site:Null<HxcIRInstructionSite>, instructionId:String, block:HxcIRBlock):Bool {
		if (site == null || site.block.id != block.id) {
			return false;
		}
		for (index in 1...block.instructions.length) {
			if (block.instructions[index - 1].id == site.instruction.id && block.instructions[index].id == instructionId) {
				return true;
			}
		}
		return false;
	}

	static function isLocalLoad(site:Null<HxcIRInstructionSite>, localId:String, blockId:String, expectedType:String):Bool {
		return site != null && site.block.id == blockId && switch site.instruction.kind {
			case IRIOLoad(IRPLocal(loadedLocalId)): site.instruction.result != null && loadedLocalId == localId && typeKey(site.instruction.result.type) == expectedType;
			case _: false;
		};
	}

	static function isIntegerConstant(site:Null<HxcIRInstructionSite>, expected:Int, blockId:String, expectedType:String):Bool {
		return site != null && site.block.id == blockId && switch site.instruction.kind {
			case IRIOConstant(IRCInt(value)): site.instruction.result != null && typeKey(site.instruction.result.type) == expectedType && Std.parseInt(value) == expected;
			case _: false;
		};
	}

	static function isRootPlace(place:HxcIRPlace):Bool {
		return switch place {
			case IRPLocal(_) | IRPGlobal(_): true;
			case IRPField(_, _) | IRPIndex(_, _) | IRPDereference(_): false;
		};
	}

	static function isCollectionType(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTFixedArray(_, _, _) | IRTSpan(_, _): true;
			case _: false;
		};
	}

	function validateTypeRef(type:HxcIRTypeRef, path:String, source:HxcSourceSpan, allowVoid:Bool):Void {
		switch type {
			case IRTBool | IRTString | IRTDynamic:
			case IRTInt(width, _):
				if (width != 8 && width != 16 && width != 32 && width != 64) {
					add(path, 'integer width $width is unsupported; expected 8, 16, 32, or 64', source);
				}
			case IRTAbiInteger(_):
			case IRTFloat(width):
				if (width != 32 && width != 64) {
					add(path, 'floating width $width is unsupported; expected 32 or 64', source);
				}
			case IRTVoid:
				if (!allowVoid) {
					add(path, "Void is not a storable value type", source);
				}
			case IRTInstance(instanceId):
				if (!typeInstances.exists(instanceId)) {
					add(path, 'type refers to unknown instance `$instanceId`', source);
				}
			case IRTPointer(pointee, _):
				validateTypeRef(pointee, '$path.pointee', source, true);
			case IRTNullable(inner, representation):
				validateTypeRef(inner, '$path.value', source, false);
				switch inner {
					case IRTNullable(_, _):
						add(path, "nested nullable values must be canonicalized to one nullable layer", source);
					case IRTVoid | IRTFunction(_, _) | IRTDynamic:
						add(path, "Void, function, and Dynamic types cannot use a primitive nullable representation", source);
					case _:
				}
				switch representation {
					case IRNTagged:
						switch inner {
							case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_):
							case _:
								add(path, "tagged primitive nullability requires a scalar payload", source);
						}
					case IRNPointer:
						switch inner {
							case IRTInstance(_):
							case _:
								add(path,
									"pointer nullable wrappers require a reference instance; native pointers encode nullability directly on their pointer type",
									source);
						}
				}
			case IRTFunction(parameters, result):
				for (index => parameter in parameters) {
					validateTypeRef(parameter, '$path.parameter:$index', source, false);
				}
				validateTypeRef(result, '$path.result', source, true);
			case IRTFixedArray(element, length, witnessId):
				validateTypeRef(element, '$path.element', source, false);
				if (length <= 0) {
					add(path, "fixed-array length must be positive for strict C11", source);
				}
				validateStableId(witnessId, '$path.witness', source);
				switch element {
					case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_):
					case _:
						add(path, "the admitted fixed-array slice requires a direct scalar element", source);
				}
			case IRTSpan(element, _):
				validateTypeRef(element, '$path.element', source, false);
				switch element {
					case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_):
					case _:
						add(path, "the admitted span slice requires a direct scalar element", source);
				}
		}
	}

	function validateConstant(value:HxcIRConstant, path:String, source:HxcSourceSpan):Void {
		switch value {
			case IRCInt(text):
				if (!~/^-?(0|[1-9][0-9]*)$/.match(text)) {
					add(path, 'integer constant `$text` is not canonical decimal', source);
				}
			case IRCFloat(text):
				if (!~/^-?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?$/.match(text)) {
					add(path, 'floating constant `$text` is not canonical decimal', source);
				}
			case IRCBool(_) | IRCNull:
			case IRCString(text, byteLength):
				final actual = HxcUtf8.byteLength(text);
				if (actual == null) {
					add(path, "string constant is not a valid Unicode-scalar sequence", source);
				} else if (byteLength != actual) {
					add(path, 'string constant records UTF-8 byte length $byteLength but encodes to $actual byte(s)', source);
				}
		}
	}

	static function constantMatchesType(value:HxcIRConstant, type:HxcIRTypeRef):Bool {
		return switch value {
			case IRCInt(_):
				switch type {
					case IRTInt(_, _) | IRTAbiInteger(_): true;
					case _: false;
				}
			case IRCFloat(_):
				switch type {
					case IRTFloat(_): true;
					case _: false;
				}
			case IRCBool(_): type == IRTBool;
			case IRCString(_, _): type == IRTString;
			case IRCNull:
				switch type {
					case IRTPointer(_, true) | IRTNullable(_, _): true;
					case _: false;
				}
		};
	}

	function validateSpan(span:HxcSourceSpan, path:String):Void {
		if (!HxcSourceSpan.isNormalizedFile(span.file)) {
			final stableSource = span.stableForDiagnostic();
			add(path, "source path must be normalized, repository-relative UTF-8 with `/` separators; the unstable spelling was redacted", stableSource);
		}
		if (span.startLine < 1 || span.startColumn < 1 || span.endLine < 1 || span.endColumn < 1) {
			add(path, "source line and column values must be one-based", span);
		} else if (span.endLine < span.startLine || span.endLine == span.startLine && span.endColumn < span.startColumn) {
			add(path, "source span end precedes its start", span);
		}
	}

	function validateStableId(value:String, path:String, source:HxcSourceSpan):Void {
		if (value == "" || StringTools.trim(value) != value || hasControl(value)) {
			add(path, 'stable identifier `${escaped(value)}` is empty, padded, or contains control characters', source);
		}
	}

	function validateText(value:String, path:String, source:HxcSourceSpan):Void {
		if (value == "" || hasControl(value)) {
			add(path, "text must be non-empty and contain no control characters", source);
		}
	}

	function hasControl(value:String):Bool {
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return true;
			}
		}
		return false;
	}

	function add(path:String, message:String, source:HxcSourceSpan):Void
		diagnostics.push(HxcIRDiagnostic.invalidIR(profile, path, message, source));

	static function stateName(state:HxcIRInitializationState):String {
		return switch state {
			case IRISUninitialized: "uninitialized";
			case IRISInitializing: "initializing";
			case IRISInitialized: "initialized";
			case IRISMoved: "moved";
			case IRISDestroyed: "destroyed";
		}
	}

	static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTAbiInteger(kind): 'abi:${abiIntegerKey(kind)}';
			case IRTFloat(width): 'f$width';
			case IRTString: "string-utf8";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "nonnull"}<${typeKey(pointee)}>';
			case IRTNullable(inner, representation): 'nullable:${nullableRepresentationKey(representation)}<${typeKey(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(typeKey).join(",")})->${typeKey(result)}';
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId<${typeKey(element)}>';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${typeKey(element)}>';
			case IRTDynamic: "dynamic";
		}
	}

	static function abiIntegerKey(kind:HxcIRAbiIntegerKind):String {
		return switch kind {
			case IRAKSize: "size";
			case IRAKPtrDiff: "ptrdiff";
			case IRAKIntPtr: "intptr";
			case IRAKUIntPtr: "uintptr";
		}
	}

	static function nullableRepresentationKey(representation:HxcIRNullableRepresentation):String {
		return switch representation {
			case IRNTagged: "tagged";
			case IRNPointer: "pointer";
		}
	}

	static function constantKey(value:HxcIRConstant):String {
		return switch value {
			case IRCInt(text): 'int:$text';
			case IRCFloat(text): 'float:$text';
			case IRCBool(flag): 'bool:$flag';
			case IRCString(text, byteLength): 'string-utf8:$byteLength:${escaped(text)}';
			case IRCNull: "null";
		}
	}

	static function escaped(value:String):String
		return HxcJsonString.quote(value);

	static function sorted<T>(values:Array<T>, key:T->String):Array<T> {
		final copy = values.copy();
		copy.sort((left, right) -> compareUtf8(key(left), key(right)));
		return copy;
	}

	static function sortedMapValues<T>(values:Map<String, T>, key:T->String):Array<T> {
		final copy = [for (value in values) value];
		copy.sort((left, right) -> compareUtf8(key(left), key(right)));
		return copy;
	}

	static function compareDiagnostics(left:HxcIRDiagnostic, right:HxcIRDiagnostic):Int {
		var result = compareUtf8(left.source.file, right.source.file);
		if (result != 0)
			return result;
		result = left.source.startLine - right.source.startLine;
		if (result != 0)
			return result;
		result = left.source.startColumn - right.source.startColumn;
		if (result != 0)
			return result;
		result = compareUtf8(left.id, right.id);
		if (result != 0)
			return result;
		result = compareUtf8(left.irPath, right.irPath);
		return result != 0 ? result : compareUtf8(left.message, right.message);
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
