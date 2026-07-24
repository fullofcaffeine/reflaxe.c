package reflaxe.c.ir;

import haxe.io.Bytes;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRControlFlowAnalysis.HxcIRControlFlowFacts;
import reflaxe.c.ir.HxcIRFixedArrayPolicy.HxcIRFixedArrayStorageDecision;

private typedef HxcIRInstructionSite = {
	final instruction:HxcIRInstruction;
	final block:HxcIRBlock;
}

/** One path state used to prove a branch-assigned local before its first read. */
private typedef HxcIRCarrierFlowState = {
	final blockId:String;
	final assigned:Bool;
}

private enum HxcIRManagedCarrierPhase {
	IRMCEmpty;
	IRMCOwned;
	IRMCMoved;
}

/** One path state used to prove exactly one managed owner crosses a join. */
private typedef HxcIRManagedCarrierFlowState = {
	final blockId:String;
	final phase:HxcIRManagedCarrierPhase;
}

/** Safety checks that are guaranteed to run before another basic block. */
private typedef HxcIRDominanceProofs = {
	final controlFlow:HxcIRControlFlowFacts;
	final proofsByBlock:Map<String, Map<String, Bool>>;
	final nullProofsByBlock:Map<String, Map<String, Bool>>;
}

/** Whether one dispatch layout is embedded in a class or carried by an interface value. */
private enum HxcIRDispatchLayoutKind {
	IRDLVirtual(root:HxcIRTypeInstance);
	IRDLInterface(interfaceType:HxcIRTypeInstance);
}

/** Validates the semantic invariants required before any HxcIR reaches C AST lowering. */
class HxcIRValidator {
	public static inline final SCHEMA_VERSION = 19;

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
	final virtualLayouts:Map<String, HxcIRVirtualTableLayout> = [];
	final virtualSlots:Map<String, HxcIRVirtualSlot> = [];
	final virtualTables:Map<String, HxcIRVirtualTable> = [];
	final managedRootPaths:HxcIRManagedRootPaths;

	public function new(program:HxcIRProgram, profile:String) {
		this.program = program;
		this.profile = profile;
		this.managedRootPaths = new HxcIRManagedRootPaths(program);
	}

	public function validate():Array<HxcIRDiagnostic> {
		if (program.schemaVersion != HxcIRValidator.SCHEMA_VERSION) {
			add("program", 'schema version ${program.schemaVersion} is unsupported; expected ${HxcIRValidator.SCHEMA_VERSION}', programSource());
		}
		indexProgram();
		validateProgramContents();
		validateDispatchPlan();
		validateRetainedInterfaceGraphs();
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
		for (layout in sorted(program.dispatch.layouts, item -> item.id)) {
			indexUnique(virtualLayouts, layout.id, layout, 'dispatch.layout:${layout.id}', layout.source, "virtual-table layout");
		}
		for (slot in sorted(program.dispatch.slots, item -> item.id)) {
			indexUnique(virtualSlots, slot.id, slot, 'dispatch.slot:${slot.id}', slot.source, "virtual slot");
		}
		for (table in sorted(program.dispatch.tables, item -> item.id)) {
			indexUnique(virtualTables, table.id, table, 'dispatch.table:${table.id}', table.source, "virtual table");
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

	function validateDispatchPlan():Void {
		validateOrderedIds(program.dispatch.layouts.map(value -> value.id), "dispatch.layouts", programSource());
		validateOrderedIds(program.dispatch.slots.map(value -> value.id), "dispatch.slots", programSource());
		validateOrderedIds(program.dispatch.tables.map(value -> value.id), "dispatch.tables", programSource());
		final headersByLayout:Map<String, String> = [];
		final layoutsBySlot:Map<String, String> = [];
		final layoutKinds:Map<String, HxcIRDispatchLayoutKind> = [];
		for (declaration in typeDeclarations) {
			switch declaration.kind {
				case IRTKClass({header: IRCHVirtual(layoutId)}):
					final instance = instanceForDeclaration(declaration.id);
					if (instance != null) {
						if (headersByLayout.exists(layoutId)) {
							add('dispatch.layout:$layoutId', 'virtual layout `$layoutId` is installed on more than one class root', declaration.source);
						} else {
							headersByLayout.set(layoutId, instance.id);
						}
					}
				case _:
			}
		}
		for (layout in program.dispatch.layouts) {
			final path = 'dispatch.layout:${layout.id}';
			validateSpan(layout.source, '$path.source');
			final layoutKind = requireDispatchLayoutRoot(layout.rootInstanceId, '$path.rootInstanceId', layout.source);
			if (layoutKind != null)
				layoutKinds.set(layout.id, layoutKind);
			switch layoutKind {
				case IRDLVirtual(_):
					if (headersByLayout.get(layout.id) != layout.rootInstanceId)
						add(path, 'virtual layout `${layout.id}` is not selected by root `${layout.rootInstanceId}`', layout.source);
				case IRDLInterface(_):
				case null:
			}
			validateOrderedIds(layout.slotIds, '$path.slotIds', layout.source);
			for (slotId in layout.slotIds) {
				final priorLayout = layoutsBySlot.get(slotId);
				if (priorLayout != null) {
					add(path, 'dispatch slot `$slotId` belongs to both `$priorLayout` and `${layout.id}`', layout.source);
				} else {
					layoutsBySlot.set(slotId, layout.id);
				}
				final slot = virtualSlots.get(slotId);
				if (slot == null) {
					add(path, 'dispatch layout `${layout.id}` refers to unknown slot `$slotId`', layout.source);
				} else {
					switch layoutKind {
						case IRDLVirtual(root) if (!isClassDescendant(slot.ownerInstanceId, root.id)):
							add(path, 'virtual slot `$slotId` owner `${slot.ownerInstanceId}` is outside root `${root.id}`', slot.source);
						case IRDLInterface(interfaceType) if (slot.ownerInstanceId != interfaceType.id):
							add(path, 'interface slot `$slotId` owner `${slot.ownerInstanceId}` does not match `${interfaceType.id}`', slot.source);
						case _:
					}
				}
			}
		}
		for (layoutId => instanceId in headersByLayout) {
			if (!virtualLayouts.exists(layoutId)) {
				final instance = typeInstances.get(instanceId);
				add('dispatch.layout:$layoutId', 'class root `$instanceId` selects unknown virtual layout `$layoutId`',
					instance == null ? programSource() : instance.source);
			}
		}
		for (slot in program.dispatch.slots) {
			final path = 'dispatch.slot:${slot.id}';
			validateSpan(slot.source, '$path.source');
			if (!layoutsBySlot.exists(slot.id)) {
				add(path, 'dispatch slot `${slot.id}` does not belong to a table layout', slot.source);
			}
			final layoutId = layoutsBySlot.get(slot.id);
			final layoutKind = layoutId == null ? null : layoutKinds.get(layoutId);
			switch layoutKind {
				case IRDLVirtual(_):
					requireDirectClassInstance(slot.ownerInstanceId, '$path.ownerInstanceId', slot.source);
				case IRDLInterface(_):
					requireDirectReferenceInstance(slot.ownerInstanceId, '$path.ownerInstanceId', slot.source);
				case null:
			}
			for (index => parameter in slot.parameterTypes) {
				validateTypeRef(parameter, '$path.parameter:$index', slot.source, false);
				if (parameter == IRTVoid)
					add(path, 'dispatch slot `${slot.id}` parameter $index is Void', slot.source);
			}
			validateTypeRef(slot.returnType, '$path.returnType', slot.source, true);
		}
		for (table in program.dispatch.tables) {
			final path = 'dispatch.table:${table.id}';
			validateSpan(table.source, '$path.source');
			final layout = virtualLayouts.get(table.layoutId);
			final layoutKind = layoutKinds.get(table.layoutId);
			final tableClass = switch layoutKind {
				case IRDLInterface(_): requireInterfaceTableClassInstance(table.classInstanceId, '$path.classInstanceId', table.source);
				case IRDLVirtual(_) | null: requireDirectClassInstance(table.classInstanceId, '$path.classInstanceId', table.source);
			};
			if (layout == null) {
				add(path, 'dispatch table `${table.id}` refers to unknown layout `${table.layoutId}`', table.source);
				continue;
			}
			final family = dispatchFamily(layoutKind);
			switch layoutKind {
				case IRDLVirtual(_) if (tableClass != null && !isClassDescendant(tableClass.id, layout.rootInstanceId)):
					add(path, 'virtual table class `${table.classInstanceId}` is outside layout root `${layout.rootInstanceId}`', table.source);
				case IRDLInterface(_):
				case _:
			}
			if (table.entries.length != layout.slotIds.length) {
				add(path, '$family table `${table.id}` has ${table.entries.length} entries for ${layout.slotIds.length} layout slots', table.source);
			}
			for (index => entry in table.entries) {
				final entryPath = '$path.entry:$index:${entry.slotId}';
				if (index >= layout.slotIds.length || entry.slotId != layout.slotIds[index]) {
					add(entryPath, '$family table entry order does not match layout `${layout.id}`', table.source);
				}
				final slot = virtualSlots.get(entry.slotId);
				if (slot == null)
					continue;
				final applicable = switch layoutKind {
					case IRDLInterface(_): tableClass != null;
					case IRDLVirtual(_): tableClass != null && isClassDescendant(tableClass.id, slot.ownerInstanceId);
					case null: false;
				};
				if (entry.implementationFunctionId == null) {
					if (applicable)
						add(entryPath, 'applicable $family slot `${slot.id}` has no implementation', table.source);
					continue;
				}
				if (!applicable) {
					add(entryPath, 'inapplicable $family slot `${slot.id}` unexpectedly has an implementation', table.source);
					continue;
				}
				final implementation = functions.get(entry.implementationFunctionId);
				if (implementation == null) {
					add(entryPath, '$family table refers to unknown implementation `${entry.implementationFunctionId}`', table.source);
					continue;
				}
				switch layoutKind {
					case IRDLInterface(_):
						validateInterfaceImplementation(slot, table, implementation, entryPath);
					case IRDLVirtual(_):
						validateVirtualImplementation(slot, table, implementation, entryPath);
					case null:
				}
			}
		}
	}

	/**
		Validate the collector graph implied by an interface-valued class field.

		An interface field contains an object pointer that survives its constructor
		call. HxcIR therefore requires both the field owner and every concrete class
		named by that interface's reachable tables to use the exact `managed(gc)`
		representation. CAST lowering may then emit one trace edge from the field's
		object member without guessing lifetime from C syntax.
	**/
	function validateRetainedInterfaceGraphs():Void {
		final dispatchRoots:Map<String, Bool> = [];
		for (layout in program.dispatch.layouts)
			dispatchRoots.set(layout.rootInstanceId, true);
		final interfaceInstances:Map<String, Bool> = [];
		for (instance in typeInstances) {
			final declaration = typeDeclarations.get(instance.declarationId);
			if (declaration != null)
				switch declaration.kind {
					// Arrays, maps, and Bytes are reference-shaped too. Only a
					// reference instance that owns a dispatch layout is a Haxe
					// interface whose `{ object, table }` field needs this graph
					// proof.
					case IRTKReference if (dispatchRoots.exists(instance.id)):
						interfaceInstances.set(instance.id, true);
					case _:
				}
		}
		for (owner in typeInstances) {
			final declaration = typeDeclarations.get(owner.declarationId);
			if (declaration == null)
				continue;
			final fields = switch declaration.kind {
				case IRTKClass(layout): layout.fields;
				case _: continue;
			};
			for (field in fields) {
				final interfaceInstanceId = switch field.type {
					case IRTInstance(instanceId) if (interfaceInstances.exists(instanceId)): instanceId;
					case _: continue;
				};
				if (!isGcManaged(owner.representation))
					add('retained-interface:${owner.id}.${field.name}',
						'class `${owner.id}` retains interface `$interfaceInstanceId` without managed(gc) ownership', field.source);
				var matched = false;
				for (table in program.dispatch.tables) {
					final layout = virtualLayouts.get(table.layoutId);
					if (layout == null || layout.rootInstanceId != interfaceInstanceId)
						continue;
					matched = true;
					final implementation = typeInstances.get(table.classInstanceId);
					if (implementation == null || !isGcManaged(implementation.representation))
						add('retained-interface:${owner.id}.${field.name}',
							'interface `$interfaceInstanceId` table `${table.id}` has non-managed concrete object `${table.classInstanceId}`', table.source);
				}
				if (!matched)
					add('retained-interface:${owner.id}.${field.name}',
						'interface `$interfaceInstanceId` has no reachable concrete table for retained field `${field.name}`', field.source);
			}
		}
	}

	/** Matches the collector representation without relying on unsafe enum equality. */
	function isGcManaged(representation:HxcIRRepresentation):Bool
		return switch representation {
			case IRRManaged("gc"): true;
			case _: false;
		};

	/** Names the dispatch mechanism in diagnostics without conflating interface tables with class virtual tables. */
	function dispatchFamily(kind:Null<HxcIRDispatchLayoutKind>):String {
		return switch kind {
			case IRDLVirtual(_): "virtual";
			case IRDLInterface(_): "interface";
			case null: "dispatch";
		};
	}

	function validateInterfaceImplementation(slot:HxcIRVirtualSlot, table:HxcIRVirtualTable, implementation:HxcIRFunction, path:String):Void {
		if (implementation.parameters.length != slot.parameterTypes.length + 1) {
			add(path,
				'interface implementation `${implementation.id}` has ${implementation.parameters.length} parameters for slot `${slot.id}` expected ${slot.parameterTypes.length + 1}',
				implementation.source);
			return;
		}
		final implementationOwner = switch implementation.parameters[0].type {
			case IRTPointer(IRTInstance(instanceId), true): instanceId;
			case other:
				add(path, 'interface implementation `${implementation.id}` has invalid receiver `${typeKey(other)}`', implementation.source);
				return;
		};
		if (!isClassDescendant(table.classInstanceId, implementationOwner))
			add(path, 'interface table class `${table.classInstanceId}` does not descend from implementation receiver `$implementationOwner`',
				implementation.source);
		for (index in 0...slot.parameterTypes.length)
			if (typeKey(implementation.parameters[index + 1].type) != typeKey(slot.parameterTypes[index]))
				add(path, 'interface implementation `${implementation.id}` parameter $index does not preserve slot `${slot.id}` representation',
					implementation.source);
		if (typeKey(implementation.returnType) != typeKey(slot.returnType))
			add(path, 'interface implementation `${implementation.id}` return type does not preserve slot `${slot.id}` representation', implementation.source);
		switch implementation.failureConvention {
			case IRFCInfallible:
			case IRFCStatus(_):
				add(path, 'interface implementation `${implementation.id}` must be infallible in the admitted dispatch slice', implementation.source);
		}
	}

	function validateVirtualImplementation(slot:HxcIRVirtualSlot, table:HxcIRVirtualTable, implementation:HxcIRFunction, path:String):Void {
		if (implementation.parameters.length != slot.parameterTypes.length + 1) {
			add(path,
				'virtual implementation `${implementation.id}` has ${implementation.parameters.length} parameters for slot `${slot.id}` expected ${slot.parameterTypes.length + 1}',
				implementation.source);
			return;
		}
		final implementationOwner = switch implementation.parameters[0].type {
			case IRTPointer(IRTInstance(instanceId), true): instanceId;
			case other:
				add(path, 'virtual implementation `${implementation.id}` has invalid receiver `${typeKey(other)}`', implementation.source);
				return;
		};
		if (!isClassDescendant(implementationOwner, slot.ownerInstanceId)) {
			add(path, 'virtual implementation receiver `$implementationOwner` does not descend from slot owner `${slot.ownerInstanceId}`',
				implementation.source);
		}
		if (!isClassDescendant(table.classInstanceId, implementationOwner)) {
			add(path, 'virtual table class `${table.classInstanceId}` does not descend from implementation receiver `$implementationOwner`',
				implementation.source);
		}
		for (index in 0...slot.parameterTypes.length) {
			if (typeKey(implementation.parameters[index + 1].type) != typeKey(slot.parameterTypes[index])) {
				add(path, 'virtual implementation `${implementation.id}` parameter $index does not preserve slot `${slot.id}` representation',
					implementation.source);
			}
		}
		if (typeKey(implementation.returnType) != typeKey(slot.returnType)) {
			add(path, 'virtual implementation `${implementation.id}` return type does not preserve slot `${slot.id}` representation', implementation.source);
		}
		switch implementation.failureConvention {
			case IRFCInfallible:
			case IRFCStatus(_):
				add(path, 'virtual implementation `${implementation.id}` must be infallible in the admitted dispatch slice', implementation.source);
		}
	}

	function requireDirectClassInstance(instanceId:String, path:String, source:HxcSourceSpan):Null<HxcIRTypeInstance> {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		if (instance == null || instance.representation != IRRDirect || declaration == null) {
			add(path, 'dispatch refers to unknown direct class instance `$instanceId`', source);
			return null;
		}
		switch declaration.kind {
			case IRTKClass(_):
				return instance;
			case _:
				add(path, 'dispatch instance `$instanceId` is not a class', source);
				return null;
		}
	}

	/**
		Accept the concrete object behind an interface table.

		Call-only interface values may point at a direct stack class. A retained
		interface instead points at a `managed(gc)` class whose exact descriptor
		keeps it alive. Both representations use the same interface table and
		implementation thunks; rejecting the managed form would make the lifetime
		upgrade invalidate an otherwise identical dispatch proof.
	**/
	function requireInterfaceTableClassInstance(instanceId:String, path:String, source:HxcSourceSpan):Null<HxcIRTypeInstance> {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		final validRepresentation = instance != null && switch instance.representation {
			case IRRDirect | IRRManaged("gc"): true;
			case _: false;
		};
		if (!validRepresentation || instance == null || declaration == null) {
			add(path, 'interface dispatch refers to unknown direct-or-managed class instance `$instanceId`', source);
			return null;
		}
		return switch declaration.kind {
			case IRTKClass(_): instance;
			case _:
				add(path, 'interface dispatch instance `$instanceId` is not a class', source);
				null;
		};
	}

	function requireDirectReferenceInstance(instanceId:String, path:String, source:HxcSourceSpan):Null<HxcIRTypeInstance> {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		if (instance == null || instance.representation != IRRDirect || declaration == null) {
			add(path, 'dispatch refers to unknown direct interface instance `$instanceId`', source);
			return null;
		}
		return switch declaration.kind {
			case IRTKReference: instance;
			case _:
				add(path, 'dispatch instance `$instanceId` is not an interface reference', source);
				null;
		};
	}

	function requireDispatchLayoutRoot(instanceId:String, path:String, source:HxcSourceSpan):Null<HxcIRDispatchLayoutKind> {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		if (instance == null || instance.representation != IRRDirect || declaration == null) {
			add(path, 'dispatch refers to unknown direct layout root `$instanceId`', source);
			return null;
		}
		return switch declaration.kind {
			case IRTKClass(_): IRDLVirtual(instance);
			case IRTKReference: IRDLInterface(instance);
			case _:
				add(path, 'dispatch layout root `$instanceId` is neither a class nor an interface reference', source);
				null;
		};
	}

	function instanceForDeclaration(declarationId:String):Null<HxcIRTypeInstance> {
		for (instance in typeInstances) {
			if (instance.declarationId == declarationId)
				return instance;
		}
		return null;
	}

	function isClassDescendant(candidateInstanceId:String, ancestorInstanceId:String):Bool {
		var current:Null<String> = candidateInstanceId;
		final seen:Map<String, Bool> = [];
		while (current != null && !seen.exists(current)) {
			if (current == ancestorInstanceId)
				return true;
			seen.set(current, true);
			final instance = typeInstances.get(current);
			final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
			current = if (declaration == null) {
				null;
			} else {
				switch declaration.kind {
					case IRTKClass(layout): layout.baseInstanceId;
					case _: null;
				}
			};
		}
		return false;
	}

	function validateOrderedIds(ids:Array<String>, path:String, source:HxcSourceSpan):Void {
		final seen:Map<String, Bool> = [];
		var previous:Null<String> = null;
		for (index => id in ids) {
			validateStableId(id, '$path:$index', source);
			if (seen.exists(id))
				add(path, 'ordered ID list repeats `$id`', source);
			seen.set(id, true);
			if (previous != null && compareUtf8(previous, id) >= 0)
				add(path, 'ID list is not strictly UTF-8 ordered at `$id`', source);
			previous = id;
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
			case IRTKClass(layout):
				if (layout.baseInstanceId != null) {
					validateStableId(layout.baseInstanceId, '$path.baseInstanceId', declaration.source);
					final base = typeInstances.get(layout.baseInstanceId);
					final baseDeclaration = base == null ? null : typeDeclarations.get(base.declarationId);
					if (base == null || base.representation != IRRDirect || baseDeclaration == null) {
						add(path, 'class base `${layout.baseInstanceId}` is not a known direct type instance', declaration.source);
					} else {
						switch baseDeclaration.kind {
							case IRTKClass(_):
							case _: add(path, 'class base `${layout.baseInstanceId}` is not a class instance', declaration.source);
						}
					}
				}
				switch layout.header {
					case IRCHNone:
					case IRCHVirtual(layoutId): validateStableId(layoutId, '$path.header.virtualLayout', declaration.source);
					case IRCHRuntime(featureId): validateStableId(featureId, '$path.header.runtimeFeature', declaration.source);
				}
				final names:Map<String, Bool> = [];
				for (index => field in layout.fields) {
					final fieldPath = '$path.field:$index:${field.name}';
					validateStableId(field.name, '$fieldPath.name', field.source);
					validateSpan(field.source, '$fieldPath.source');
					validateTypeRef(field.type, '$fieldPath.type', field.source, false);
					if (names.exists(field.name)) {
						add(fieldPath, 'duplicate class storage field `${field.name}`', field.source);
					} else {
						names.set(field.name, true);
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
				case IRTKClass(layout):
					final validRepresentation = switch [instance.representation, layout.header] {
						case [IRRDirect, IRCHNone | IRCHVirtual(_)]: true;
						case [IRRManaged("gc"), IRCHRuntime("gc")]: true;
						case _: false;
					};
					if (!validRepresentation)
						add(path,
							'class instance `${instance.id}` must pair direct storage with a direct/virtual header or managed(gc) storage with a runtime(gc) header',
							instance.source);
				case _:
			}
		}
		if (declaration != null) {
			switch declaration.kind {
				case IRTKClass(_) if (instance.arguments.length != 0):
					add(path, 'class instance `${instance.id}` must be specialized before layout emission', instance.source);
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
		switch fn.failureConvention {
			case IRFCInfallible:
			case IRFCStatus(_):
				if (fn.returnType != IRTVoid) {
					add(path, "the admitted status convention requires a Void semantic result", fn.source);
				}
		}
		validateStableId(fn.entryBlockId, '$path.entryBlockId', fn.source);

		final locals:Map<String, HxcIRLocal> = [];
		final values:Map<String, HxcIRTypeRef> = [];
		final parametersById:Map<String, HxcIRParameter> = [];
		for (index => parameter in fn.parameters) {
			final parameterPath = '$path.parameter:$index:${parameter.id}';
			validateParameter(parameter, parameterPath);
			indexValue(values, parameter.id, parameter.type, parameterPath, parameter.source);
			parametersById.set(parameter.id, parameter);
		}
		final borrowedIds:Map<String, Bool> = [];
		for (index => parameterId in fn.borrowedClassParameterIds) {
			final borrowPath = '$path.borrowedClassParameter:$index';
			validateStableId(parameterId, borrowPath, fn.source);
			if (borrowedIds.exists(parameterId)) {
				add(borrowPath, 'duplicate borrowed class parameter `$parameterId`', fn.source);
				continue;
			}
			borrowedIds.set(parameterId, true);
			final parameter = parametersById.get(parameterId);
			if (parameter == null) {
				add(borrowPath, 'borrowed class parameter `$parameterId` is not a function parameter', fn.source);
			} else {
				switch parameter.type {
					case IRTPointer(IRTInstance(_), _):
					case _:
						add(borrowPath, 'borrowed parameter `$parameterId` must be a concrete class reference', parameter.source);
				}
			}
		}
		final borrowedInterfaceParameterIds = fn.borrowedInterfaceParameterIds == null ? [] : fn.borrowedInterfaceParameterIds;
		for (index => parameterId in borrowedInterfaceParameterIds) {
			final borrowPath = '$path.borrowedInterfaceParameter:$index';
			validateStableId(parameterId, borrowPath, fn.source);
			if (borrowedIds.exists(parameterId)) {
				add(borrowPath, 'duplicate borrowed reference parameter `$parameterId`', fn.source);
				continue;
			}
			borrowedIds.set(parameterId, true);
			final parameter = parametersById.get(parameterId);
			if (parameter == null) {
				add(borrowPath, 'borrowed interface parameter `$parameterId` is not a function parameter', fn.source);
			} else {
				switch parameter.type {
					case IRTInstance(instanceId):
						requireDirectReferenceInstance(instanceId, '$borrowPath.type', parameter.source);
					case _:
						add(borrowPath, 'borrowed interface parameter `$parameterId` must be a direct interface value', parameter.source);
				}
			}
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
		final borrowedLocalIds:Map<String, Bool> = [];
		for (index => localId in fn.borrowedClassLocalIds) {
			final borrowPath = '$path.borrowedClassLocal:$index';
			validateStableId(localId, borrowPath, fn.source);
			if (borrowedLocalIds.exists(localId)) {
				add(borrowPath, 'duplicate borrowed class local `$localId`', fn.source);
				continue;
			}
			borrowedLocalIds.set(localId, true);
			final local = locals.get(localId);
			if (local == null) {
				add(borrowPath, 'borrowed class local `$localId` is not a function local', fn.source);
				continue;
			}
			if (!isConcreteClassReference(local.type))
				add(borrowPath, 'borrowed local `$localId` must be a concrete class reference', local.source);
			switch local.storage {
				case IRLSAutomatic:
				case IRLSStatic | IRLSFrame | IRLSRegion(_):
					add(borrowPath, 'borrowed class local `$localId` must use automatic function storage', local.source);
			}
			if (local.initialState != IRISUninitialized)
				add(borrowPath, 'borrowed class local `$localId` must begin uninitialized', local.source);
		}

		final blocks:Map<String, HxcIRBlock> = [];
		final instructionIds:Map<String, Bool> = [];
		final instructionSites:Map<String, HxcIRInstructionSite> = [];
		final valueSites:Map<String, HxcIRInstructionSite> = [];
		final blockParameterIds:Map<String, Bool> = [];
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
				blockParameterIds.set(parameter.id, true);
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

		validateManagedRoots(fn, path, values, parametersById, valueSites, blockParameterIds);

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

		final dominanceProofs = buildDominanceProofs(fn);
		for (block in sorted(fn.blocks, item -> item.id)) {
			validateBlock(fn, block, '$path.block:${block.id}', locals, borrowedLocalIds, blocks, regions, instructionSites, valueSites, dominanceProofs);
		}
	}

	/**
		Prove that every function root names one exact collector-managed value.

		Block parameters are deliberately rejected in schema 19. Their value changes
		on incoming edges, so they need an edge-owned root update rather than the
		simpler "store immediately after definition" rule used for parameters and
		instruction results.
	**/
	function validateManagedRoots(fn:HxcIRFunction, path:String, values:Map<String, HxcIRTypeRef>, parameters:Map<String, HxcIRParameter>,
			valueSites:Map<String, HxcIRInstructionSite>, blockParameterIds:Map<String, Bool>):Void {
		if (fn.managedRoots == null) {
			add('$path.managedRoots', "function has no explicit managed-root plan for schema 19", fn.source);
			return;
		}
		final rootIds:Map<String, Bool> = [];
		final rootedPaths:Map<String, Bool> = [];
		for (index => root in fn.managedRoots) {
			final rootPath = '$path.managedRoot:$index:${root.id}';
			validateStableId(root.id, '$rootPath.id', root.source);
			validateStableId(root.valueId, '$rootPath.valueId', root.source);
			validateSpan(root.source, '$rootPath.source');
			if (rootIds.exists(root.id))
				add(rootPath, 'duplicate managed root ID `${root.id}`', root.source);
			else
				rootIds.set(root.id, true);
			final type = values.get(root.valueId);
			if (type == null) {
				add(rootPath, 'managed root `${root.id}` names unknown value `${root.valueId}`', root.source);
				continue;
			}
			if (blockParameterIds.exists(root.valueId)) {
				add(rootPath, 'managed block parameter `${root.valueId}` requires edge-owned root updates', root.source);
				continue;
			}
			if (!parameters.exists(root.valueId) && !valueSites.exists(root.valueId)) {
				add(rootPath, 'managed root `${root.id}` must name a function parameter or instruction result', root.source);
			}
			final projectionKey = HxcIRManagedRootPaths.key(root.projections);
			final completeKey = root.valueId + "|" + projectionKey;
			if (rootedPaths.exists(completeKey))
				add(rootPath, 'managed value `${root.valueId}` repeats root path `$projectionKey`', root.source);
			else
				rootedPaths.set(completeKey, true);
			var admitted = false;
			for (expected in managedRootPaths.collect(type))
				if (HxcIRManagedRootPaths.key(expected) == projectionKey) {
					admitted = true;
					break;
				}
			if (!admitted)
				add(rootPath, 'managed root `${root.id}` has invalid path `$projectionKey` for `${typeKey(type)}`', root.source);
		}
		for (valueId => type in values) {
			if (blockParameterIds.exists(valueId)) {
				if (managedRootPaths.collect(type).length > 0)
					add('$path.managedRoots', 'managed block parameter `$valueId` requires edge-owned root updates', fn.source);
				continue;
			}
			for (expected in managedRootPaths.collect(type)) {
				final projectionKey = HxcIRManagedRootPaths.key(expected);
				if (!rootedPaths.exists(valueId + "|" + projectionKey))
					add('$path.managedRoots', 'managed value `$valueId` is missing exact root path `$projectionKey`', fn.source);
			}
		}
	}

	/**
		Find bounds and null checks that dominate later control-flow blocks.

		A check in a predecessor remains valid because HxcIR values are immutable;
		whole fixed-array/span assignment is also forbidden. Checks in a block with
		an instruction-level failure jump are kept local: that jump could leave before
		a later check, so block-level dominance alone would not be a sufficient proof.
	**/
	function buildDominanceProofs(fn:HxcIRFunction):HxcIRDominanceProofs {
		final controlFlow = new HxcIRControlFlowAnalysis().analyze(fn);
		final proofsByBlock:Map<String, Map<String, Bool>> = [];
		final nullProofsByBlock:Map<String, Map<String, Bool>> = [];
		for (block in fn.blocks) {
			final proofs:Map<String, Bool> = [];
			final nullProofs:Map<String, Bool> = [];
			if (!controlFlow.hasInstructionFailureJump(block.id)) {
				for (instruction in block.instructions) {
					switch instruction.kind {
						case IRIOBoundsCheck(collection, indexValueId, _):
							final key = collectionProofKey(collection, indexValueId);
							if (key != null)
								proofs.set(key, true);
						case IRIONullCheck(valueId, _):
							nullProofs.set(valueId, true);
						case _:
					}
				}
			}
			proofsByBlock.set(block.id, proofs);
			nullProofsByBlock.set(block.id, nullProofs);
		}
		return {controlFlow: controlFlow, proofsByBlock: proofsByBlock, nullProofsByBlock: nullProofsByBlock};
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
		final noValues:Map<String, HxcIRTypeRef> = [];
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
					case IRCARelease(place, implementation):
						validateStableCleanupPlace(place, actionPath, action.source, locals);
						validateImplementation(implementation, '$actionPath.implementation', action.source);
						if (isArrayRuntimeImplementation(implementation)
							&& managedArrayElement(knownPlaceType(place, noValues, locals)) == null)
							add(actionPath, "array release cleanup requires a managed Array place", action.source);
						if (isStringMapRuntimeImplementation(implementation)
							&& managedStringMapValue(knownPlaceType(place, noValues, locals)) == null)
							add(actionPath, "StringMap release cleanup requires a managed Map<String, V> place", action.source);
						if (isIntMapRuntimeImplementation(implementation) && !isManagedIntBoolMap(knownPlaceType(place, noValues, locals)))
							add(actionPath, "IntMap release cleanup requires a managed Map<Int, Bool> place", action.source);
						if (isBytesRuntimeImplementation(implementation) && !isManagedBytes(knownPlaceType(place, noValues, locals)))
							add(actionPath, "bytes release cleanup requires a managed Bytes place", action.source);
						switch implementation {
							case IRIProgramLocal(helperId):
								if (StringTools.startsWith(helperId, "array-element-lifecycle:")) {
									final expectedInstanceId = arrayElementDestroyInstanceId(helperId);
									if (expectedInstanceId == null) {
										add(actionPath, "Array element release cleanup has a malformed typed destroy plan", action.source);
									} else
										switch knownPlaceType(place, noValues, locals) {
											case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
											case _:
												add(actionPath, "Array element destroy plan and cleanup place type differ", action.source);
										}
								} else if (StringTools.startsWith(helperId, "enum-lifecycle:")) {
									final expectedInstanceId = enumArrayLifecycleInstanceId(helperId, "destroy");
									if (expectedInstanceId == null) {
										add(actionPath, "managed enum release cleanup has a malformed typed destroy plan", action.source);
									} else
										switch knownPlaceType(place, noValues, locals) {
											case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
											case _:
												add(actionPath, "managed enum destroy plan and cleanup place type differ", action.source);
										}
								} else if (StringTools.startsWith(helperId, "aggregate-lifecycle:")) {
									final expectedInstanceId = aggregateLifecycleInstanceId(helperId, "destroy");
									if (expectedInstanceId == null) {
										add(actionPath, "managed record release cleanup has a malformed typed destroy plan", action.source);
									} else
										switch knownPlaceType(place, noValues, locals) {
											case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
											case _:
												add(actionPath, "managed record destroy plan and cleanup place type differ", action.source);
										}
								}
							case _:
						}
					case IRCADeallocate(place, implementation):
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

	/** Extract the exact element instance named by a program-local destroy plan. */
	static function arrayElementDestroyInstanceId(helperId:String):Null<String> {
		final prefix = "array-element-lifecycle:";
		final suffix = ":destroy";
		if (!StringTools.startsWith(helperId, prefix) || !StringTools.endsWith(helperId, suffix))
			return null;
		final result = helperId.substring(prefix.length, helperId.length - suffix.length);
		return result == "" ? null : result;
	}

	/** Extract the enum instance named by a typed managed-Array lifecycle plan. */
	static function enumArrayLifecycleInstanceId(helperId:String, operation:String):Null<String> {
		final prefix = "enum-lifecycle:";
		final suffix = ':$operation';
		if (!StringTools.startsWith(helperId, prefix) || !StringTools.endsWith(helperId, suffix))
			return null;
		final result = helperId.substring(prefix.length, helperId.length - suffix.length);
		return result == "" ? null : result;
	}

	/** Require one private tagged enum; lifecycle helpers make its managed policy explicit. */
	function requireManagedTaggedEnum(type:HxcIRTypeRef, path:String, source:HxcSourceSpan):Null<String> {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _:
				add(path, "managed carrier requires one tagged enum instance", source);
				return null;
		};
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		if (instance == null || declaration == null || instance.representation != IRRTagged) {
			add(path, "managed carrier requires one tagged enum instance", source);
			return null;
		}
		return switch declaration.kind {
			case IRTKTaggedUnion(_): instanceId;
			case _:
				add(path, "managed carrier requires one tagged enum instance", source);
				null;
		};
	}

	/** Match a managed carrier's retain/destroy helper to its exact enum instance. */
	function requireManagedEnumLifecycle(type:HxcIRTypeRef, selected:HxcIRImplementation, operation:String, path:String, source:HxcSourceSpan):Null<String> {
		final instanceId = requireManagedTaggedEnum(type, path, source);
		final plannedId = switch selected {
			case IRIProgramLocal(helperId): enumArrayLifecycleInstanceId(helperId, operation);
			case _:
				add(path, 'managed carrier $operation requires one program-local enum lifecycle plan', source);
				null;
		};
		if (plannedId == null)
			add(path, 'managed carrier has a malformed enum $operation plan', source);
		else if (instanceId != null && plannedId != instanceId)
			add(path, 'managed carrier enum and $operation plan differ', source);
		return instanceId == null || plannedId != instanceId ? null : instanceId;
	}

	/** Extract the closed-record instance named by a typed ownership plan. */
	static function aggregateLifecycleInstanceId(helperId:String, operation:String):Null<String> {
		final prefix = "aggregate-lifecycle:";
		final suffix = ':$operation';
		if (!StringTools.startsWith(helperId, prefix) || !StringTools.endsWith(helperId, suffix))
			return null;
		final result = helperId.substring(prefix.length, helperId.length - suffix.length);
		return result == "" ? null : result;
	}

	function validateBlock(fn:HxcIRFunction, block:HxcIRBlock, path:String, locals:Map<String, HxcIRLocal>, borrowedClassLocals:Map<String, Bool>,
			blocks:Map<String, HxcIRBlock>, regions:Map<String, HxcIRCleanupRegion>, instructionSites:Map<String, HxcIRInstructionSite>,
			valueSites:Map<String, HxcIRInstructionSite>, dominanceProofs:HxcIRDominanceProofs):Void {
		final available:Map<String, HxcIRTypeRef> = [];
		final borrowedClassValues:Map<String, Bool> = [];
		for (parameter in fn.parameters) {
			available.set(parameter.id, parameter.type);
		}
		for (parameterId in fn.borrowedClassParameterIds)
			borrowedClassValues.set(parameterId, true);
		final borrowedInterfaceParameterIds = fn.borrowedInterfaceParameterIds == null ? [] : fn.borrowedInterfaceParameterIds;
		for (parameterId in borrowedInterfaceParameterIds)
			borrowedClassValues.set(parameterId, true);
		for (parameter in block.parameters) {
			available.set(parameter.id, parameter.type);
		}

		final boundsProofs:Map<String, Bool> = [];
		final nullProofs:Map<String, Bool> = [];
		for (dominator in dominanceProofs.controlFlow.strictDominatorsOf(block.id)) {
			final proofs = dominanceProofs.nullProofsByBlock.get(dominator);
			if (proofs != null)
				for (valueId in proofs.keys())
					nullProofs.set(valueId, true);
		}
		for (index => instruction in block.instructions) {
			final instructionPath = '$path.instruction:$index:${instruction.id}';
			validateInstruction(instruction, instructionPath, block, available, locals, blocks, regions, instructionSites, valueSites, boundsProofs,
				nullProofs, dominanceProofs);
			validateBorrowedClassInstruction(instruction, instructionPath, borrowedClassValues, borrowedClassLocals);
			if (instruction.result != null) {
				available.set(instruction.result.id, instruction.result.type);
				if (instructionResultBorrowsClass(instruction, borrowedClassValues, borrowedClassLocals))
					borrowedClassValues.set(instruction.result.id, true);
			}
		}

		if (block.terminator == null) {
			add(path, 'basic block `${block.id}` has no terminator', block.source);
			return;
		}
		validateSpan(block.terminator.source, '$path.terminator.source');
		validateBorrowedClassTerminator(block.terminator.kind, '$path.terminator', block.terminator.source, borrowedClassValues);
		validateTerminator(fn, block.terminator.kind, '$path.terminator', block.terminator.source, available, blocks, regions);
	}

	/**
		Enforce caller-owned class and interface reference contracts before C.

		A borrowed class pointer or interface pair refers to storage that this
		function may use during the call but may not keep. Scalar field mutation
		and interface dispatch are therefore fine; copying the reference into
		another owner, returning it, or handing it to a callee without the same
		checked contract is not.
	**/
	function validateBorrowedClassInstruction(instruction:HxcIRInstruction, path:String, borrowed:Map<String, Bool>, borrowedLocals:Map<String, Bool>):Void {
		function rejectValue(valueId:String, role:String):Void {
			if (borrowed.exists(valueId))
				add(path, 'borrowed reference value `$valueId` escapes through $role', instruction.source);
		}
		function rejectValues(valueIds:Array<String>, role:String):Void {
			for (index => valueId in valueIds)
				if (borrowed.exists(valueId))
					add(path, 'borrowed reference value `$valueId` escapes through $role $index', instruction.source);
		}

		switch instruction.kind {
			case IRIOStore(IRPLocal(localId), _) if (borrowedLocals.exists(localId)):
				add(path, 'borrowed class local `$localId` cannot be reassigned', instruction.source);
			case IRIOStore(_, valueId):
				rejectValue(valueId, "a store");
			case IRIOInitialize(IRPLocal(localId), valueId, _, _) if (borrowedLocals.exists(localId)):
				if (!borrowed.exists(valueId))
					add(path, 'borrowed class local `$localId` must be initialized from a borrowed class value', instruction.source);
			case IRIOInitialize(_, valueId, _, _):
				rejectValue(valueId, "an initializer");
			case IRIOInitializeFixedArray(_, values, _, _):
				rejectValues(values, "fixed-array initializer value");
			case IRIOConstructAggregate(_, fields):
				for (field in fields)
					rejectValue(field.valueId, 'aggregate field `${field.name}`');
			case IRIOConstructInterface(_, _, _):
				// The wrapper retains the same borrow. A later store, return, or
				// unowned argument is rejected through result-borrow propagation.
			case IRIOConstructTag(_, tagName, payload):
				rejectValues(payload, 'tag `$tagName` payload');
			case IRIOCall(call):
				validateBorrowedClassCall(call, path, instruction.source, borrowed);
				if (call.failure != null)
					rejectValues(call.failure.arguments, "failure-edge argument");
			case IRIODeallocate(place, _) | IRIORetain(place, _) | IRIORelease(place, _) | IRIOTrace(place, _):
				if (placeUsesBorrowedClass(place, borrowed))
					add(path, "borrowed class storage cannot be deallocated, retained, or traced as owned storage", instruction.source);
			case IRIOLifetime(place, _, _, _):
				if (placeUsesBorrowedClass(place, borrowed))
					add(path, "borrowed class storage cannot acquire a local ownership lifetime", instruction.source);
			case IRIOSequence(_) | IRIOConstant(_) | IRIOFunctionReference(_) | IRIOLoad(_) | IRIOAddress(_) | IRIOBorrowClassField(_) | IRIOUnary(_, _, _) |
				IRIOBinary(_, _, _) | IRIOConvert(_, _, _, _, _) | IRIOProject(_, _) | IRIOMatchTag(_, _) | IRIOProjectTag(_, _, _, _) |
				IRIOAllocate(_, _, _, _) | IRIODeclareUninitialized(_) | IRIODeclareManagedCarrier(_, _) | IRIOAcquireManagedCarrier(_, _, _) |
				IRIOMoveManagedCarrier(_) | IRIODefaultInitialize(_, _, _) | IRIOZeroInitializeFixedArray(_, _, _) | IRIOInitializeSpan(_, _, _, _) |
				IRIOBindVirtualTable(_, _) | IRIOBoundsCheck(_, _, _) | IRIONullCheck(_, _):
		}
	}

	/** Permit a borrow only at a direct target parameter that declares it. */
	function validateBorrowedClassCall(call:HxcIRCall, path:String, source:HxcSourceSpan, borrowed:Map<String, Bool>):Void {
		for (index => valueId in call.arguments) {
			if (!borrowed.exists(valueId))
				continue;
			final admitted = switch call.dispatch {
				case IRCDDirect(functionId): directTargetBorrowsArgument(functionId, index);
				case IRCDVirtual(_, _) | IRCDInterface(_, _, _) | IRCDClosure(_) | IRCDNative(_) | IRCDRuntime(_, _) | IRCDIntrinsic(_):
					false;
			};
			if (!admitted)
				add(path, 'borrowed reference argument `$valueId` has no checked borrow contract at argument $index', source);
		}
		for (receiverId in borrowedDispatchReceivers(call.dispatch))
			if (borrowed.exists(receiverId))
				switch call.dispatch {
					case IRCDVirtual(_, _) | IRCDInterface(_, _, _):
						// A virtual or interface receiver is used only for this dispatch. Its explicit
						// arguments remain rejected above until slots carry borrow contracts.
					case _:
						add(path, 'borrowed reference receiver `$receiverId` has no checked dispatch contract', source);
				}
	}

	/** Check one direct-call parameter against both admitted borrow carriers. */
	function directTargetBorrowsArgument(functionId:String, argumentIndex:Int):Bool {
		final target = functions.get(functionId);
		if (target == null || argumentIndex >= target.parameters.length)
			return false;
		final parameterId = target.parameters[argumentIndex].id;
		final borrowedInterfaces = target.borrowedInterfaceParameterIds == null ? [] : target.borrowedInterfaceParameterIds;
		return target.borrowedClassParameterIds.indexOf(parameterId) != -1 || borrowedInterfaces.indexOf(parameterId) != -1;
	}

	function borrowedDispatchReceivers(dispatch:HxcIRCallDispatch):Array<String> {
		return switch dispatch {
			case IRCDVirtual(_, receiverValueId) | IRCDInterface(_, _, receiverValueId): [receiverValueId];
			case IRCDClosure(callableValueId): [callableValueId];
			case IRCDDirect(_) | IRCDNative(_) | IRCDRuntime(_, _) | IRCDIntrinsic(_): [];
		};
	}

	/** Track class pointers and interface wrappers whose lifetime remains tied to borrowed storage. */
	function instructionResultBorrowsClass(instruction:HxcIRInstruction, borrowed:Map<String, Bool>, borrowedLocals:Map<String, Bool>):Bool {
		final result = instruction.result;
		if (result == null)
			return false;
		return switch instruction.kind {
			case IRIOLoad(IRPLocal(localId)): isConcreteClassReference(result.type) && borrowedLocals.exists(localId);
			case IRIOBorrowClassField(_):
				isConcreteClassReference(result.type);
			case IRIOAddress(place): isConcreteClassReference(result.type) && placeUsesBorrowedClass(place, borrowed);
			case IRIOConvert(valueId, _, _, _, _): isConcreteClassReference(result.type) && borrowed.exists(valueId);
			case IRIOConstructInterface(_, objectValueId, _):
				borrowed.exists(objectValueId);
			case IRIOCall(call): isConcreteClassReference(result.type) && switch call.dispatch {
					case IRCDDirect(functionId): StringTools.startsWith(functionId,
							"method.") && call.arguments.length > 0 && borrowed.exists(call.arguments[0]);
					case IRCDVirtual(_, receiverValueId):
						borrowed.exists(receiverValueId);
					case IRCDInterface(_, _, receiverValueId) | IRCDClosure(receiverValueId):
						borrowed.exists(receiverValueId);
					case IRCDNative(_) | IRCDRuntime(_, _) | IRCDIntrinsic(_):
						false;
				};
			case _:
				false;
		};
	}

	function validateBorrowedClassTerminator(kind:HxcIRTerminatorKind, path:String, source:HxcSourceSpan, borrowed:Map<String, Bool>):Void {
		function rejectValue(valueId:String, role:String):Void {
			if (borrowed.exists(valueId))
				add(path, 'borrowed reference value `$valueId` escapes through $role', source);
		}
		function rejectEdge(edge:HxcIRBlockEdge, role:String):Void {
			for (index => valueId in edge.arguments)
				if (borrowed.exists(valueId))
					add(path, 'borrowed reference value `$valueId` escapes through $role argument $index', source);
		}
		switch kind {
			case IRTJump(edge):
				rejectEdge(edge, "a jump");
			case IRTBranch(_, whenTrue, whenFalse):
				rejectEdge(whenTrue, "a branch");
				rejectEdge(whenFalse, "a branch");
			case IRTSwitch(_, cases, defaultEdge):
				for (item in cases)
					rejectEdge(item.edge, "a switch edge");
				rejectEdge(defaultEdge, "a switch edge");
			case IRTTagSwitch(_, cases, defaultEdge):
				for (item in cases)
					rejectEdge(item.edge, "a tag-switch edge");
				if (defaultEdge != null)
					rejectEdge(defaultEdge, "a tag-switch edge");
			case IRTReturn(valueId, _):
				if (valueId != null)
					rejectValue(valueId, "a return");
			case IRTThrow(valueId, edge):
				rejectValue(valueId, "a throw");
				for (index => argument in edge.arguments)
					if (borrowed.exists(argument))
						add(path, 'borrowed reference value `$argument` escapes through failure-edge argument $index', source);
			case IRTUnreachable:
		}
	}

	function placeUsesBorrowedClass(place:HxcIRPlace, borrowed:Map<String, Bool>):Bool {
		return switch place {
			case IRPDereference(pointerValueId): borrowed.exists(pointerValueId);
			case IRPField(base, _) | IRPIndex(base, _): placeUsesBorrowedClass(base, borrowed);
			case IRPLocal(_) | IRPGlobal(_): false;
		};
	}

	function isConcreteClassReference(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTPointer(IRTInstance(instanceId), _): isClassInstance(instanceId);
			case _: false;
		};
	}

	function validateInstruction(instruction:HxcIRInstruction, path:String, block:HxcIRBlock, available:Map<String, HxcIRTypeRef>,
			locals:Map<String, HxcIRLocal>, blocks:Map<String, HxcIRBlock>, regions:Map<String, HxcIRCleanupRegion>,
			instructionSites:Map<String, HxcIRInstructionSite>, valueSites:Map<String, HxcIRInstructionSite>, boundsProofs:Map<String, Bool>,
			nullProofs:Map<String, Bool>, dominanceProofs:HxcIRDominanceProofs):Void {
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
			case IRIOFunctionReference(functionId):
				validateStableId(functionId, '$path.function', instruction.source);
				final target = functions.get(functionId);
				if (target == null) {
					add(path, 'function reference names unknown target `$functionId`', instruction.source);
				} else if (instruction.result != null) {
					switch instruction.result.type {
						case IRTFunction(parameters, result):
							if (target.failureConvention != IRFCInfallible)
								add(path, 'function reference target `$functionId` is not infallible', instruction.source);
							if (parameters.length != target.parameters.length)
								add(path, 'function reference target `$functionId` has a different parameter count', instruction.source);
							else
								for (index in 0...parameters.length)
									if (typeKey(parameters[index]) != typeKey(target.parameters[index].type))
										add(path, 'function reference target `$functionId` parameter $index has a different type', instruction.source);
							if (typeKey(result) != typeKey(target.returnType)) add(path,
								'function reference target `$functionId` has a different return type', instruction.source);
						case _:
							add(path, "function reference result must have a function type", instruction.source);
					}
				}
			case IRIOLoad(place):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals, block.id, dominanceProofs);
				final loadedType = knownPlaceType(place, available, locals);
				if (instruction.result != null && loadedType != null && typeKey(instruction.result.type) != typeKey(loadedType)) {
					add(path, "load result type does not match its place type", instruction.source);
				}
			case IRIOAddress(place):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals, block.id, dominanceProofs);
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
			case IRIOBorrowClassField(place):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				final borrowedType = knownPlaceType(place, available, locals);
				switch place {
					case IRPField(_, _):
					case _:
						add(path, "borrowed class field operation requires an embedded field place", instruction.source);
				}
				if (instruction.result != null && borrowedType != null) {
					switch instruction.result.type {
						case IRTPointer(pointee, false) if (typeKey(pointee) == typeKey(borrowedType)
							&& isConcreteClassReference(instruction.result.type)):
						case _:
							add(path, "borrowed class field result must be a non-null pointer to a concrete embedded class", instruction.source);
					}
				}
			case IRIOStore(place, valueId):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateCollectionAccessProof(place, boundsProofs, path, instruction.source, available, locals, block.id, dominanceProofs);
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
				final operandType = requireValue(valueId, '$path.value', instruction.source, available);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (operationId == "haxe.direct-optional.is-null" || operationId == "haxe.direct-optional.is-not-null") {
					final validOperand = operandType != null && isTaggedOptionalType(operandType);
					final validResult = instruction.result != null && instruction.result.type == IRTBool;
					if (!validOperand || implementation != IRIStatic || !validResult)
						add(path, "direct-optional null testing requires a tagged scalar, record, or enum operand and a static Bool result",
							instruction.source);
				} else if (operationId == "haxe.string.is-null" || operationId == "haxe.string.is-not-null") {
					final validResult = instruction.result != null && instruction.result.type == IRTBool;
					if (operandType != IRTString || implementation != IRIStatic || !validResult)
						add(path, "String null testing requires one String carrier and a static Bool result", instruction.source);
				}
			case IRIOBinary(operationId, leftValueId, rightValueId, implementation):
				validateStableId(operationId, '$path.operation', instruction.source);
				final leftType = requireValue(leftValueId, '$path.left', instruction.source, available);
				final rightType = requireValue(rightValueId, '$path.right', instruction.source, available);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (operationId == "haxe.class-reference.equal" || operationId == "haxe.class-reference.not-equal") {
					if (leftType == null
						|| rightType == null
						|| typeKey(leftType) != typeKey(rightType)
						|| !isNullableClassPointer(leftType)) {
						add(path, "class-reference equality requires matching nullable concrete-class pointer operands", instruction.source);
					}
					final binaryResult = instruction.result;
					final hasBoolResult = switch binaryResult {
						case null: false;
						case result: result.type == IRTBool;
					};
					if (implementation != IRIStatic || !hasBoolResult) {
						add(path, "class-reference equality requires a static Bool result", instruction.source);
					}
				} else if (operationId == "haxe.array-reference.equal" || operationId == "haxe.array-reference.not-equal") {
					if (leftType == null
						|| rightType == null
						|| typeKey(leftType) != typeKey(rightType)
						|| !isManagedArrayReference(leftType)) {
						add(path, "Array-reference equality requires matching managed Array pointer operands", instruction.source);
					}
					final binaryResult = instruction.result;
					final hasBoolResult = switch binaryResult {
						case null: false;
						case result: result.type == IRTBool;
					};
					if (implementation != IRIStatic || !hasBoolResult)
						add(path, "Array-reference equality requires a static Bool result", instruction.source);
				} else if (operationId == "haxe.string-map-reference.equal" || operationId == "haxe.string-map-reference.not-equal") {
					if (leftType == null
						|| rightType == null
						|| typeKey(leftType) != typeKey(rightType)
						|| !isManagedStringMapReference(leftType)) {
						add(path, "StringMap-reference equality requires matching managed Map<String, V> pointer operands", instruction.source);
					}
					final binaryResult = instruction.result;
					final hasBoolResult = switch binaryResult {
						case null: false;
						case result: result.type == IRTBool;
					};
					if (implementation != IRIStatic || !hasBoolResult)
						add(path, "StringMap-reference equality requires a static Bool result", instruction.source);
				} else if (operationId == "haxe.enum-tag.equal" || operationId == "haxe.enum-tag.not-equal") {
					if (leftType == null
						|| rightType == null
						|| typeKey(leftType) != typeKey(rightType)
						|| !isPayloadFreeDirectEnum(leftType)) {
						add(path, "enum-tag equality requires matching payload-free direct-enum operands", instruction.source);
					}
					final binaryResult = instruction.result;
					final hasBoolResult = switch binaryResult {
						case null: false;
						case result: result.type == IRTBool;
					};
					if (implementation != IRIStatic || !hasBoolResult) {
						add(path, "enum-tag equality requires a static Bool result", instruction.source);
					}
				} else if (isStringEqualityOperation(operationId)) {
					final binaryResult = instruction.result;
					final hasBoolResult = binaryResult != null && binaryResult.type == IRTBool;
					if (leftType != IRTString || rightType != IRTString || implementation != IRIStatic || !hasBoolResult)
						add(path, "String equality requires two immutable String views and a static Bool result", instruction.source);
					if (stringEqualityRequiresLeftNonNull(operationId) && !isStringConstantValue(leftValueId, valueSites))
						add(path, "String equality's left-non-null proof requires a directly defined String constant", instruction.source);
					if (stringEqualityRequiresRightNonNull(operationId) && !isStringConstantValue(rightValueId, valueSites))
						add(path, "String equality's right-non-null proof requires a directly defined String constant", instruction.source);
				}
			case IRIOConvert(valueId, kind, targetType, implementation, failure):
				final sourceType = requireValue(valueId, '$path.value', instruction.source, available);
				validateTypeRef(targetType, '$path.targetType', instruction.source, false);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (sourceType != null) {
					validateConversion(kind, sourceType, targetType, implementation, failure, nullProofs.exists(valueId), path, instruction.source);
				}
				if (failure != null) {
					validateFailureEdge(failure, '$path.failure', instruction.source, available, blocks, regions);
				}
				if (instruction.result != null && typeKey(instruction.result.type) != typeKey(targetType)) {
					add(path, "conversion result type does not match its target type", instruction.source);
				}
			case IRIOCall(call):
				validateCall(call, path, instruction.source, available, blocks, regions, nullProofs);
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
			case IRIOConstructInterface(interfaceInstanceId, objectValueId, tableId):
				final interfaceType = requireDirectReferenceInstance(interfaceInstanceId, '$path.interfaceInstanceId', instruction.source);
				final objectType = requireValue(objectValueId, '$path.object', instruction.source, available);
				validateStableId(tableId, '$path.tableId', instruction.source);
				if (instruction.result != null && typeKey(instruction.result.type) != typeKey(IRTInstance(interfaceInstanceId)))
					add(path, "interface construction result does not match its interface type", instruction.source);
				final objectInstanceId = switch objectType {
					case IRTPointer(IRTInstance(instanceId), _) if (isClassInstance(instanceId)): instanceId;
					case _:
						add(path, "interface construction requires a concrete class reference", instruction.source);
						null;
				};
				final table = virtualTables.get(tableId);
				if (table == null) {
					add(path, 'interface construction refers to unknown table `$tableId`', instruction.source);
				} else {
					final layout = virtualLayouts.get(table.layoutId);
					if (layout == null || interfaceType == null)
						add(path, 'interface table `$tableId` does not implement `$interfaceInstanceId`', instruction.source);
					else if (layout.rootInstanceId != interfaceType.id)
						add(path, 'interface table `$tableId` does not implement `$interfaceInstanceId`', instruction.source);
					if (objectInstanceId != null && !isClassDescendant(table.classInstanceId, objectInstanceId))
						add(path, 'interface table `$tableId` is incompatible with object class `$objectInstanceId`', instruction.source);
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
			case IRIOAllocate(type, intent, implementation, failure):
				validateTypeRef(type, '$path.type', instruction.source, false);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (instruction.result == null || typeKey(instruction.result.type) != typeKey(IRTPointer(type, false)))
					add(path, "allocation result must be a non-null pointer to the allocated type", instruction.source);
				if (failure != null) {
					validateFailureEdge(failure, '$path.failure', instruction.source, available, blocks, regions);
					if (failure.kind != IRFAllocationFailure) {
						add(path, "allocation failure edge must use allocation-failure kind", instruction.source);
					}
				}
				switch implementation {
					case IRIRuntime("gc"):
						if (intent != IRAShared)
							add(path, "collector allocation requires shared lifetime intent", instruction.source);
						final managed = switch type {
							case IRTInstance(instanceId): final instance = typeInstances.get(instanceId); instance != null && switch instance.representation {
									case IRRManaged("gc"): true;
									case _: false;
								};
							case _: false;
						};
						if (!managed)
							add(path, "collector allocation requires one managed(gc) instance payload", instruction.source);
						if (failure == null || failure.target != IRFTAbort) add(path,
							"collector allocation currently requires an explicit abort failure edge", instruction.source);
					case _:
				}
			case IRIODeallocate(place, implementation) | IRIOTrace(place, implementation):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateImplementation(implementation, '$path.implementation', instruction.source);
			case IRIORetain(place, implementation):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (isArrayRuntimeImplementation(implementation) && managedArrayElement(knownPlaceType(place, available, locals)) == null)
					add(path, "array retain requires a managed Array place", instruction.source);
				if (isStringMapRuntimeImplementation(implementation)
					&& managedStringMapValue(knownPlaceType(place, available, locals)) == null)
					add(path, "StringMap retain requires a managed Map<String, V> place", instruction.source);
				if (isIntMapRuntimeImplementation(implementation) && !isManagedIntBoolMap(knownPlaceType(place, available, locals)))
					add(path, "IntMap retain requires a managed Map<Int, Bool> place", instruction.source);
				if (isBytesRuntimeImplementation(implementation) && !isManagedBytes(knownPlaceType(place, available, locals)))
					add(path, "bytes retain requires a managed Bytes place", instruction.source);
				switch implementation {
					case IRIProgramLocal(helperId) if (StringTools.startsWith(helperId, "enum-lifecycle:")):
						final expectedInstanceId = enumArrayLifecycleInstanceId(helperId, "retain");
						if (expectedInstanceId == null) {
							add(path, "managed enum retain has a malformed typed plan", instruction.source);
						} else switch knownPlaceType(place, available, locals) {
							case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
							case _:
								add(path, "managed enum retain plan and place type differ", instruction.source);
						}
					case IRIProgramLocal(helperId) if (StringTools.startsWith(helperId, "aggregate-lifecycle:")):
						final expectedInstanceId = aggregateLifecycleInstanceId(helperId, "retain");
						if (expectedInstanceId == null) {
							add(path, "managed record retain has a malformed typed plan", instruction.source);
						} else switch knownPlaceType(place, available, locals) {
							case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
							case _:
								add(path, "managed record retain plan and place type differ", instruction.source);
						}
					case _:
				}
			case IRIORelease(place, implementation):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateImplementation(implementation, '$path.implementation', instruction.source);
				if (isArrayRuntimeImplementation(implementation) && managedArrayElement(knownPlaceType(place, available, locals)) == null)
					add(path, "array release requires a managed Array place", instruction.source);
				if (isStringMapRuntimeImplementation(implementation)
					&& managedStringMapValue(knownPlaceType(place, available, locals)) == null)
					add(path, "StringMap release requires a managed Map<String, V> place", instruction.source);
				if (isIntMapRuntimeImplementation(implementation) && !isManagedIntBoolMap(knownPlaceType(place, available, locals)))
					add(path, "IntMap release requires a managed Map<Int, Bool> place", instruction.source);
				if (isBytesRuntimeImplementation(implementation) && !isManagedBytes(knownPlaceType(place, available, locals)))
					add(path, "bytes release requires a managed Bytes place", instruction.source);
				switch implementation {
					case IRIProgramLocal(helperId) if (StringTools.startsWith(helperId, "enum-lifecycle:")):
						final expectedInstanceId = enumArrayLifecycleInstanceId(helperId, "destroy");
						if (expectedInstanceId == null) add(path, "managed enum release has a malformed typed plan",
							instruction.source); else switch knownPlaceType(place, available, locals) {
							case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
							case _: add(path, "managed enum destroy plan and release place type differ", instruction.source);
						}
					case IRIProgramLocal(helperId) if (StringTools.startsWith(helperId, "aggregate-lifecycle:")):
						final expectedInstanceId = aggregateLifecycleInstanceId(helperId, "destroy");
						if (expectedInstanceId == null) add(path, "managed record release has a malformed typed plan",
							instruction.source); else switch knownPlaceType(place, available, locals) {
							case IRTInstance(instanceId) if (instanceId == expectedInstanceId):
							case _: add(path, "managed record destroy plan and release place type differ", instruction.source);
						}
					case _:
				}
			case IRIODeclareUninitialized(place):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				switch place {
					case IRPLocal(localId):
						final local = locals.get(localId);
						if (local != null && local.initialState != IRISUninitialized)
							add(path, "uninitialized declaration requires an initially uninitialized local", instruction.source);
						if (local != null && !isUnmanagedDirectCarrier(local.type))
							add(path, "uninitialized declaration requires an unmanaged direct-value local", instruction.source);
						validateConditionalCarrierFlow(localId, block, blocks, path, instruction.source);
					case _:
						add(path, "uninitialized declaration requires an automatic local place", instruction.source);
				}
			case IRIODeclareManagedCarrier(place, destroyImplementation):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateImplementation(destroyImplementation, '$path.destroyImplementation', instruction.source);
				switch place {
					case IRPLocal(localId):
						final local = locals.get(localId);
						if (local != null && local.initialState != IRISUninitialized)
							add(path, "managed carrier declaration requires an initially uninitialized local", instruction.source);
						if (local != null && isUnmanagedDirectCarrier(local.type))
							add(path, "managed carrier declaration requires a value with managed lifetime", instruction.source);
						if (local != null)
							requireManagedEnumLifecycle(local.type, destroyImplementation, "destroy", path, instruction.source);
						if (managedCarrierDeclarationCount(localId, blocks) != 1)
							add(path, "managed carrier storage must have exactly one declaration", instruction.source);
						validateManagedCarrierFlow(localId, block, blocks, path, instruction.source);
					case _:
						add(path, "managed carrier declaration requires an automatic local place", instruction.source);
				}
			case IRIOAcquireManagedCarrier(place, valueId, acquisition):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				switch place {
					case IRPLocal(localId):
						if (managedCarrierDeclarationCount(localId,
							blocks) != 1) add(path, "managed carrier acquisition requires exactly one matching declaration", instruction.source);
					case _:
						add(path, "managed carrier acquisition requires an automatic local place", instruction.source);
				}
				final placeType = knownPlaceType(place, available, locals);
				final valueType = requireValue(valueId, '$path.value', instruction.source, available);
				if (placeType != null && valueType != null && typeKey(placeType) != typeKey(valueType))
					add(path, "managed carrier acquisition value does not match its carrier type", instruction.source);
				if (placeType != null)
					switch acquisition {
						case IRMCAMoveFresh:
							requireManagedTaggedEnum(placeType, path, instruction.source);
							final freshOwner = isFreshManagedCarrierValue(valueId, valueSites);
							if (!freshOwner) add(path,
								"move-fresh acquisition requires a newly constructed enum, an owned call result, or another managed-carrier move",
								instruction.source);
						case IRMCARetainBorrowed(retainImplementation):
							validateImplementation(retainImplementation, '$path.acquisition.implementation', instruction.source);
							requireManagedEnumLifecycle(placeType, retainImplementation, "retain", path, instruction.source);
					}
			case IRIOMoveManagedCarrier(place):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				switch place {
					case IRPLocal(localId):
						if (managedCarrierDeclarationCount(localId,
							blocks) != 1) add(path, "managed carrier move requires exactly one matching declaration", instruction.source);
					case _:
						add(path, "managed carrier move requires an automatic local place", instruction.source);
				}
				final placeType = knownPlaceType(place, available, locals);
				if (placeType != null) {
					requireManagedTaggedEnum(placeType, path, instruction.source);
					if (instruction.result != null && typeKey(instruction.result.type) != typeKey(placeType))
						add(path, "managed carrier move result does not match its carrier type", instruction.source);
				}
			case IRIODefaultInitialize(place, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				final initializedType = knownPlaceType(place, available, locals);
				switch initializedType {
					case IRTInstance(instanceId) if (isClassInstance(instanceId) || isDirectAggregateInstance(instanceId)):
					case _:
						add(path, "default initialization requires a direct record or concrete-class place", instruction.source);
				}
				validateTransition(from, to, '$path.transition', instruction.source);
				if (from != IRISUninitialized || to != IRISInitializing && to != IRISInitialized) {
					add(path, "default object initialization must begin uninitialized and end initializing or initialized", instruction.source);
				}
			case IRIOInitialize(place, valueId, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
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
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
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
			case IRIOZeroInitializeFixedArray(place, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				switch place {
					case IRPLocal(localId):
						final local = locals.get(localId);
						if (local != null) {
							switch local.storage {
								case IRLSAutomatic:
								case _:
									add(path, "zero-initialized fixed-array storage must be an automatic local", instruction.source);
							}
						}
					case _:
						add(path, "zero-initialized fixed-array storage must be an automatic local", instruction.source);
				}
				switch knownPlaceType(place, available, locals) {
					case IRTFixedArray(element, length, _):
						switch HxcIRFixedArrayPolicy.zeroStorage(element, length) {
							case IRFASAutomatic(_, _):
							case IRFASInvalidLength(_):
								add(path, "zero-initialized fixed-array length must be positive", instruction.source);
							case IRFASUnsupportedElement:
								add(path, "zero-initialized fixed arrays require an exact-size integer, binary32, or binary64 element", instruction.source);
							case IRFASSizeOverflow(elementBytes, invalidLength):
								add(path, 'zero-initialized fixed-array size overflows the compiler policy: $invalidLength * $elementBytes bytes',
									instruction.source);
							case IRFASOverBudget(_, totalBytes, maximumBytes):
								add(path, 'zero-initialized fixed-array storage $totalBytes bytes exceeds the $maximumBytes-byte automatic limit',
									instruction.source);
						}
					case _:
						add(path, "zero-initialization requires a fixed-array place", instruction.source);
				}
				validateInitializeTransition(from, to, path, instruction.source);
			case IRIOInitializeSpan(place, sourceArray, from, to):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validatePlace(sourceArray, '$path.sourceArray', instruction.source, available, locals, nullProofs);
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
			case IRIOBindVirtualTable(place, tableId):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateStableId(tableId, '$path.tableId', instruction.source);
				final table = virtualTables.get(tableId);
				if (table == null) {
					add(path, 'virtual-table bind refers to unknown table `$tableId`', instruction.source);
				} else {
					final placeType = knownPlaceType(place, available, locals);
					if (placeType == null || typeKey(placeType) != typeKey(IRTInstance(table.classInstanceId))) {
						add(path, 'virtual-table bind `$tableId` does not match its concrete object place', instruction.source);
					}
				}
			case IRIOBoundsCheck(collection, indexValueId, policy):
				validatePlace(collection, '$path.collection', instruction.source, available, locals, nullProofs);
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
				validateBoundsPolicy(policy, '$path.policy', instruction.source, collection, indexValueId, instruction.id, block, available, locals,
					instructionSites, valueSites);
				final proofKey = collectionProofKey(collection, indexValueId);
				if (proofKey != null) {
					boundsProofs.set(proofKey, true);
				}
			case IRIONullCheck(valueId, policy):
				final checkedType = requireValue(valueId, '$path.value', instruction.source, available);
				if (checkedType != null) {
					switch checkedType {
						case IRTPointer(IRTInstance(instanceId), true) if (isClassInstance(instanceId)):
							nullProofs.set(valueId, true);
						case IRTString:
							nullProofs.set(valueId, true);
						case value if (isTaggedOptionalType(value)):
							nullProofs.set(valueId, true);
						case _:
							add(path, "null check requires a nullable String, concrete-class reference, or tagged scalar, record, or enum", instruction.source);
					}
				}
				validateNullCheckPolicy(policy, '$path.policy', instruction.source);
			case IRIOLifetime(place, from, to, reason):
				validatePlace(place, '$path.place', instruction.source, available, locals, nullProofs);
				validateTransition(from, to, '$path.transition', instruction.source);
				validateText(reason, '$path.reason', instruction.source);
		}
	}

	function instructionProducesValue(kind:HxcIRInstructionKind):Bool {
		return switch kind {
			case IRIOConstant(_) | IRIOFunctionReference(_) | IRIOLoad(_) | IRIOAddress(_) | IRIOBorrowClassField(_) | IRIOUnary(_, _, _) |
				IRIOBinary(_, _, _) | IRIOConvert(_, _, _, _, _) | IRIOConstructAggregate(_, _) | IRIOConstructInterface(_, _, _) | IRIOProject(_, _) |
				IRIOConstructTag(_, _, _) | IRIOMatchTag(_, _) | IRIOProjectTag(_, _, _, _) | IRIOAllocate(_, _, _, _) | IRIOMoveManagedCarrier(_):
				true;
			case IRIOCall(call):
				call.returnType != IRTVoid;
			case IRIOSequence(_) | IRIOStore(_, _) | IRIODeallocate(_, _) | IRIORetain(_, _) | IRIORelease(_, _) | IRIOTrace(_, _) |
				IRIODeclareUninitialized(_) | IRIODeclareManagedCarrier(_, _) | IRIOAcquireManagedCarrier(_, _, _) | IRIODefaultInitialize(_, _, _) |
				IRIOInitialize(_, _, _, _) | IRIOInitializeFixedArray(_, _, _, _) | IRIOZeroInitializeFixedArray(_, _, _) | IRIOInitializeSpan(_, _, _, _) |
				IRIOBindVirtualTable(_, _) | IRIOBoundsCheck(_, _, _) | IRIONullCheck(_, _) | IRIOLifetime(_, _, _, _):
				false;
		}
	}

	function validateCall(call:HxcIRCall, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>, blocks:Map<String, HxcIRBlock>,
			regions:Map<String, HxcIRCleanupRegion>, nullProofs:Map<String, Bool>):Void {
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
					switch target.failureConvention {
						case IRFCInfallible:
						case IRFCStatus(kind):
							if (call.failure == null) {
								add(path, 'status-returning direct call `$functionId` requires an explicit failure edge', source);
							} else if (call.failure.kind != kind) {
								add(path, 'status-returning direct call `$functionId` failure kind does not match its target convention', source);
							}
					}
					if (StringTools.startsWith(functionId, "method.")
						&& call.arguments.length > 0
						&& !nullProofs.exists(call.arguments[0])) {
						add(path, 'direct instance call `$functionId` requires a preceding dominating receiver null check', source);
					}
				}
			case IRCDVirtual(slotId, receiverValueId):
				validateStableId(slotId, '$path.virtualSlot', source);
				final receiverType = requireValue(receiverValueId, '$path.receiver', source, available);
				final slot = virtualSlots.get(slotId);
				if (slot == null) {
					add(path, 'virtual call refers to unknown slot `$slotId`', source);
				} else {
					final expectedReceiver = IRTPointer(IRTInstance(slot.ownerInstanceId), true);
					if (receiverType != null && typeKey(receiverType) != typeKey(expectedReceiver)) {
						add(path, 'virtual call receiver does not match slot `$slotId` owner `${slot.ownerInstanceId}`', source);
					}
					final parameters:Array<HxcIRParameter> = [];
					for (index => parameterType in slot.parameterTypes)
						parameters.push({id: 'virtual.argument.$index', type: parameterType, source: slot.source});
					validateKnownCallSignature(call, argumentTypes, parameters, slot.returnType, path, source);
				}
				if (!nullProofs.exists(receiverValueId))
					add(path, 'virtual call receiver `$receiverValueId` requires a preceding dominating null check', source);
				if (call.failure != null)
					add(path, "virtual calls are infallible in the admitted dispatch slice", source);
			case IRCDInterface(interfaceTypeId, slotId, receiverValueId):
				requireDirectReferenceInstance(interfaceTypeId, '$path.interfaceType', source);
				validateStableId(slotId, '$path.interfaceSlot', source);
				final receiverType = requireValue(receiverValueId, '$path.receiver', source, available);
				if (receiverType != null && typeKey(receiverType) != typeKey(IRTInstance(interfaceTypeId)))
					add(path, 'interface call receiver does not have interface value type `$interfaceTypeId`', source);
				final slot = virtualSlots.get(slotId);
				if (slot == null) {
					add(path, 'interface call refers to unknown slot `$slotId`', source);
				} else {
					if (slot.ownerInstanceId != interfaceTypeId)
						add(path, 'interface call slot `$slotId` belongs to `${slot.ownerInstanceId}`, not `$interfaceTypeId`', source);
					final parameters:Array<HxcIRParameter> = [];
					for (index => parameterType in slot.parameterTypes)
						parameters.push({id: 'interface.argument.$index', type: parameterType, source: slot.source});
					validateKnownCallSignature(call, argumentTypes, parameters, slot.returnType, path, source);
				}
				if (call.failure != null)
					add(path, "interface calls are infallible in the admitted dispatch slice", source);
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
				} else if (featureId == "array") {
					validateManagedArrayCall(call, argumentTypes, path, source);
				} else if (featureId == "string-map") {
					validateStringMapCall(call, argumentTypes, path, source);
				} else if (featureId == "int-map") {
					validateIntMapCall(call, argumentTypes, path, source);
				} else if (featureId == "bytes") {
					validateManagedBytesCall(call, argumentTypes, path, source);
					if (operationId == "of-string-utf8" && call.arguments.length > 0 && !nullProofs.exists(call.arguments[0]))
						add(path, "Bytes.ofString requires a preceding dominating String null check", source);
				} else if (featureId == "string-scalar") {
					validateStringScalarCall(call, argumentTypes, path, source);
					if (operationId == "char-at" && call.arguments.length > 0 && !nullProofs.exists(call.arguments[0]))
						add(path, "String.charAt requires a preceding dominating receiver null check", source);
				}
			case IRCDIntrinsic(intrinsicId):
				validateStableId(intrinsicId, '$path.intrinsic', source);
		}
		if (call.failure != null) {
			validateFailureEdge(call.failure, '$path.failure', source, available, blocks, regions);
		}
	}

	function validateManagedArrayCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("array", value): value;
			case _: return;
		};
		final receiverElement = argumentTypes.length == 0 ? null : managedArrayElement(argumentTypes[0]);
		final secondArgumentType = argumentTypes.length < 2 ? null : argumentTypes[1];
		final thirdArgumentType = argumentTypes.length < 3 ? null : argumentTypes[2];
		switch operationId {
			case "create-literal":
				final resultElement = managedArrayElement(call.returnType);
				if (resultElement == null) {
					add(path, "Array literal creation must return a managed Array instance", source);
				} else {
					for (index => argumentType in argumentTypes)
						if (argumentType != null && typeKey(argumentType) != typeKey(resultElement))
							add(path, 'Array literal element $index does not match its managed element type', source);
				}
			case "length":
				if (argumentTypes.length != 1 || receiverElement == null || typeKey(call.returnType) != typeKey(IRTInt(32, true)))
					add(path, "Array.length requires one managed Array argument and returns Haxe Int", source);
			case "get-checked":
				if (argumentTypes.length != 2 || receiverElement == null || secondArgumentType == null) {
					add(path, "checked Array indexing requires managed Array + Haxe Int and returns its element type", source);
				} else if (typeKey(secondArgumentType) != typeKey(IRTInt(32, true))
					|| typeKey(call.returnType) != typeKey(receiverElement)) {
					add(path, "checked Array indexing requires managed Array + Haxe Int and returns its element type", source);
				}
			case "push":
				if (argumentTypes.length != 2 || receiverElement == null || secondArgumentType == null) {
					add(path, "Array.push requires managed Array + matching element and returns the new Haxe Int length", source);
				} else if (typeKey(secondArgumentType) != typeKey(receiverElement)
					|| typeKey(call.returnType) != typeKey(IRTInt(32, true))) {
					add(path, "Array.push requires managed Array + matching element and returns the new Haxe Int length", source);
				}
			case "set":
				if (argumentTypes.length != 3 || receiverElement == null || secondArgumentType == null || thirdArgumentType == null) {
					add(path, "Array indexed assignment requires managed Array + Haxe Int + matching element", source);
				} else if (typeKey(secondArgumentType) != typeKey(IRTInt(32, true))
					|| typeKey(thirdArgumentType) != typeKey(receiverElement)
					|| typeKey(call.returnType) != typeKey(receiverElement)) {
					add(path, "Array indexed assignment requires managed Array + Haxe Int + matching element", source);
				}
			case _:
				add(path, 'array runtime call names unsupported operation `$operationId`', source);
		}
		validateCleanupFreeStatusAbort(call.failure, path, source, "managed Array operation");
	}

	function managedArrayElement(type:Null<HxcIRTypeRef>):Null<HxcIRTypeRef> {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _: return null;
		};
		final instance = typeInstances.get(instanceId);
		if (instance == null || instance.arguments.length != 1)
			return null;
		return switch instance.representation {
			case IRRManaged("array") | IRRManaged("gc"): instance.arguments[0];
			case _: null;
		};
	}

	/** True when an HxcIR instance is the managed pointer carrier for Array<T>. */
	function isManagedArrayReference(type:HxcIRTypeRef):Bool
		return managedArrayElement(type) != null;

	static function isArrayRuntimeImplementation(implementation:HxcIRImplementation):Bool
		return switch implementation {
			case IRIRuntime("array"): true;
			case _: false;
		};

	/** Validate the closed first Map<String, V> runtime operation family. */
	function validateStringMapCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("string-map", value): value;
			case _: return;
		};
		final receiverValue = argumentTypes.length == 0 ? null : managedStringMapValue(argumentTypes[0]);
		final hasStringKey = argumentTypes.length > 1 && argumentTypes[1] == IRTString;
		final returnsBool = call.returnType == IRTBool;
		switch operationId {
			case "create":
				if (argumentTypes.length != 0 || managedStringMapValue(call.returnType) == null)
					add(path, "StringMap creation takes no arguments and returns one exact Map<String, V> specialization", source);
			case "set":
				final storedType = argumentTypes.length > 2 ? argumentTypes[2] : null;
				if (argumentTypes.length != 3
					|| receiverValue == null
					|| !hasStringKey
					|| storedType == null
					|| typeKey(storedType) != typeKey(receiverValue)
					|| call.returnType != IRTVoid)
					add(path, "StringMap.set requires map + String + exact value and returns Void", source);
			case "clear":
				if (argumentTypes.length != 1 || receiverValue == null || call.returnType != IRTVoid)
					add(path, "StringMap.clear requires one map and returns Void", source);
			case "exists" | "remove":
				if (argumentTypes.length != 2 || receiverValue == null || !hasStringKey || !returnsBool)
					add(path, 'StringMap.$operationId requires map + String and returns Bool', source);
			case "get":
				final expectedReturnKey = receiverValue == null ? null : typeKey(IRTNullable(receiverValue, IRNTagged));
				if (argumentTypes.length != 2
					|| receiverValue == null
					|| !hasStringKey
					|| expectedReturnKey == null
					|| typeKey(call.returnType) != expectedReturnKey)
					add(path, "StringMap.get requires map + String and returns a tagged nullable value", source);
			case _:
				add(path, 'string-map runtime call names unsupported operation `$operationId`', source);
		}
		validateCleanupFreeStatusAbort(call.failure, path, source, "managed StringMap operation");
	}

	/**
		Return V only for the exact managed `Map<String, V>` HxcIR shape.

		The String key remains explicit even though hxrt specializes its table for
		String keys; this prevents a later pass from accepting another map family
		merely because its runtime feature name happens to match.
	**/
	function managedStringMapValue(type:Null<HxcIRTypeRef>):Null<HxcIRTypeRef> {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _: return null;
		};
		final instance = typeInstances.get(instanceId);
		if (instance == null || instance.arguments.length != 2 || instance.arguments[0] != IRTString)
			return null;
		return switch instance.representation {
			case IRRManaged("string-map"): instance.arguments[1];
			case _: null;
		};
	}

	/** True only for an exact validated managed `Map<String, V>` carrier. */
	function isManagedStringMapReference(type:HxcIRTypeRef):Bool
		return managedStringMapValue(type) != null;

	static function isStringMapRuntimeImplementation(implementation:HxcIRImplementation):Bool
		return switch implementation {
			case IRIRuntime("string-map"): true;
			case _: false;
		};

	/** Validate the bounded `Map<Int, Bool>` runtime family. */
	function validateIntMapCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("int-map", value): value;
			case _: return;
		};
		final receiver = argumentTypes.length == 0 ? null : argumentTypes[0];
		final hasReceiver = isManagedIntBoolMap(receiver);
		final keyType = argumentTypes.length > 1 ? argumentTypes[1] : null;
		final hasIntKey = keyType != null && typeKey(keyType) == typeKey(IRTInt(32, true));
		switch operationId {
			case "create":
				if (argumentTypes.length != 0 || !isManagedIntBoolMap(call.returnType))
					add(path, "IntMap creation takes no arguments and returns Map<Int, Bool>", source);
			case "set":
				if (argumentTypes.length != 3 || !hasReceiver || !hasIntKey || argumentTypes[2] != IRTBool || call.returnType != IRTVoid)
					add(path, "IntMap.set requires Map<Int, Bool> + Int + Bool and returns Void", source);
			case "exists":
				if (argumentTypes.length != 2 || !hasReceiver || !hasIntKey || call.returnType != IRTBool)
					add(path, "IntMap.exists requires Map<Int, Bool> + Int and returns Bool", source);
			case _:
				add(path, 'int-map runtime call names unsupported operation `$operationId`', source);
		}
		validateCleanupFreeStatusAbort(call.failure, path, source, "managed IntMap operation");
	}

	/** True only for the exact managed Int/Bool specialization admitted here. */
	function isManagedIntBoolMap(type:Null<HxcIRTypeRef>):Bool {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _: return false;
		};
		final instance = typeInstances.get(instanceId);
		if (instance == null || instance.arguments.length != 2 || instance.arguments[1] != IRTBool)
			return false;
		switch instance.representation {
			case IRRManaged("int-map"):
			case _:
				return false;
		}
		return switch instance.arguments[0] {
			case IRTInt(32, true): true;
			case _: false;
		};
	}

	static function isIntMapRuntimeImplementation(implementation:HxcIRImplementation):Bool
		return switch implementation {
			case IRIRuntime("int-map"): true;
			case _: false;
		};

	function validateManagedBytesCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("bytes", value): value;
			case _: return;
		};
		final intType = typeKey(IRTInt(32, true));
		final isInt = (index:Int) -> {
			if (index >= argumentTypes.length)
				return false;
			final type = argumentTypes[index];
			return type != null && typeKey(type) == intType;
		};
		final isBytes = (index:Int) -> index < argumentTypes.length && isManagedBytes(argumentTypes[index]);
		final returnsBytes = isManagedBytes(call.returnType);
		final returnsInt = typeKey(call.returnType) == intType;
		switch operationId {
			case "alloc":
				if (argumentTypes.length != 1 || !isInt(0) || !returnsBytes)
					add(path, "Bytes.alloc requires one Haxe Int and returns managed Bytes", source);
			case "of-string-utf8":
				if (argumentTypes.length != 1 || argumentTypes[0] != IRTString || !returnsBytes)
					add(path, "Bytes.ofString requires one validated UTF-8 String view and returns managed Bytes", source);
			case "length":
				if (argumentTypes.length != 1 || !isBytes(0) || !returnsInt)
					add(path, "Bytes.length requires one managed Bytes value and returns Haxe Int", source);
			case "get":
				if (argumentTypes.length != 2 || !isBytes(0) || !isInt(1) || !returnsInt)
					add(path, "Bytes.get requires managed Bytes plus a Haxe Int position and returns Haxe Int", source);
			case "set":
				if (argumentTypes.length != 3 || !isBytes(0) || !isInt(1) || !isInt(2) || call.returnType != IRTVoid)
					add(path, "Bytes.set requires managed Bytes, position, and value and returns Void", source);
			case "sub":
				if (argumentTypes.length != 3 || !isBytes(0) || !isInt(1) || !isInt(2) || !returnsBytes)
					add(path, "Bytes.sub requires managed Bytes, position, and length and returns managed Bytes", source);
			case "compare":
				if (argumentTypes.length != 2 || !isBytes(0) || !isBytes(1) || !returnsInt)
					add(path, "Bytes.compare requires two managed Bytes values and returns Haxe Int", source);
			case "blit":
				if (argumentTypes.length != 5 || !isBytes(0) || !isInt(1) || !isBytes(2) || !isInt(3) || !isInt(4) || call.returnType != IRTVoid)
					add(path, "Bytes.blit requires destination, position, source, source position, and length and returns Void", source);
			case "fill":
				if (argumentTypes.length != 4 || !isBytes(0) || !isInt(1) || !isInt(2) || !isInt(3) || call.returnType != IRTVoid)
					add(path, "Bytes.fill requires managed Bytes, position, length, and value and returns Void", source);
			case _:
				add(path, 'bytes runtime call names unsupported operation `$operationId`', source);
		}
		validateCleanupFreeStatusAbort(call.failure, path, source, "managed Bytes operation");
	}

	/**
		Validate the allocation-free String operation before C chooses a symbol.

		The receiver and result are immutable UTF-8 views, while the index remains
		Haxe's signed 32-bit `Int`. `charAt` is total for valid String values, so a
		failure edge would incorrectly turn normal out-of-range access into abort.
	**/
	function validateStringScalarCall(call:HxcIRCall, argumentTypes:Array<Null<HxcIRTypeRef>>, path:String, source:HxcSourceSpan):Void {
		final operationId = switch call.dispatch {
			case IRCDRuntime("string-scalar", value): value;
			case _: return;
		};
		switch operationId {
			case "char-at":
				if (argumentTypes.length != 2
					|| argumentTypes[0] == null
					|| argumentTypes[1] == null
					|| typeKey(argumentTypes[0]) != typeKey(IRTString)
					|| typeKey(argumentTypes[1]) != typeKey(IRTInt(32, true))
					|| typeKey(call.returnType) != typeKey(IRTString))
					add(path, "String.charAt requires String plus Haxe Int and returns String", source);
			case _:
				add(path, 'string-scalar runtime call names unsupported operation `$operationId`', source);
		}
		if (call.failure != null)
			add(path, "String.charAt is total for valid String values and must not carry a failure edge", source);
	}

	function isManagedBytes(type:Null<HxcIRTypeRef>):Bool {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _: return false;
		};
		final instance = typeInstances.get(instanceId);
		if (instance == null || instance.arguments.length != 0)
			return false;
		return switch instance.representation {
			case IRRManaged("bytes"): true;
			case _: false;
		};
	}

	static function isBytesRuntimeImplementation(implementation:HxcIRImplementation):Bool
		return switch implementation {
			case IRIRuntime("bytes"): true;
			case _: false;
		};

	function validateCleanupFreeStatusAbort(failure:Null<HxcIRFailureEdge>, path:String, source:HxcSourceSpan, owner:String):Void {
		if (failure == null) {
			add(path, '$owner requires an explicit native-status failure edge', source);
			return;
		}
		if (failure.kind != IRFNativeStatus
			|| failure.target != IRFTAbort
			|| failure.arguments.length != 0
			|| failure.cleanup.length != 0)
			add(path, '$owner requires a cleanup-free native-status abort edge', source);
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
		validateCleanupFreeStatusAbort(call.failure, path, source, "hosted output");
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
				switch edge.target {
					case IRFTPropagate:
						switch fn.failureConvention {
							case IRFCStatus(kind) if (kind == edge.kind):
							case _:
								add(path, "throw propagation requires a matching function status convention", source);
						}
					case IRFTBlock(_) | IRFTAbort:
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

	function validatePlace(place:HxcIRPlace, path:String, source:HxcSourceSpan, available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>,
			nullProofs:Map<String, Bool>):Void {
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
				final pointerType = requireValue(pointerValueId, '$path.pointer', source, available);
				switch pointerType {
					case IRTPointer(_, true):
						if (!nullProofs.exists(pointerValueId)) add(path, 'nullable pointer `$pointerValueId` requires a preceding dominating null check',
							source);
					case IRTPointer(_, false):
					case null:
					case _:
						add(path, 'dereference requires a pointer value, found `${typeKey(pointerType)}`', source);
				}
			case IRPField(base, fieldName):
				validatePlace(base, '$path.base', source, available, locals, nullProofs);
				validateStableId(fieldName, '$path.field', source);
				final baseType = knownPlaceType(base, available, locals);
				if (baseType != null) {
					switch baseType {
						case IRTInstance(instanceId):
							if (aggregateFieldType(baseType,
								fieldName) == null) add(path, 'direct aggregate/class instance `$instanceId` has no storage field `$fieldName`', source);
						case _:
							add(path, "field place requires a direct aggregate or class instance base", source);
					}
				}
			case IRPIndex(base, indexValueId):
				validatePlace(base, '$path.base', source, available, locals, nullProofs);
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

	function validateNullCheckPolicy(policy:HxcIRNullCheckPolicy, path:String, source:HxcSourceSpan):Void {
		switch policy {
			case IRNCPCheckedAbort(policyProfile, buildMode):
				if (policyProfile != "portable" && policyProfile != "metal") {
					add(path, 'null-check policy has unknown profile `$policyProfile`', source);
				} else if (policyProfile != profile) {
					add(path, 'null-check policy profile `$policyProfile` does not match validator profile `$profile`', source);
				}
				if (buildMode != "debug" && buildMode != "release" && buildMode != "minsizerel") {
					add(path, 'null-check policy has unknown build mode `$buildMode`', source);
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
				if (instance == null || instance.arguments.length != 0 || declaration == null) {
					null;
				} else {
					switch declaration.kind {
						case IRTKAggregate(fields):
							if (instance.representation != IRRDirect) {
								null;
							} else {
								final field = findAggregateField(fields, fieldName);
								field == null ? null : field.type;
							}
						case IRTKClass(layout):
							final fieldBearing = switch instance.representation {
								case IRRDirect | IRRManaged("gc"): true;
								case _: false;
							};
							if (!fieldBearing) {
								null;
							} else {
								final field = findAggregateField(layout.fields, fieldName);
								if (field != null) {
									field.type;
								} else if (layout.baseInstanceId != null) {
									aggregateFieldType(IRTInstance(layout.baseInstanceId), fieldName);
								} else {
									null;
								}
							}
						case _: null;
					}
				}
			case _: null;
		};
	}

	function isClassInstance(instanceId:String):Bool {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		return declaration != null && switch declaration.kind {
			case IRTKClass(_): true;
			case _: false;
		};
	}

	function isDirectAggregateInstance(instanceId:String):Bool {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		return instance != null && instance.representation == IRRDirect && declaration != null && switch declaration.kind {
			case IRTKAggregate(_): true;
			case _: false;
		};
	}

	/**
	 * Whether a value has a complete by-value representation and no cleanup.
	 *
	 * This recursive check is deliberately conservative. A managed, opaque,
	 * reference, class, or recursive representation cannot use an uninitialized
	 * branch carrier because choosing a branch may require ownership work.
	 */
	function isUnmanagedDirectCarrier(type:HxcIRTypeRef):Bool
		return isUnmanagedDirectCarrierInner(type, []);

	function isUnmanagedDirectCarrierInner(type:HxcIRTypeRef, visiting:Map<String, Bool>):Bool {
		return switch type {
			case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTCString: true;
			case IRTInstance(instanceId):
				if (visiting.exists(instanceId)) {
					false;
				} else {
					final instance = typeInstances.get(instanceId);
					final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
					if (instance == null || declaration == null) {
						false;
					} else {
						final direct = switch instance.representation {
							case IRRDirect | IRRTagged: true;
							case IRROpaqueHandle | IRRManaged(_): false;
						};
						if (!direct) {
							false;
						} else {
							visiting.set(instanceId, true);
							final unmanaged = switch declaration.kind {
								case IRTKAggregate(fields):
									allTypeFieldsUnmanaged(fields, visiting);
								case IRTKTaggedUnion(cases):
									var result = true;
									for (tagCase in cases)
										for (payload in tagCase.payload)
											if (!isUnmanagedDirectCarrierInner(payload.type, visiting))
												result = false;
									result;
								case IRTKExtern | IRTKPrimitive: true;
								case IRTKClass(_) | IRTKReference | IRTKFunction: false;
							};
							visiting.remove(instanceId);
							unmanaged;
						}
					}
				}
			case IRTVoid | IRTPointer(_, _) | IRTNullable(_, _) | IRTFunction(_, _) | IRTFixedArray(_, _, _) | IRTSpan(_, _) | IRTDynamic:
				false;
		};
	}

	function allTypeFieldsUnmanaged(fields:Array<HxcIRTypeField>, visiting:Map<String, Bool>):Bool {
		for (field in fields)
			if (!isUnmanagedDirectCarrierInner(field.type, visiting))
				return false;
		return true;
	}

	/**
	 * Prove that every reachable read follows an assignment on that path.
	 *
	 * The declaration must end in a real two-way branch. Walking `(block,
	 * assigned)` pairs then checks nested conditionals and joins without assuming
	 * that visiting one branch first changes the other branch's state.
	 */
	function validateConditionalCarrierFlow(localId:String, declarationBlock:HxcIRBlock, blocks:Map<String, HxcIRBlock>, path:String,
			source:HxcSourceSpan):Void {
		var sawDeclaration = false;
		for (instruction in declarationBlock.instructions) {
			if (!sawDeclaration) {
				sawDeclaration = switch instruction.kind {
					case IRIODeclareUninitialized(IRPLocal(declaredId)) if (declaredId == localId): true;
					case _: false;
				};
			} else if (instructionTouchesLocalPlace(instruction.kind, localId)) {
				add(path, "uninitialized declaration must be the last operation that mentions its carrier before the branch", source);
				return;
			}
		}
		final starts:Array<HxcIRCarrierFlowState> = switch declarationBlock.terminator == null ? null : declarationBlock.terminator.kind {
			case IRTBranch(_, whenTrue, whenFalse): [
					{blockId: whenTrue.targetBlockId, assigned: false},
					{blockId: whenFalse.targetBlockId, assigned: false}
				];
			case _: [];
		};
		if (starts.length != 2) {
			add(path, "uninitialized declaration must immediately feed a structured two-way branch", source);
			return;
		}
		final pending = starts.copy();
		final visited:Map<String, Bool> = [];
		var sawStore = false;
		var sawLoad = false;
		var unsafeRead = false;
		while (pending.length > 0) {
			final state = pending.pop();
			if (state == null)
				continue;
			final stateKey = state.blockId + (state.assigned ? ":assigned" : ":unassigned");
			if (visited.exists(stateKey))
				continue;
			visited.set(stateKey, true);
			final current = blocks.get(state.blockId);
			if (current == null)
				continue;
			var assigned = state.assigned;
			for (instruction in current.instructions) {
				switch instruction.kind {
					case IRIOStore(IRPLocal(targetId), _) if (targetId == localId):
						assigned = true;
						sawStore = true;
					case IRIOLoad(IRPLocal(targetId)) if (targetId == localId):
						sawLoad = true;
						if (!assigned)
							unsafeRead = true;
					case IRIOLoad(place) | IRIOAddress(place) | IRIOBorrowClassField(place) if (placeContainsLocal(place, localId)):
						if (!assigned)
							unsafeRead = true;
					case IRIOStore(place, _) if (placeContainsLocal(place, localId)):
						if (!assigned)
							unsafeRead = true;
					case _:
						if (!assigned && instructionTouchesLocalPlace(instruction.kind, localId))
							unsafeRead = true;
				}
			}
			if (current.terminator != null) {
				final successors:Array<String> = switch current.terminator.kind {
					case IRTJump(edge): [edge.targetBlockId];
					case IRTBranch(_, whenTrue, whenFalse): [whenTrue.targetBlockId, whenFalse.targetBlockId];
					case IRTSwitch(_, cases, defaultEdge): cases.map(item -> item.edge.targetBlockId).concat([defaultEdge.targetBlockId]);
					case IRTTagSwitch(_, cases, defaultEdge):
						final result = cases.map(item -> item.edge.targetBlockId);
						if (defaultEdge != null)
							result.push(defaultEdge.targetBlockId);
						result;
					case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable: [];
				};
				for (successor in successors)
					pending.push({blockId: successor, assigned: assigned});
			}
		}
		if (!sawStore || !sawLoad || unsafeRead)
			add(path, "uninitialized conditional carrier must be assigned on every path before its first read", source);
	}

	/**
	 * Prove that a managed join has one acquired owner on every path that moves it.
	 *
	 * Branches may either move a fresh result or retain a borrowed value. Both
	 * become the same `owned` state. The join consumes that state exactly once;
	 * ordinary loads, stores, retain/release calls, and a second move are rejected
	 * so ownership cannot be duplicated or silently lost in C syntax.
	 */
	function validateManagedCarrierFlow(localId:String, declarationBlock:HxcIRBlock, blocks:Map<String, HxcIRBlock>, path:String, source:HxcSourceSpan):Void {
		var sawDeclaration = false;
		for (instruction in declarationBlock.instructions) {
			if (!sawDeclaration) {
				sawDeclaration = switch instruction.kind {
					case IRIODeclareManagedCarrier(IRPLocal(declaredId), _) if (declaredId == localId): true;
					case _: false;
				};
			} else if (instructionTouchesLocalPlace(instruction.kind, localId)) {
				add(path, "managed carrier declaration must be the last operation that mentions its storage before the branch", source);
				return;
			}
		}
		final starts:Array<HxcIRManagedCarrierFlowState> = switch declarationBlock.terminator == null ? null : declarationBlock.terminator.kind {
			case IRTBranch(_, whenTrue, whenFalse): [
					{blockId: whenTrue.targetBlockId, phase: IRMCEmpty},
					{blockId: whenFalse.targetBlockId, phase: IRMCEmpty}
				];
			case _: [];
		};
		if (starts.length != 2) {
			add(path, "managed carrier declaration must immediately feed a structured two-way branch", source);
			return;
		}
		final pending = starts.copy();
		final visited:Map<String, Bool> = [];
		var sawAcquire = false;
		var sawMove = false;
		var invalidFlow = false;
		while (pending.length > 0) {
			final state = pending.pop();
			if (state == null)
				continue;
			final stateKey = state.blockId + ":" + managedCarrierPhaseKey(state.phase);
			if (visited.exists(stateKey))
				continue;
			visited.set(stateKey, true);
			final current = blocks.get(state.blockId);
			if (current == null)
				continue;
			var phase = state.phase;
			for (instruction in current.instructions) {
				switch instruction.kind {
					case IRIOAcquireManagedCarrier(IRPLocal(targetId), _, _) if (targetId == localId):
						if (phase != IRMCEmpty)
							invalidFlow = true;
						phase = IRMCOwned;
						sawAcquire = true;
					case IRIOMoveManagedCarrier(IRPLocal(targetId)) if (targetId == localId):
						if (phase != IRMCOwned)
							invalidFlow = true;
						phase = IRMCMoved;
						sawMove = true;
					case _:
						if (instructionTouchesLocalPlace(instruction.kind, localId))
							invalidFlow = true;
				}
			}
			final successors:Array<String> = if (current.terminator == null) {
				[];
			} else switch current.terminator.kind {
				case IRTJump(edge): [edge.targetBlockId];
				case IRTBranch(_, whenTrue, whenFalse): [whenTrue.targetBlockId, whenFalse.targetBlockId];
				case IRTSwitch(_, cases, defaultEdge): cases.map(item -> item.edge.targetBlockId).concat([defaultEdge.targetBlockId]);
				case IRTTagSwitch(_, cases, defaultEdge):
					final result = cases.map(item -> item.edge.targetBlockId);
					if (defaultEdge != null)
						result.push(defaultEdge.targetBlockId);
					result;
				case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable: [];
			};
			if (successors.length == 0 && phase == IRMCOwned)
				invalidFlow = true;
			for (successor in successors)
				pending.push({blockId: successor, phase: phase});
		}
		if (!sawAcquire || !sawMove || invalidFlow)
			add(path, "managed carrier must acquire exactly one owner on every normal path before moving it once", source);
	}

	static function managedCarrierPhaseKey(phase:HxcIRManagedCarrierPhase):String
		return switch phase {
			case IRMCEmpty: "empty";
			case IRMCOwned: "owned";
			case IRMCMoved: "moved";
		};

	/** Count the declaration that gives one carrier operation its ownership context. */
	static function managedCarrierDeclarationCount(localId:String, blocks:Map<String, HxcIRBlock>):Int {
		var count = 0;
		for (block in blocks)
			for (instruction in block.instructions)
				switch instruction.kind {
					case IRIODeclareManagedCarrier(IRPLocal(declaredId), _) if (declaredId == localId):
						count++;
					case _:
				}
		return count;
	}

	/**
	 * Check the ownership source claimed by `move-fresh`.
	 *
	 * A move is safe only when the value-producing instruction gives its caller
	 * the one owner. Managed enum construction, a call returning that enum, and
	 * a nested carrier move have that contract. A load does not: it borrows the
	 * owner still held by a local or parameter and must use retain-borrowed.
	 */
	static function isFreshManagedCarrierValue(valueId:String, valueSites:Map<String, HxcIRInstructionSite>):Bool {
		final site = valueSites.get(valueId);
		if (site == null)
			return false;
		return switch site.instruction.kind {
			case IRIOConstructTag(_, _, _) | IRIOCall(_) | IRIOMoveManagedCarrier(_): true;
			case _:
				false;
		};
	}

	/** Finds every instruction whose typed place payload can observe or alter one local's storage. */
	static function instructionTouchesLocalPlace(kind:HxcIRInstructionKind, localId:String):Bool
		return switch kind {
			case IRIOLoad(place) | IRIOStore(place, _) | IRIOAddress(place) | IRIOBorrowClassField(place) | IRIODeallocate(place, _) | IRIORetain(place, _) |
				IRIORelease(place, _) | IRIOTrace(place, _) | IRIODeclareUninitialized(place) | IRIODeclareManagedCarrier(place, _) |
				IRIOAcquireManagedCarrier(place, _, _) | IRIOMoveManagedCarrier(place) | IRIODefaultInitialize(place, _, _) | IRIOInitialize(place, _, _, _) |
				IRIOInitializeFixedArray(place, _, _, _) | IRIOZeroInitializeFixedArray(place, _, _) | IRIOBindVirtualTable(place, _) |
				IRIOBoundsCheck(place, _, _) | IRIOLifetime(place, _, _, _):
				placeContainsLocal(place, localId);
			case IRIOInitializeSpan(place, sourceArray, _, _): placeContainsLocal(place, localId) || placeContainsLocal(sourceArray, localId);
			case IRIOSequence(_) | IRIOConstant(_) | IRIOFunctionReference(_) | IRIOUnary(_, _, _) | IRIOBinary(_, _, _, _) | IRIOConvert(_, _, _, _, _) |
				IRIOCall(_) | IRIOConstructAggregate(_, _) | IRIOConstructInterface(_, _, _) | IRIOProject(_, _) | IRIOConstructTag(_, _, _) |
				IRIOMatchTag(_, _) | IRIOProjectTag(_, _, _, _) | IRIOAllocate(_, _, _, _) | IRIONullCheck(_, _):
				false;
		};

	static function placeContainsLocal(place:HxcIRPlace, localId:String):Bool
		return switch place {
			case IRPLocal(value): value == localId;
			case IRPField(base, _) | IRPIndex(base, _): placeContainsLocal(base, localId);
			case IRPGlobal(_) | IRPDereference(_): false;
		};

	/** True when an instance can live directly inside `{ has_value, value }`. */
	function isTaggedOptionalPayloadInstance(instanceId:String):Bool {
		final instance = typeInstances.get(instanceId);
		final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
		if (instance == null || declaration == null)
			return false;
		return switch declaration.kind {
			case IRTKAggregate(_): instance.representation == IRRDirect;
			case IRTKTaggedUnion(_): instance.representation == IRRDirect || instance.representation == IRRTagged;
			case _:
				false;
		};
	}

	/** True for every payload admitted by the direct tagged optional layout. */
	function isTaggedOptionalType(type:HxcIRTypeRef):Bool
		return switch type {
			case IRTNullable(IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_), IRNTagged): true;
			case IRTNullable(IRTInstance(instanceId), IRNTagged): isTaggedOptionalPayloadInstance(instanceId);
			case _: false;
		};

	/** True only for a direct C-enum instance whose constructors carry no data. */
	function isPayloadFreeDirectEnum(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTInstance(instanceId): final instance = typeInstances.get(instanceId); final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId); instance != null && instance.representation == IRRDirect && declaration != null && switch declaration.kind {
					case IRTKTaggedUnion(cases):
						var payloadFree = true;
						for (tagCase in cases) {
							if (tagCase.payload.length != 0)
								payloadFree = false;
						}
						payloadFree;
					case _: false;
				};
			case _: false;
		};
	}

	function isNullableClassPointer(type:HxcIRTypeRef):Bool {
		return switch type {
			case IRTPointer(IRTInstance(instanceId), true): isClassInstance(instanceId);
			case _: false;
		};
	}

	function isSafeClassUpcast(source:HxcIRTypeRef, target:HxcIRTypeRef):Bool {
		return switch [source, target] {
			case [
				IRTPointer(IRTInstance(sourceId), sourceNullable),
				IRTPointer(IRTInstance(targetId), targetNullable)
			]: sourceNullable == targetNullable && sourceId != targetId && isClassAncestor(sourceId, targetId);
			case _:
				false;
		};
	}

	function isClassAncestor(sourceInstanceId:String, targetInstanceId:String):Bool {
		var current:Null<String> = sourceInstanceId;
		final seen:Map<String, Bool> = [];
		while (current != null && !seen.exists(current)) {
			seen.set(current, true);
			final instance = typeInstances.get(current);
			final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId);
			if (declaration == null)
				return false;
			current = switch declaration.kind {
				case IRTKClass(layout): layout.baseInstanceId;
				case _: return false;
			};
			if (current == targetInstanceId)
				return true;
		}
		return false;
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
						case IRTKClass(layout):
							if (layout.baseInstanceId != null && result.indexOf(layout.baseInstanceId) == -1)
								result.push(layout.baseInstanceId);
							for (field in layout.fields)
								collectDirectLayoutDependencies(field.type, result);
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
			case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTCString | IRTVoid | IRTPointer(_, _) | IRTNullable(_, IRNPointer) |
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
			failure:Null<HxcIRFailureEdge>, hasNullProof:Bool, path:String, source:HxcSourceSpan):Void {
		final requiresFailure = kind == IRCNumericChecked || kind == IRCNullableUnwrap && !hasNullProof;
		if (requiresFailure && failure == null) {
			add(path, "checked or nullable-unwrapping conversion requires an explicit failure edge", source);
		} else if (kind == IRCNullableUnwrap && hasNullProof && failure != null) {
			add(path, "a nullable unwrap with a dominating null check must not repeat a failure edge", source);
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
			case IRCNumericRoundBinary32:
				if (!isFloatWidth(sourceType, 64) || !isFloatWidth(targetType, 32) || implementation != IRIStatic) {
					add(path, "binary32 rounding requires a direct f64 source to f32 target conversion", source);
				}
			case IRCNumericWidenBinary64:
				if (!isFloatWidth(sourceType, 32) || !isFloatWidth(targetType, 64) || implementation != IRIStatic) {
					add(path, "binary64 widening requires a direct f32 source to f64 target conversion", source);
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
			case IRCRepresentation:
				if (!isSafeClassUpcast(sourceType, targetType) || implementation != IRIStatic || failure != null) {
					add(path, "direct representation conversion requires a null-preserving concrete-class upcast", source);
				}
			case IRCPointer | IRCBox | IRCUnbox:
		}
	}

	static function isPrimitiveConversion(kind:HxcIRConversionKind):Bool {
		return switch kind {
			case IRCNumericExact | IRCNumericRoundBinary32 | IRCNumericWidenBinary64 | IRCNumericWrapping | IRCNumericSaturating | IRCNumericChecked |
				IRCNullableInject | IRCNullableUnwrap:
				true;
			case IRCPointer | IRCBox | IRCUnbox | IRCRepresentation:
				false;
		}
	}

	static function isFloatWidth(type:HxcIRTypeRef, width:Int):Bool {
		return switch type {
			case IRTFloat(actual): actual == width;
			case _: false;
		}
	}

	static function isNumeric(type:HxcIRTypeRef):Bool
		return isInteger(type) || isFloat(type);

	/**
		Recognize the closed String-equality family, including non-null proofs.

		The proof-bearing variants do not change Haxe semantics. They state that
		one or both operands came from a direct String literal, which lets CAST
		omit a null branch that cannot be taken. Listing every spelling here keeps
		unknown operation names fail-closed.
	**/
	static function isStringEqualityOperation(operationId:String):Bool {
		return switch operationId {
			case "haxe.string.equal" | "haxe.string.equal.left-non-null" | "haxe.string.equal.right-non-null" | "haxe.string.equal.non-null" |
				"haxe.string.not-equal" | "haxe.string.not-equal.left-non-null" | "haxe.string.not-equal.right-non-null" | "haxe.string.not-equal.non-null":
				true;
			case _:
				false;
		};
	}

	/** Whether this closed operation spelling claims a non-null left operand. */
	static function stringEqualityRequiresLeftNonNull(operationId:String):Bool {
		return switch operationId {
			case "haxe.string.equal.left-non-null" | "haxe.string.equal.non-null" | "haxe.string.not-equal.left-non-null" | "haxe.string.not-equal.non-null":
				true;
			case _:
				false;
		};
	}

	/** Whether this closed operation spelling claims a non-null right operand. */
	static function stringEqualityRequiresRightNonNull(operationId:String):Bool {
		return switch operationId {
			case "haxe.string.equal.right-non-null" | "haxe.string.equal.non-null" | "haxe.string.not-equal.right-non-null" | "haxe.string.not-equal.non-null":
				true;
			case _:
				false;
		};
	}

	/**
		Prove that one SSA value is a real String rather than Haxe `null`.

		`IRCString` always owns compiler-emitted bytes, including a non-null byte
		address for `""`. Looking through loads or calls would require a separate
		data-flow proof, so this focused optimization deliberately accepts only a
		direct constant definition.
	**/
	static function isStringConstantValue(valueId:String, valueSites:Map<String, HxcIRInstructionSite>):Bool {
		final site = valueSites.get(valueId);
		if (site == null)
			return false;
		return switch site.instruction.kind {
			case IRIOConstant(IRCString(_, _)): true;
			case _:
				false;
		};
	}

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
			available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>, blockId:String, dominanceProofs:HxcIRDominanceProofs):Void {
		switch place {
			case IRPIndex(base, indexValueId):
				switch knownPlaceType(base, available, locals) {
					case IRTFixedArray(_, _, _) | IRTSpan(_, _):
						final key = collectionProofKey(base, indexValueId);
						if (key == null || !boundsProofs.exists(key) && !hasDominatingBoundsProof(key, blockId, dominanceProofs)) {
							add(path, "fixed-array/span access requires a preceding bounds policy for the same collection and index", source);
						}
					case _:
				}
				validateCollectionAccessProof(base, boundsProofs, path, source, available, locals, blockId, dominanceProofs);
			case IRPField(base, _):
				validateCollectionAccessProof(base, boundsProofs, path, source, available, locals, blockId, dominanceProofs);
			case IRPLocal(_) | IRPGlobal(_) | IRPDereference(_):
		}
	}

	/** Accept a proof only when its complete block precedes every path here. */
	static function hasDominatingBoundsProof(key:String, blockId:String, facts:HxcIRDominanceProofs):Bool {
		for (dominator in facts.controlFlow.strictDominatorsOf(blockId)) {
			final proofs = facts.proofsByBlock.get(dominator);
			if (proofs != null && proofs.exists(key))
				return true;
		}
		return false;
	}

	static function collectionProofKey(collection:HxcIRPlace, indexValueId:String):Null<String> {
		final place = collectionPlaceProofKey(collection);
		return place == null ? null : '$place\x00$indexValueId';
	}

	/**
		Give one stable in-block collection place an exact proof identity.

		Owned fixed-array fields are addressed through the receiver value and field
		path. Value IDs are block-local, so these keys cannot accidentally carry a
		pointer proof across blocks; ordinary local/global keys retain dominance.
	**/
	static function collectionPlaceProofKey(place:HxcIRPlace):Null<String> {
		return switch place {
			case IRPLocal(localId): 'local:${localId.length}:$localId';
			case IRPGlobal(globalId): 'global:${globalId.length}:$globalId';
			case IRPDereference(valueId): 'deref:${valueId.length}:$valueId';
			case IRPField(base, fieldName):
				final baseKey = collectionPlaceProofKey(base);
				baseKey == null ? null : 'field:${baseKey.length}:$baseKey:${fieldName.length}:$fieldName';
			case IRPIndex(_, _): null;
		};
	}

	function validateBoundsPolicy(policy:HxcIRBoundsPolicy, path:String, source:HxcSourceSpan, collection:HxcIRPlace, indexValueId:String,
			boundsInstructionId:String, currentBlock:HxcIRBlock, available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>,
			instructionSites:Map<String, HxcIRInstructionSite>, valueSites:Map<String, HxcIRInstructionSite>):Void {
		final knownLength = knownCollectionLength(collection, available, locals, instructionSites);
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

	function knownCollectionLength(collection:HxcIRPlace, available:Map<String, HxcIRTypeRef>, locals:Map<String, HxcIRLocal>,
			instructionSites:Map<String, HxcIRInstructionSite>):Null<Int> {
		final placeType = knownPlaceType(collection, available, locals);
		switch placeType {
			case IRTFixedArray(_, length, _):
				return length;
			case _:
		}
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
									case IRIOInitializeSpan(IRPLocal(targetId), source, _, _) if (targetId == localId):
										initializerCount++;
										sourceLength = switch knownPlaceType(source, available, locals) {
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
			case IRTBool | IRTString | IRTCString | IRTDynamic:
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
					case IRTVoid | IRTFunction(_, _) | IRTString | IRTCString | IRTDynamic:
						add(path, "Void, string views, function, and Dynamic types cannot use a primitive nullable representation", source);
					case _:
				}
				switch representation {
					case IRNTagged:
						switch inner {
							case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_):
							case IRTInstance(instanceId) if (isTaggedOptionalPayloadInstance(instanceId)):
							case _:
								add(path, "tagged nullability requires a direct scalar, closed-record, or closed-enum payload", source);
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
			case IRCCStringLiteral(text, byteLength):
				final actual = HxcUtf8.byteLength(text);
				if (actual == null) {
					add(path, "C string literal is not a valid Unicode-scalar sequence", source);
				} else if (text.indexOf("\x00") != -1) {
					add(path, "C string literal contains an embedded NUL byte", source);
				} else if (byteLength != actual) {
					add(path, 'C string literal records UTF-8 byte length $byteLength but encodes to $actual byte(s)', source);
				}
			case IRCNativeConstant(constantId):
				validateStableId(constantId, '$path.nativeConstant', source);
		}
	}

	function constantMatchesType(value:HxcIRConstant, type:HxcIRTypeRef):Bool {
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
			case IRCCStringLiteral(_, _): type == IRTCString;
			case IRCNativeConstant(_): switch type {
					case IRTBool | IRTInt(_, _) | IRTAbiInteger(_) | IRTFloat(_): true;
					case IRTInstance(instanceId): final instance = typeInstances.get(instanceId); final declaration = instance == null ? null : typeDeclarations.get(instance.declarationId); instance != null && instance.representation == IRRDirect && declaration != null && switch declaration.kind {
							case IRTKExtern | IRTKAggregate(_): true;
							case _: false;
						};
					case _: false;
				};
			case IRCNull:
				switch type {
					case IRTString | IRTPointer(_, true) | IRTNullable(_, _): true;
					case IRTInstance(_): isManagedArrayReference(type) || isManagedIntBoolMap(type) || isManagedStringMapReference(type);
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
			case IRTCString: "cstring-borrowed-literal";
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
			case IRCCStringLiteral(text, byteLength): 'cstring-literal:$byteLength:${escaped(text)}';
			case IRCNativeConstant(constantId): 'native-constant:${escaped(constantId)}';
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
