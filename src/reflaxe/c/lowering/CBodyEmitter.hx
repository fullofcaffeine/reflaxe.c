package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CDetailTimingId;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRFixedArrayPolicy;
import reflaxe.c.ir.HxcIRFixedArrayPolicy.HxcIRFixedArrayStorageDecision;
import reflaxe.c.interop.CImportRegistry.CImportTypeKind;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyRuntimeNames.CBodyRuntimeName;
import reflaxe.c.lowering.CBodyAggregate.CLoweredBodyAggregate;
import reflaxe.c.lowering.CBodyArray.CLoweredBodyArray;
import reflaxe.c.emit.CObjectDescriptorEmitter;
import reflaxe.c.emit.CObjectDescriptorEmitter.CObjectDescriptorSpec;
import reflaxe.c.lowering.CBodyArray.CBodyArrayElementLifecycle;
import reflaxe.c.lowering.CBodyBytes.CPreparedBodyBytes;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyDispatch.CLoweredBodyDispatch;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;
import reflaxe.c.lowering.CBodyIntMap.CPreparedBodyIntMap;
import reflaxe.c.lowering.CBodyOptional.CLoweredBodyOptional;
import reflaxe.c.lowering.CBodyStringMap.CLoweredBodyStringMap;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowCompletion;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowNode;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlan;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlanner;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowRegion;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowSwitchArm;
import reflaxe.c.lowering.CBodyControlFlow.CBodySwitchLabel;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueCoalescingPlan;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueCoalescingPlanner;
import reflaxe.c.lowering.CBodyLowering.CManagedProgramNames;

private enum CBodyEnumCRepresentation {
	CBECNative;
	CBECTagged;
}

private typedef CBodyEmitterVirtualSlot = {
	final id:String;
	final ownerInstanceId:String;
	final parameterTypes:Array<HxcIRTypeRef>;
	final returnType:HxcIRTypeRef;
	final cMember:CIdentifier;
}

private typedef CBodyEmitterVirtualLayout = {
	final id:String;
	final rootInstanceId:String;
	final cTag:CIdentifier;
	final cValueTag:Null<CIdentifier>;
	final cObjectMember:Null<CIdentifier>;
	final cTableMember:Null<CIdentifier>;
	final slots:Array<CBodyEmitterVirtualSlot>;
}

private typedef CBodyEmitterVirtualThunk = {
	final id:String;
	final slot:CBodyEmitterVirtualSlot;
	final implementationFunctionId:String;
	final implementationOwnerInstanceId:String;
	final cName:CIdentifier;
	final receiverName:CIdentifier;
	final argumentNames:Array<CIdentifier>;
}

private typedef CBodyEmitterVirtualTableEntry = {
	final slot:CBodyEmitterVirtualSlot;
	final implementationFunctionId:Null<String>;
	final thunk:Null<CBodyEmitterVirtualThunk>;
}

private typedef CBodyEmitterVirtualTable = {
	final id:String;
	final classInstanceId:String;
	final layout:CBodyEmitterVirtualLayout;
	final cName:CIdentifier;
	final entries:Array<CBodyEmitterVirtualTableEntry>;
}

/** Final callback selected by one HxcIR managed-element cleanup action. */
private typedef CBodyEmitterArrayElementCleanup = {
	final elementType:HxcIRTypeRef;
	final destroyName:CIdentifier;
}

/** Final helpers selected by one tagged enum's managed-Array ownership plan. */
private typedef CBodyEmitterEnumArrayLifecycle = {
	final instanceId:String;
	final retainName:CIdentifier;
	final destroyName:CIdentifier;
}

/** Final helpers selected by one managed closed record's ownership plan. */
private typedef CBodyEmitterAggregateLifecycle = {
	final instanceId:String;
	final retainName:CIdentifier;
	final destroyName:CIdentifier;
}

/** Final helpers selected by one managed tagged optional's ownership plan. */
private typedef CBodyEmitterOptionalLifecycle = {
	final type:HxcIRTypeRef;
	final retainName:CIdentifier;
	final destroyName:CIdentifier;
}

/** Shared callable shape for record and enum retain/destroy helpers. */
private typedef CBodyEmitterProgramLocalLifecycle = {
	final instanceId:String;
	final retainName:CIdentifier;
	final destroyName:CIdentifier;
}

private typedef CBodyEmitterManagedOperation = {
	final retain:CExpr;
	final release:CExpr;
}

/** One C root-array slot fed by one semantic value and projection path. */
private typedef CBodyEmitterManagedRootSlot = {
	final index:Int;
	final root:HxcIRManagedRoot;
}

/** Request-local mutable emission facts shared by structural region recursion. */
private typedef CBodyEmissionState = {
	final values:Map<String, CExpr>;
	final spanValueLengths:Map<String, CExpr>;
	final declared:Map<String, Bool>;
	final referencedValues:Map<String, Bool>;
	final referencedLocals:Map<String, Bool>;
	final referencedSpanLengths:Map<String, Bool>;
	final hoistedLocals:Map<String, Bool>;
	final parameterNames:Map<String, CIdentifier>;
	final localNames:Map<String, CIdentifier>;
	final temporaryNames:Map<String, CIdentifier>;
	final functionNames:Map<String, CIdentifier>;
	final globalNames:Map<String, CIdentifier>;
	final helperNames:Map<String, CIdentifier>;
	final tailArgumentNames:Map<String, Array<CIdentifier>>;
	final labelNames:Map<String, CIdentifier>;
	final spanLengthNames:Map<String, CIdentifier>;
	final nonReturningFunctionIds:Null<Map<String, Bool>>;
	final boundsAbortName:Null<CIdentifier>;
	final lineDirectives:Bool;
	final labeledTargets:Map<String, Bool>;
	final coalescing:CBodyValueCoalescingPlan;
	final managedRootSlots:Map<String, Array<CBodyEmitterManagedRootSlot>>;
	var managedRootArray:Null<CIdentifier>;
	var managedRootFrame:Null<CIdentifier>;
	var terminatedByTailLoop:Bool;
}

/** Lowers the admitted direct-value HxcIR body subset into structural strict C11. */
class CBodyEmitter {
	final aggregateTags:Map<String, CIdentifier> = [];
	final aggregateFieldNames:Map<String, CIdentifier> = [];
	final aggregateFieldTypes:Map<String, HxcIRTypeRef> = [];
	final aggregateFieldOrder:Map<String, Array<String>> = [];
	final aggregateInstanceOrder:Array<String> = [];
	final aggregatesByInstance:Map<String, CLoweredBodyAggregate> = [];
	final aggregateLifecycles:Map<String, CBodyEmitterAggregateLifecycle> = [];
	final enumRepresentations:Map<String, CBodyEnumCRepresentation> = [];
	final enumValueTags:Map<String, CIdentifier> = [];
	final enumDiscriminantTags:Map<String, CIdentifier> = [];
	final enumPayloadUnionTags:Map<String, CIdentifier> = [];
	final enumTagMembers:Map<String, CIdentifier> = [];
	final enumPayloadMembers:Map<String, CIdentifier> = [];
	final enumCaseOrder:Map<String, Array<String>> = [];
	final enumCaseValues:Map<String, Int> = [];
	final enumCaseDiscriminants:Map<String, CIdentifier> = [];
	final enumCasePayloadStructTags:Map<String, CIdentifier> = [];
	final enumCaseUnionMembers:Map<String, CIdentifier> = [];
	final enumPayloadNames:Map<String, Array<String>> = [];
	final enumPayloadFieldNames:Map<String, CIdentifier> = [];
	final enumPayloadFieldTypes:Map<String, HxcIRTypeRef> = [];
	final enumInstanceOrder:Array<String> = [];
	final enumsByInstance:Map<String, CLoweredBodyEnum> = [];
	final enumArrayLifecycles:Map<String, CBodyEmitterEnumArrayLifecycle> = [];
	final classTags:Map<String, CIdentifier> = [];
	final classBaseInstances:Map<String, String> = [];
	final classBaseMembers:Map<String, CIdentifier> = [];
	final classEmptyAnchors:Map<String, CIdentifier> = [];
	final classFieldNames:Map<String, CIdentifier> = [];
	final classFieldTypes:Map<String, HxcIRTypeRef> = [];
	final classFieldOrder:Map<String, Array<String>> = [];
	final classInstanceOrder:Array<String> = [];
	final classesByInstance:Map<String, CLoweredBodyClass> = [];
	final managedDescriptorNames:Map<String, CIdentifier> = [];
	final arrayElementTypes:Map<String, HxcIRTypeRef> = [];
	final arraysByInstance:Map<String, CLoweredBodyArray> = [];
	final intMapInstanceIds:Map<String, Bool> = [];
	final stringMapValueTypes:Map<String, HxcIRTypeRef> = [];
	final stringMapsByInstance:Map<String, CLoweredBodyStringMap> = [];
	final arrayElementCleanups:Map<String, CBodyEmitterArrayElementCleanup> = [];
	final bytesInstanceIds:Map<String, Bool> = [];
	final optionalsByType:Map<String, CLoweredBodyOptional> = [];
	final optionalsByPlan:Map<String, CLoweredBodyOptional> = [];
	final optionalPlanOrder:Array<String> = [];
	final optionalLifecycles:Map<String, CBodyEmitterOptionalLifecycle> = [];
	final classDispatchLayoutIds:Map<String, String> = [];
	final classDispatchHeaders:Map<String, CIdentifier> = [];
	final virtualLayouts:Map<String, CBodyEmitterVirtualLayout> = [];
	final interfaceLayoutsByInstance:Map<String, CBodyEmitterVirtualLayout> = [];
	final virtualSlots:Map<String, CBodyEmitterVirtualSlot> = [];
	final virtualTables:Map<String, CBodyEmitterVirtualTable> = [];
	final virtualThunks:Array<CBodyEmitterVirtualThunk> = [];
	final imports:CLoweredImports;
	final managedProgram:Null<CManagedProgramNames>;

	#if (macro || reflaxe_runtime)
	public function new(?aggregates:Array<CLoweredBodyAggregate>, ?enums:Array<CLoweredBodyEnum>, ?classes:Array<CLoweredBodyClass>,
			?arrays:Array<CLoweredBodyArray>, ?intMaps:Array<CPreparedBodyIntMap>, ?stringMaps:Array<CLoweredBodyStringMap>, ?bytes:Array<CPreparedBodyBytes>,
			?optionals:Array<CLoweredBodyOptional>, ?dispatch:CLoweredBodyDispatch, ?imports:CLoweredImports, ?managedProgram:CManagedProgramNames) {
		this.imports = imports == null ? CLoweredImports.empty() : imports;
		this.managedProgram = managedProgram;
		if (aggregates != null) {
			for (aggregate in aggregates) {
				final instanceId = aggregate.prepared.instanceId;
				aggregatesByInstance.set(instanceId, aggregate);
				aggregateInstanceOrder.push(instanceId);
				aggregateTags.set(instanceId, aggregate.cTag);
				final order:Array<String> = [];
				for (field in aggregate.fields) {
					order.push(field.semanticName);
					aggregateFieldNames.set(aggregateFieldKey(instanceId, field.semanticName), field.cName);
					aggregateFieldTypes.set(aggregateFieldKey(instanceId, field.semanticName), field.type.irType);
				}
				aggregateFieldOrder.set(instanceId, order);
				if (aggregate.prepared.managedLifetime) {
					final lifecycle:CBodyEmitterAggregateLifecycle = {
						instanceId: instanceId,
						retainName: requireAggregateLifecycleName(aggregate.retainName, aggregate, "retain"),
						destroyName: requireAggregateLifecycleName(aggregate.destroyName, aggregate, "destroy")
					};
					final retainId = aggregate.prepared.retainImplementationId();
					final destroyId = aggregate.prepared.destroyImplementationId();
					if (retainId == null || destroyId == null)
						throw new CBodyEmissionError('managed aggregate `$instanceId` lost its lifecycle implementation IDs');
					aggregateLifecycles.set(retainId, lifecycle);
					aggregateLifecycles.set(destroyId, lifecycle);
				}
			}
		}
		for (imported in this.imports.types) {
			if (imported.prepared.kind != CITStruct)
				continue;
			final instanceId = imported.prepared.instanceId;
			final order:Array<String> = [];
			for (field in imported.fields) {
				order.push(field.prepared.name);
				aggregateFieldNames.set(aggregateFieldKey(instanceId, field.prepared.name), field.cName);
				aggregateFieldTypes.set(aggregateFieldKey(instanceId, field.prepared.name), field.prepared.type.irType);
			}
			aggregateFieldOrder.set(instanceId, order);
		}
		if (enums != null) {
			for (value in enums) {
				final instanceId = value.prepared.instanceId;
				enumsByInstance.set(instanceId, value);
				enumInstanceOrder.push(instanceId);
				enumRepresentations.set(instanceId, value.prepared.representation == CBERNativeEnum ? CBECNative : CBECTagged);
				enumValueTags.set(instanceId, value.valueTag);
				enumDiscriminantTags.set(instanceId, value.discriminantTag);
				if (value.payloadUnionTag != null)
					enumPayloadUnionTags.set(instanceId, value.payloadUnionTag);
				if (value.tagMember != null)
					enumTagMembers.set(instanceId, value.tagMember);
				if (value.payloadMember != null)
					enumPayloadMembers.set(instanceId, value.payloadMember);
				final caseOrder:Array<String> = [];
				for (tagCase in value.cases) {
					final caseName = tagCase.prepared.name;
					final caseKey = enumCaseKey(instanceId, caseName);
					caseOrder.push(caseName);
					enumCaseValues.set(caseKey, tagCase.prepared.tagValue);
					enumCaseDiscriminants.set(caseKey, tagCase.discriminant);
					if (tagCase.payloadStructTag != null)
						enumCasePayloadStructTags.set(caseKey, tagCase.payloadStructTag);
					if (tagCase.unionMember != null)
						enumCaseUnionMembers.set(caseKey, tagCase.unionMember);
					final payloadNames:Array<String> = [];
					for (payload in tagCase.payload) {
						payloadNames.push(payload.prepared.name);
						final payloadKey = enumPayloadKey(instanceId, caseName, payload.prepared.name);
						enumPayloadFieldNames.set(payloadKey, payload.cName);
						enumPayloadFieldTypes.set(payloadKey, payload.prepared.storageType());
					}
					enumPayloadNames.set(caseKey, payloadNames);
				}
				enumCaseOrder.set(instanceId, caseOrder);
				if (value.prepared.managedLifetime) {
					final retainName = requireEnumLifecycleName(value.retainName, value, "retain");
					final destroyName = requireEnumLifecycleName(value.destroyName, value, "destroy");
					final lifecycle:CBodyEmitterEnumArrayLifecycle = {
						instanceId: instanceId,
						retainName: retainName,
						destroyName: destroyName
					};
					final retainId = value.prepared.retainImplementationId();
					final destroyId = value.prepared.destroyImplementationId();
					if (retainId == null || destroyId == null)
						throw new CBodyEmissionError('managed enum `$instanceId` lost its lifecycle implementation IDs');
					enumArrayLifecycles.set(retainId, lifecycle);
					enumArrayLifecycles.set(destroyId, lifecycle);
				}
			}
		}
		if (classes != null) {
			for (value in classes) {
				final instanceId = value.prepared.instanceId;
				classesByInstance.set(instanceId, value);
				classInstanceOrder.push(instanceId);
				if (value.prepared.managedByCollector) {
					if (value.descriptorName == null)
						throw new CBodyEmissionError('managed class `$instanceId` lost its descriptor name');
					managedDescriptorNames.set(instanceId, value.descriptorName);
				}
				classTags.set(instanceId, value.cTag);
				if (value.prepared.base != null) {
					classBaseInstances.set(instanceId, value.prepared.base.instanceId);
					if (value.baseMember == null)
						throw new CBodyEmissionError('class `$instanceId` lost its finalized base-prefix member');
					classBaseMembers.set(instanceId, value.baseMember);
				}
				if (value.emptyAnchor != null)
					classEmptyAnchors.set(instanceId, value.emptyAnchor);
				if (value.prepared.dispatchLayoutId != null) {
					if (value.dispatchHeader == null)
						throw new CBodyEmissionError('class `$instanceId` lost its finalized virtual-table header member');
					classDispatchLayoutIds.set(instanceId, value.prepared.dispatchLayoutId);
					classDispatchHeaders.set(instanceId, value.dispatchHeader);
				}
				final order:Array<String> = [];
				for (field in value.fields) {
					order.push(field.prepared.name);
					classFieldNames.set(classFieldKey(instanceId, field.prepared.name), field.cName);
					classFieldTypes.set(classFieldKey(instanceId, field.prepared.name), field.prepared.type.irType);
				}
				classFieldOrder.set(instanceId, order);
			}
		}
		if (arrays != null)
			for (value in arrays) {
				arrayElementTypes.set(value.prepared.instanceId, value.prepared.element.irType);
				arraysByInstance.set(value.prepared.instanceId, value);
				if (value.prepared.managedByCollector) {
					if (value.descriptorName == null)
						throw new CBodyEmissionError('collector-managed Array `${value.prepared.instanceId}` lost its descriptor name');
					managedDescriptorNames.set(value.prepared.instanceId, value.descriptorName);
				}
				final implementationId = value.prepared.destroyImplementationId();
				if (implementationId != null) {
					final destroyName = requireArrayCallbackName(value.destroyName, value, "destroy");
					final existing = arrayElementCleanups.get(implementationId);
					if (existing != null
						&& (typeKey(existing.elementType) != typeKey(value.prepared.element.irType)
							|| existing.destroyName.value != destroyName.value))
						throw new CBodyEmissionError('Array element cleanup `$implementationId` resolved to conflicting callbacks');
					arrayElementCleanups.set(implementationId, {elementType: value.prepared.element.irType, destroyName: destroyName});
				}
			}
		if (stringMaps != null)
			for (value in stringMaps) {
				stringMapsByInstance.set(value.prepared.instanceId, value);
				stringMapValueTypes.set(value.prepared.instanceId, value.prepared.value.irType);
			}
		if (intMaps != null)
			for (value in intMaps)
				intMapInstanceIds.set(value.instanceId, true);
		if (bytes != null)
			for (_ in bytes)
				bytesInstanceIds.set(CPreparedBodyBytes.INSTANCE_ID, true);
		if (optionals != null) {
			for (value in optionals) {
				final key = exactTypeKey(value.prepared.irType());
				if (optionalsByType.exists(key))
					throw new CBodyEmissionError('duplicate direct optional type `$key`');
				optionalsByType.set(key, value);
				optionalsByPlan.set(value.prepared.planId, value);
				optionalPlanOrder.push(value.prepared.planId);
				if (value.prepared.managedLifetime) {
					final lifecycle:CBodyEmitterOptionalLifecycle = {
						type: value.prepared.irType(),
						retainName: requireOptionalLifecycleName(value.retainName, value, "retain"),
						destroyName: requireOptionalLifecycleName(value.destroyName, value, "destroy")
					};
					final retainId = value.prepared.retainImplementationId();
					final destroyId = value.prepared.destroyImplementationId();
					if (retainId == null || destroyId == null)
						throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its lifecycle implementation IDs');
					optionalLifecycles.set(retainId, lifecycle);
					optionalLifecycles.set(destroyId, lifecycle);
				}
			}
			optionalPlanOrder.sort(compareUtf8);
		}
		if (dispatch != null) {
			for (slot in dispatch.slots) {
				virtualSlots.set(slot.prepared.input.id, {
					id: slot.prepared.input.id,
					ownerInstanceId: slot.prepared.ownerInstanceId(),
					parameterTypes: slot.prepared.parameters.map(value -> value.irType),
					returnType: slot.prepared.returnType.irType,
					cMember: slot.cMember
				});
			}
			for (layout in dispatch.layouts) {
				final slots:Array<CBodyEmitterVirtualSlot> = [];
				for (slot in layout.slots)
					slots.push(requireVirtualSlot(slot.prepared.input.id));
				final emittedLayout:CBodyEmitterVirtualLayout = {
					id: layout.prepared.id,
					rootInstanceId: layout.prepared.rootInstanceId(),
					cTag: layout.cTag,
					cValueTag: layout.cValueTag,
					cObjectMember: layout.cObjectMember,
					cTableMember: layout.cTableMember,
					slots: slots
				};
				virtualLayouts.set(layout.prepared.id, emittedLayout);
				if (layout.prepared.isInterface())
					interfaceLayoutsByInstance.set(layout.prepared.rootInstanceId(), emittedLayout);
			}
			final thunksById:Map<String, CBodyEmitterVirtualThunk> = [];
			for (thunk in dispatch.thunks) {
				final value:CBodyEmitterVirtualThunk = {
					id: thunk.prepared.id,
					slot: requireVirtualSlot(thunk.prepared.slot.input.id),
					implementationFunctionId: thunk.prepared.implementationFunctionId,
					implementationOwnerInstanceId: thunk.prepared.implementationOwner.instanceId,
					cName: thunk.cName,
					receiverName: thunk.receiverName,
					argumentNames: thunk.argumentNames.copy()
				};
				thunksById.set(value.id, value);
				virtualThunks.push(value);
			}
			for (table in dispatch.tables) {
				final entries:Array<CBodyEmitterVirtualTableEntry> = [];
				for (entry in table.entries) {
					entries.push({
						slot: requireVirtualSlot(entry.slot.prepared.input.id),
						implementationFunctionId: entry.implementationFunctionId,
						thunk: entry.thunk == null ? null : thunksById.get(entry.thunk.prepared.id)
					});
				}
				final layout = requireVirtualLayout(table.layout.prepared.id);
				virtualTables.set(table.prepared.input.id, {
					id: table.prepared.input.id,
					classInstanceId: table.prepared.classValue.instanceId,
					layout: layout,
					cName: table.cName,
					entries: entries
				});
			}
		}
	}
	#else
	public function new() {
		this.imports = CLoweredImports.empty();
		this.managedProgram = null;
	}
	#end

	public function emitBody(fn:HxcIRFunction, parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			helperNames:Map<String, CIdentifier>, lineDirectives:Bool, tailArgumentNames:Map<String, Array<CIdentifier>>, labelNames:Map<String, CIdentifier>,
			?nonReturningFunctionIds:Map<String, Bool>, ?spanLengthNames:Map<String, CIdentifier>, ?boundsAbortName:CIdentifier):CStmt {
		if (fn.blocks.length == 0 || fn.entryBlockId != fn.blocks[0].id) {
			fail('body lowering requires an entry-first block graph in `${fn.id}`');
		}
		final setupTimer = CPhaseTiming.startDetail(CDTBodySetupAndValuePlanning);
		validateConstructionCleanupRegions(fn);
		final resolvedSpanLengthNames:Map<String, CIdentifier> = spanLengthNames == null ? [] : spanLengthNames;
		final coalescing = new CBodyValueCoalescingPlanner().plan(fn);
		final statements:Array<CStmt> = [];
		final state:CBodyEmissionState = {
			values: [],
			spanValueLengths: [],
			declared: [],
			referencedValues: referencedValueIds(fn),
			referencedLocals: referencedLocalIds(fn),
			referencedSpanLengths: referencedSpanLengthIds(fn),
			hoistedLocals: [],
			parameterNames: parameterNames,
			localNames: localNames,
			temporaryNames: temporaryNames,
			functionNames: functionNames,
			globalNames: globalNames,
			helperNames: helperNames,
			tailArgumentNames: tailArgumentNames,
			labelNames: labelNames,
			spanLengthNames: resolvedSpanLengthNames,
			nonReturningFunctionIds: nonReturningFunctionIds,
			boundsAbortName: boundsAbortName,
			lineDirectives: lineDirectives,
			labeledTargets: [],
			coalescing: coalescing,
			managedRootSlots: [],
			managedRootArray: null,
			managedRootFrame: null,
			terminatedByTailLoop: false
		};
		prepareManagedRootFrame(statements, state, fn);
		prepareHoistedCleanupLocals(statements, state, fn);
		for (parameter in fn.parameters) {
			final name = requireParameterName(parameterNames, parameter.id, fn.id);
			state.values.set(parameter.id, EIdentifier(name));
			switch parameter.type {
				case IRTSpan(_, _):
					state.spanValueLengths.set(parameter.id, EIdentifier(requireSpanLengthName(resolvedSpanLengthNames, parameter.id, fn.id)));
				case _:
			}
			if (!state.referencedValues.exists(parameter.id)) {
				addLineDirective(statements, parameter.source, lineDirectives);
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
				final spanLength = state.spanValueLengths.get(parameter.id);
				if (spanLength != null)
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), spanLength)));
			}
		}
		CPhaseTiming.stopDetail(setupTimer);
		final controlFlowTimer = CPhaseTiming.startDetail(CDTBodyControlFlowPlanning);
		final plan = new CBodyControlFlowPlanner().plan(fn);
		CPhaseTiming.stopDetail(controlFlowTimer);
		final emissionTimer = CPhaseTiming.startDetail(CDTBodyCASTEmission);
		switch plan {
			case CCFStructured(root, labeledTargets):
				for (target in labeledTargets)
					state.labeledTargets.set(target, true);
				emitRegion(statements, root, state, fn);
			case CCFLegacyIrreducible(_):
				emitLegacyGraph(statements, state, fn);
		}
		CPhaseTiming.stopDetail(emissionTimer);
		if (state.terminatedByTailLoop) {
			if (state.managedRootFrame != null)
				fail('managed-root function `${fn.id}` cannot use the tail-loop rewrite until iteration-owned root reset is explicit');
			if (fn.blocks.length != 1) {
				fail('tail-loop lowering in `${fn.id}` requires one HxcIR block');
			}
			return SBlock([SWhile(EInt(CIntegerLiteral.decimal("1")), SBlock(statements))]);
		}
		return SBlock(statements);
	}

	/**
		Hoist a cleanup-owned local whose initializer lives below the entry block.

		HxcIR locals belong to the whole function even when structured C places an
		`if`, switch, or loop around their initializing instruction. A return or
		failure edge inside that region may run the typed cleanup action. Declaring
		the C local only inside the nested block would then make shared cleanup code
		name an out-of-scope identifier. Hoisting only those cleanup-owned locals
		keeps ordinary locals narrow while giving every legal cleanup edge one C
		declaration. The inert `{0}` initializer keeps strict C/C++ definite-use
		analysis honest on sibling paths; HxcIR still decides whether a cleanup step
		runs, and every admitted runtime/lifecycle release treats that zero state as
		empty. The source initializer becomes an assignment at its original position.
	**/
	function prepareHoistedCleanupLocals(statements:Array<CStmt>, state:CBodyEmissionState, fn:HxcIRFunction):Void {
		final initializedOutsideEntry:Map<String, Bool> = [];
		for (block in fn.blocks) {
			if (block.id == fn.entryBlockId)
				continue;
			for (instruction in block.instructions)
				switch instruction.kind {
					case IRIOInitialize(IRPLocal(localId), _, IRISUninitialized, IRISInitialized):
						initializedOutsideEntry.set(localId, true);
					case _:
				}
		}
		final cleanupLocals = cleanupReferencedLocalIds(fn);
		final candidates:Array<String> = [];
		for (localId in cleanupLocals.keys())
			if (initializedOutsideEntry.exists(localId))
				candidates.push(localId);
		candidates.sort(compareUtf8);
		for (localId in candidates) {
			final local = requireLocal(fn, localId);
			final declaration = typedDeclarator(local.type, DName(requireLocalName(state.localNames, localId, fn.id)));
			addLineDirective(statements, local.source, state.lineDirectives);
			statements.push(SDecl({
				storage: [],
				alignments: [],
				type: declaration.type,
				declarator: declaration.declarator,
				initializer: IList([{designators: [], value: IExpr(EInt(CIntegerLiteral.decimal("0")))}]),
				attributes: []
			}));
			state.declared.set(localId, true);
			state.hoistedLocals.set(localId, true);
		}
	}

	/**
		Declare and register one exact root frame before the function can allocate.

		Parameter roots enter with their incoming value. Instruction-result slots
		start null and are updated immediately after the defining instruction. The
		frame therefore never contains uninitialized bytes or a guessed C address.
	**/
	function prepareManagedRootFrame(statements:Array<CStmt>, state:CBodyEmissionState, fn:HxcIRFunction):Void {
		final roots = fn.managedRoots == null ? [] : fn.managedRoots;
		if (roots.length == 0)
			return;
		if (managedProgram == null)
			fail('managed-root function `${fn.id}` has no finalized executable collector plan');
		final rootArray = managedProgram.rootArrays.get(fn.id);
		final rootFrame = managedProgram.rootFrames.get(fn.id);
		if (rootArray == null || rootFrame == null)
			fail('managed-root function `${fn.id}` lost its finalized frame names');
		state.managedRootArray = rootArray;
		state.managedRootFrame = rootFrame;
		final initializers:Array<CInitializerItem> = [];
		for (index => root in roots) {
			var slots = state.managedRootSlots.get(root.valueId);
			if (slots == null) {
				slots = [];
				state.managedRootSlots.set(root.valueId, slots);
			}
			slots.push({index: index, root: root});
			var initial:CExpr = ENull;
			for (parameter in fn.parameters)
				if (parameter.id == root.valueId)
					initial = managedRootPointer(EIdentifier(requireParameterName(state.parameterNames, parameter.id, fn.id)), parameter.type,
						root.projections, fn.id);
			initializers.push({designators: [], value: IExpr(initial)});
		}
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: new CType(TVoid, [QConst]),
			declarator: DArray(DPointer(DName(rootArray), []), ABFixed(EInt(CIntegerLiteral.decimal(Std.string(roots.length)))), []),
			initializer: IList(initializers),
			attributes: []
		}));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: new CType(TStruct(CBodyRuntimeNames.identifier(CBRNGcRootFrameType))),
			declarator: DName(rootFrame),
			initializer: IExpr(EIdentifier(CBodyRuntimeNames.identifier(CBRNGcRootFrameInitializer))),
			attributes: []
		}));
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNGcRootFramePush)), [
			EUnary(AddressOf, EIdentifier(managedProgram.thread)),
			EIdentifier(rootArray),
			EInt(CIntegerLiteral.decimal(Std.string(roots.length))),
			EUnary(AddressOf, EIdentifier(rootFrame))
		]), state.boundsAbortName, 'managed-root-frame-push', fn.id);
	}

	/** Publish one newly defined managed value to its already-registered slot. */
	function emitManagedRootUpdate(statements:Array<CStmt>, state:CBodyEmissionState, valueId:String, fn:HxcIRFunction):Void {
		final functionId = fn.id;
		final slots = state.managedRootSlots.get(valueId);
		if (slots == null)
			return;
		final rootArray = state.managedRootArray;
		if (rootArray == null)
			fail('managed value `$valueId` in `$functionId` has no root array');
		final type = valueType(fn, valueId);
		if (type == null)
			fail('managed value `$valueId` in `$functionId` lost its HxcIR type');
		for (slot in slots)
			statements.push(SExpr(EBinary(Assign, EIndex(EIdentifier(rootArray), EInt(CIntegerLiteral.decimal(Std.string(slot.index)))),
				managedRootPointer(requireValue(state.values, valueId, functionId), type, slot.root.projections, functionId))));
	}

	/** Unlink a function frame after semantic cleanup and before returning. */
	function emitManagedRootFramePop(statements:Array<CStmt>, fn:HxcIRFunction, boundsAbortName:Null<CIdentifier>):Void {
		if (fn.managedRoots == null || fn.managedRoots.length == 0)
			return;
		if (managedProgram == null)
			fail('managed-root function `${fn.id}` has no finalized executable collector plan');
		final frame = managedProgram.rootFrames.get(fn.id);
		if (frame == null)
			fail('managed-root function `${fn.id}` lost its finalized frame name');
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNGcRootFramePop)), [EUnary(AddressOf, EIdentifier(frame))]),
			boundsAbortName, 'managed-root-frame-pop', fn.id);
	}

	function managedRootPointer(value:CExpr, type:HxcIRTypeRef, projections:Array<HxcIRManagedRootProjection>, functionId:String):CExpr {
		var current = value;
		var currentType = type;
		final guards:Array<CExpr> = [];
		for (projection in projections)
			switch projection {
				case IRMRPAggregateField(instanceId, fieldName):
					switch currentType {
						case IRTInstance(actual) if (actual == instanceId):
						case _: fail('managed root in `$functionId` applies record `$instanceId` to `${typeKey(currentType)}`');
					}
					current = EMember(current, requireAggregateFieldName(instanceId, fieldName, "managed-root", functionId), false);
					currentType = requireAggregateFieldIrType(instanceId, fieldName);
				case IRMRPTagPayload(instanceId, tagName, payloadIndex):
					switch currentType {
						case IRTInstance(actual) if (actual == instanceId):
						case _: fail('managed root in `$functionId` applies enum `$instanceId` to `${typeKey(currentType)}`');
					}
					guards.push(EBinary(Equal, enumTagExpression(current, instanceId), EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName))));
					final payloadNames = requireEnumPayloadNames(instanceId, tagName);
					if (payloadIndex < 0 || payloadIndex >= payloadNames.length)
						fail('managed root in `$functionId` has invalid `$tagName` payload index `$payloadIndex`');
					final payloadName = payloadNames[payloadIndex];
					current = EMember(EMember(EMember(current, requireEnumPayloadMember(instanceId), false), requireEnumCaseUnionMember(instanceId, tagName),
						false),
						requireEnumPayloadFieldName(instanceId, tagName, payloadName), false);
					currentType = requireEnumPayloadFieldType(instanceId, tagName, payloadName);
				case IRMRPNullablePayload:
					final optional = requireOptional(currentType);
					guards.push(EMember(current, optional.presenceName, false));
					current = EMember(current, optional.payloadName, false);
					currentType = optional.prepared.payload.irType;
			}
		var pointer:CExpr = ECast(new CType(TVoid, [QConst]), DPointer(DName(null), []), current);
		if (guards.length > 0) {
			var condition = guards[0];
			for (index in 1...guards.length)
				condition = EBinary(LogicalAnd, condition, guards[index]);
			pointer = EConditional(condition, pointer, ENull);
		}
		return pointer;
	}

	/** The legacy graph form is retained only for a planner-proven irreducible CFG. */
	function emitLegacyGraph(statements:Array<CStmt>, state:CBodyEmissionState, fn:HxcIRFunction):Void {
		final entryIsTargeted = hasIncomingEdge(fn, fn.entryBlockId);
		for (index in 0...fn.blocks.length) {
			final block = fn.blocks[index];
			final terminated = emitBlockInstructions(statements, block, state, fn, index > 0 || entryIsTargeted);
			if (terminated)
				continue;
			final terminator = requireTerminator(block.terminator, fn.id);
			addLineDirective(statements, terminator.source, state.lineDirectives);
			emitTerminator(statements, state.values, terminator, state.labelNames, fn, state.boundsAbortName, state.localNames, state.globalNames,
				state.spanLengthNames);
		}
	}

	static function hasIncomingEdge(fn:HxcIRFunction, targetBlockId:String):Bool {
		for (block in fn.blocks) {
			if (block.terminator == null)
				continue;
			final targets:Array<String> = switch block.terminator.kind {
				case IRTJump(edge): [edge.targetBlockId];
				case IRTBranch(_, whenTrue, whenFalse): [whenTrue.targetBlockId, whenFalse.targetBlockId];
				case IRTSwitch(_, cases, defaultEdge): cases.map(item -> item.edge.targetBlockId).concat([defaultEdge.targetBlockId]);
				case IRTTagSwitch(_, cases, defaultEdge):
					final values = cases.map(item -> item.edge.targetBlockId);
					if (defaultEdge != null)
						values.push(defaultEdge.targetBlockId);
					values;
				case IRTThrow(_, failure):
					switch failure.target {
						case IRFTBlock(target): [target];
						case IRFTPropagate | IRFTAbort: [];
					}
				case IRTReturn(_, _) | IRTUnreachable: [];
			};
			if (targets.indexOf(targetBlockId) != -1)
				return true;
		}
		return false;
	}

	/** Emit one already-validated structural region without rediscovering CFG facts. */
	function emitRegion(statements:Array<CStmt>, region:CBodyControlFlowRegion, state:CBodyEmissionState, fn:HxcIRFunction):Bool {
		for (node in region.nodes) {
			if (emitControlFlowNode(statements, node, state, fn))
				return true;
		}
		emitRegionCompletion(statements, region.completion, state, fn);
		return false;
	}

	function emitControlFlowNode(statements:Array<CStmt>, node:CBodyControlFlowNode, state:CBodyEmissionState, fn:HxcIRFunction):Bool {
		return switch node {
			case CFNBlock(blockId):
				emitBlockInstructions(statements, requireBlock(fn, blockId), state, fn, false);
			case CFNIf(blockId, conditionValueId, whenTrue, whenFalse, _):
				final block = requireBlock(fn, blockId);
				if (emitBlockInstructions(statements, block, state, fn, false))
					return true;
				final condition = requireValue(state.values, conditionValueId, fn.id);
				final trueStatements:Array<CStmt> = [];
				final falseStatements:Array<CStmt> = [];
				emitRegion(trueStatements, whenTrue, state, fn);
				emitRegion(falseStatements, whenFalse, state, fn);
				addTerminatorLineDirective(statements, block, state.lineDirectives, fn.id);
				if (trueStatements.length == 0 && falseStatements.length > 0) {
					statements.push(SIf(EUnary(LogicalNot, condition), SBlock(falseStatements), null));
				} else {
					statements.push(SIf(condition, SBlock(trueStatements), falseStatements.length == 0 ? null : SBlock(falseStatements)));
				}
				false;
			case CFNWhile(_, decisionBlockId, conditionValueId, continuesWhenTrue, conditionRegion, body, _):
				final loopStatements:Array<CStmt> = [];
				emitRegion(loopStatements, conditionRegion, state, fn);
				final decision = requireBlock(fn, decisionBlockId);
				addTerminatorLineDirective(loopStatements, decision, state.lineDirectives, fn.id);
				final condition = requireValue(state.values, conditionValueId, fn.id);
				loopStatements.push(SIf(continuesWhenTrue ? EUnary(LogicalNot, condition) : condition, SBreak, null));
				emitRegion(loopStatements, body, state, fn);
				statements.push(SWhile(EInt(CIntegerLiteral.decimal("1")), SBlock(loopStatements)));
				false;
			case CFNDoWhile(_, decisionBlockId, conditionValueId, continuesWhenTrue, body, conditionRegion, _):
				final loopStatements:Array<CStmt> = [];
				emitRegion(loopStatements, body, state, fn);
				emitRegion(loopStatements, conditionRegion, state, fn);
				final decision = requireBlock(fn, decisionBlockId);
				addTerminatorLineDirective(loopStatements, decision, state.lineDirectives, fn.id);
				final condition = requireValue(state.values, conditionValueId, fn.id);
				loopStatements.push(SIf(continuesWhenTrue ? EUnary(LogicalNot, condition) : condition, SBreak, null));
				statements.push(SDoWhile(SBlock(loopStatements), EInt(CIntegerLiteral.decimal("1"))));
				false;
			case CFNSwitch(blockId, valueId, arms, _):
				emitStructuredSwitch(statements, blockId, valueId, arms, false, state, fn);
			case CFNTagSwitch(blockId, valueId, arms, _):
				emitStructuredSwitch(statements, blockId, valueId, arms, true, state, fn);
		};
	}

	function emitStructuredSwitch(statements:Array<CStmt>, blockId:String, valueId:String, arms:Array<CBodyControlFlowSwitchArm>, tagged:Bool,
			state:CBodyEmissionState, fn:HxcIRFunction):Bool {
		final block = requireBlock(fn, blockId);
		if (emitBlockInstructions(statements, block, state, fn, false))
			return true;
		final emittedCases:Array<CCase> = [];
		for (arm in arms) {
			final values:Array<CExpr> = [];
			var isDefault = false;
			for (label in arm.labels) {
				switch label {
					case CSLValue(value) if (!tagged):
						values.push(constantExpression(value));
					case CSLTag(tagName) if (tagged):
						final instanceId = requireEnumInstanceId(valueType(fn, valueId), "terminator", fn.id);
						values.push(EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName)));
					case CSLDefault:
						isDefault = true;
					case _:
						fail('structured switch `$blockId` in `${fn.id}` mixes value and tag labels');
				}
			}
			final body:Array<CStmt> = [];
			emitRegion(body, arm.body, state, fn);
			if (arm.body.completion == CFCFallthrough)
				body.push(SBreak);
			emittedCases.push({values: values, isDefault: isDefault, body: body});
		}
		final subject = if (tagged) {
			final instanceId = requireEnumInstanceId(valueType(fn, valueId), "terminator", fn.id);
			enumTagExpression(requireValue(state.values, valueId, fn.id), instanceId);
		} else {
			requireValue(state.values, valueId, fn.id);
		};
		addTerminatorLineDirective(statements, block, state.lineDirectives, fn.id);
		statements.push(SSwitch(subject, emittedCases));
		return false;
	}

	function emitRegionCompletion(statements:Array<CStmt>, completion:CBodyControlFlowCompletion, state:CBodyEmissionState, fn:HxcIRFunction):Void {
		switch completion {
			case CFCFallthrough | CFCClosed:
			case CFCReturn(ownerBlockId) | CFCThrow(ownerBlockId):
				final block = requireBlock(fn, ownerBlockId);
				final terminator = requireTerminator(block.terminator, fn.id);
				addLineDirective(statements, terminator.source, state.lineDirectives);
				emitTerminator(statements, state.values, terminator, state.labelNames, fn, state.boundsAbortName, state.localNames, state.globalNames,
					state.spanLengthNames);
			case CFCUnreachable(ownerBlockId):
				final block = requireBlock(fn, ownerBlockId);
				addTerminatorLineDirective(statements, block, state.lineDirectives, fn.id);
				statements.push(SExpr(ECall(EIdentifier(requireBoundsAbortName(state.boundsAbortName, "unreachable", fn.id)), [])));
			case CFCBreak(ownerBlockId, _):
				addTerminatorLineDirective(statements, requireBlock(fn, ownerBlockId), state.lineDirectives, fn.id);
				statements.push(SBreak);
			case CFCContinue(ownerBlockId, _):
				addTerminatorLineDirective(statements, requireBlock(fn, ownerBlockId), state.lineDirectives, fn.id);
				statements.push(SContinue);
			case CFCGoto(ownerBlockId, targetBlockId, _):
				addTerminatorLineDirective(statements, requireBlock(fn, ownerBlockId), state.lineDirectives, fn.id);
				statements.push(SGoto(requireLabelName(state.labelNames, targetBlockId, fn.id)));
		}
	}

	function emitBlockInstructions(statements:Array<CStmt>, block:HxcIRBlock, state:CBodyEmissionState, fn:HxcIRFunction, forceLabel:Bool):Bool {
		if (forceLabel || state.labeledTargets.exists(block.id))
			statements.push(SLabel(requireLabelName(state.labelNames, block.id, fn.id), SEmpty));
		var terminatedByNonReturningCall = false;
		for (instruction in block.instructions) {
			switch instruction.kind {
				case IRIOConstant(value):
					final result = requireResult(instruction, fn.id);
					state.values.set(result.id, constantExpressionForType(value, result.type));
				case IRIOFunctionReference(functionId):
					final result = requireResult(instruction, fn.id);
					state.values.set(result.id, EIdentifier(requireFunctionName(state.functionNames, functionId, fn.id)));
				case IRIOLoad(place):
					final result = requireResult(instruction, fn.id);
					switch result.type {
						case IRTSpan(_, _):
							emitSpanLoad(statements, state.values, state.spanValueLengths, state.referencedValues, instruction, place, fn, state.localNames,
								state.globalNames, state.spanLengthNames, state.lineDirectives);
						case _:
							emitLoad(statements, state.values, state.referencedValues, instruction,
								placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values), state.temporaryNames,
								state.lineDirectives, fn.id, state.coalescing.shouldInlineSequencedLoad(result.id));
					}
				case IRIOAddress(place) | IRIOBorrowClassField(place):
					emitAddress(statements, state.values, state.referencedValues, instruction, place, fn, state.localNames, state.globalNames,
						state.spanLengthNames, state.temporaryNames, state.lineDirectives,
						state.coalescing.shouldInlineSequencedAddress(requireResult(instruction, fn.id).id));
				case IRIOConstructAggregate(instanceId, fields):
					emitAggregateConstruction(statements, state.values, state.referencedValues, instruction, instanceId, fields, state.temporaryNames,
						state.lineDirectives, fn.id, state.coalescing.shouldInlinePure(requireResult(instruction, fn.id).id));
				case IRIOConstructInterface(interfaceInstanceId, objectValueId, tableId):
					emitInterfaceConstruction(statements, state.values, state.referencedValues, instruction, interfaceInstanceId, objectValueId, tableId,
						state.temporaryNames, state.lineDirectives, fn.id);
				case IRIOProject(valueId, fieldName):
					emitAggregateProjection(statements, state.values, state.referencedValues, instruction, valueId, fieldName, fn, state.temporaryNames,
						state.lineDirectives, state.coalescing.shouldInlinePure(requireResult(instruction, fn.id).id));
				case IRIOConstructTag(instanceId, tagName, payload):
					emitEnumConstruction(statements, state.values, state.referencedValues, instruction, instanceId, tagName, payload, state.temporaryNames,
						state.lineDirectives, fn.id, state.coalescing.shouldInlinePure(requireResult(instruction, fn.id).id));
				case IRIOMatchTag(valueId, tagName):
					emitEnumMatch(statements, state.values, state.referencedValues, instruction, valueId, tagName, fn, state.temporaryNames,
						state.lineDirectives, state.coalescing.shouldInlinePure(requireResult(instruction, fn.id).id));
				case IRIOProjectTag(valueId, tagName, payloadIndex, IRTCPCheckedAbort(_, _)):
					emitEnumProjection(statements, state.values, state.referencedValues, instruction, valueId, tagName, payloadIndex, fn,
						state.temporaryNames, state.boundsAbortName, state.lineDirectives);
				case IRIODeclareUninitialized(IRPLocal(localId)):
					emitUninitializedDeclaration(statements, state.declared, state.referencedLocals, instruction, localId, fn, state.localNames,
						state.lineDirectives);
				case IRIODeclareManagedCarrier(IRPLocal(localId), _):
					emitUninitializedDeclaration(statements, state.declared, state.referencedLocals, instruction, localId, fn, state.localNames,
						state.lineDirectives);
				case IRIOAcquireManagedCarrier(place, valueId, acquisition):
					emitManagedCarrierAcquisition(statements, state.values, instruction, place, valueId, acquisition, fn, state.localNames, state.globalNames,
						state.spanLengthNames, state.boundsAbortName, state.lineDirectives);
				case IRIOMoveManagedCarrier(place):
					emitLoad(statements, state.values, state.referencedValues, instruction,
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values), state.temporaryNames,
						state.lineDirectives, fn.id, false);
				case IRIODefaultInitialize(IRPLocal(localId), from, to):
					emitDefaultInitialize(statements, state.declared, state.referencedLocals, instruction, localId, from, to, fn, state.localNames,
						state.lineDirectives);
				case IRIOBindVirtualTable(place, tableId):
					emitBindVirtualTable(statements, instruction, place, tableId, fn, state.localNames, state.globalNames, state.spanLengthNames,
						state.values, state.lineDirectives);
				case IRIOInitialize(IRPLocal(localId), valueId, IRISUninitialized, IRISInitialized):
					switch requireLocal(fn, localId).type {
						case IRTSpan(_, _):
							emitSpanValueInitialize(statements, state.values, state.spanValueLengths, state.declared, state.referencedLocals,
								state.referencedSpanLengths, instruction, localId, valueId, fn, state.localNames, state.spanLengthNames, state.lineDirectives);
						case _:
							emitInitialize(statements, state.values, state.declared, state.referencedLocals, state.hoistedLocals, instruction, localId,
								valueId, fn, state.localNames, state.lineDirectives);
					}
				case IRIOInitialize(IRPGlobal(globalId), valueId, IRISUninitialized, IRISInitialized):
					if (instruction.result != null)
						fail('global initializer `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
					addLineDirective(statements, instruction.source, state.lineDirectives);
					statements.push(SExpr(EBinary(Assign,
						placeExpression(IRPGlobal(globalId), fn, state.localNames, state.globalNames, state.spanLengthNames, state.values),
						requireValue(state.values, valueId, fn.id))));
				case IRIOInitializeFixedArray(IRPLocal(localId), valueIds, IRISUninitialized, IRISInitialized):
					emitFixedArrayInitialize(statements, state.values, state.declared, state.referencedLocals, instruction, localId, valueIds, fn,
						state.localNames, state.lineDirectives);
				case IRIOZeroInitializeFixedArray(IRPLocal(localId), IRISUninitialized, IRISInitialized):
					emitZeroFixedArrayInitialize(statements, state.declared, state.referencedLocals, instruction, localId, fn, state.localNames,
						state.lineDirectives);
				case IRIOInitializeSpan(IRPLocal(localId), sourceArray, IRISUninitialized, IRISInitialized):
					emitSpanInitialize(statements, state.values, state.declared, state.referencedLocals, state.referencedSpanLengths, instruction, localId,
						sourceArray, fn, state.localNames, state.spanLengthNames, state.globalNames, state.lineDirectives);
				case IRIOBoundsCheck(collection, indexValueId, IRBPCheckedAbort(_, _)):
					emitBoundsCheck(statements, state.values, instruction, collection, indexValueId, fn, state.localNames, state.globalNames,
						state.spanLengthNames, state.boundsAbortName, state.lineDirectives);
				case IRIOBoundsCheck(_, _, IRBPStaticProof(_, _) | IRBPLoopGuarded(_, _, _)):
					// The semantic proof remains reviewable in HxcIR; no redundant C check survives.
				case IRIONullCheck(valueId, IRNCPCheckedAbort(_, _)):
					emitNullCheck(statements, state.values, instruction, valueId, state.boundsAbortName, state.lineDirectives, fn);
				case IRIOStore(place, valueId):
					if (instruction.result != null)
						fail('store `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
					addLineDirective(statements, instruction.source, state.lineDirectives);
					statements.push(SExpr(EBinary(Assign,
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values),
						requireValue(state.values, valueId, fn.id))));
				case IRIOUnary(operationId, valueId, implementation):
					final result = requireResult(instruction, fn.id);
					final expression = if (operationId == "haxe.direct-optional.is-null"
						|| operationId == "haxe.direct-optional.is-not-null") {
						final optional = requireOptional(valueType(fn, valueId));
						final present = EMember(requireValue(state.values, valueId, fn.id), optional.presenceName, false);
						operationId == "haxe.direct-optional.is-null" ? EUnary(LogicalNot, present) : present;
					} else if (operationId == "haxe.string.is-null" || operationId == "haxe.string.is-not-null") {
						final data = EMember(requireValue(state.values, valueId, fn.id), new CIdentifier("data"), false);
						EBinary(operationId == "haxe.string.is-null" ? Equal : NotEqual, data, ENull);
					} else {
						operationExpression(operationId, implementation, [requireValue(state.values, valueId, fn.id)], state.helperNames, instruction.id,
							fn.id);
					};
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOBinary(operationId, leftValueId, rightValueId, implementation):
					final result = requireResult(instruction, fn.id);
					final expression = operationExpression(operationId, implementation, [
						requireValue(state.values, leftValueId, fn.id),
						requireValue(state.values, rightValueId, fn.id)
					], state.helperNames, instruction.id, fn.id);
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOConvert(valueId, IRCRepresentation, targetType, IRIStatic, null):
					final result = requireResult(instruction, fn.id);
					final expression = classUpcastExpression(requireValue(state.values, valueId, fn.id), valueType(fn, valueId), targetType, instruction.id,
						fn.id);
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOConvert(valueId, IRCNullableInject, targetType, IRIStatic, null):
					final result = requireResult(instruction, fn.id);
					final expression = switch targetType {
						case IRTNullable(_, IRNTagged): directOptionalValueExpression(targetType, requireValue(state.values, valueId, fn.id));
						case _: requireValue(state.values, valueId, fn.id);
					};
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOConvert(valueId, IRCNullableUnwrap, _, IRIStatic, null):
					final result = requireResult(instruction, fn.id);
					final optional = requireOptional(valueType(fn, valueId));
					final expression = EMember(requireValue(state.values, valueId, fn.id), optional.payloadName, false);
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOConvert(valueId, kind, targetType, IRIStatic, null):
					final result = requireResult(instruction, fn.id);
					final expression = switch kind {
						case IRCNumericExact | IRCNumericRoundBinary32 | IRCNumericWidenBinary64 | IRCNumericWrapping:
							ECast(cType(targetType), DName(null), requireValue(state.values, valueId, fn.id));
						case _:
							fail('conversion `${instruction.id}` in `${fn.id}` is outside the admitted direct primitive conversion subset');
					};
					state.values.set(result.id, expression);
					if (!state.referencedValues.exists(result.id)) {
						addLineDirective(statements, instruction.source, state.lineDirectives);
						statements.push(SExpr(ECast(new CType(TVoid), DName(null), expression)));
					}
				case IRIOConvert(valueId, kind, _, IRIProgramLocal(helperId), null):
					final result = requireResult(instruction, fn.id);
					switch kind {
						case IRCNumericExact | IRCNumericWrapping | IRCNumericSaturating:
						case _:
							fail('program-local conversion `${instruction.id}` in `${fn.id}` has unsupported kind `$kind`');
					}
					final expression = helperCall(helperId, [requireValue(state.values, valueId, fn.id)], state.helperNames, instruction.id, fn.id);
					recordPureResult(statements, state.values, state.referencedValues, instruction, result, expression, state.lineDirectives, fn.id);
				case IRIOCall(call):
					if (isNonReturningSelfCall(fn.id, call, state.nonReturningFunctionIds)) {
						emitTailLoopCall(statements, state.values, instruction, call, fn, state.parameterNames, state.tailArgumentNames, state.lineDirectives);
						terminatedByNonReturningCall = true;
						state.terminatedByTailLoop = true;
					} else {
						terminatedByNonReturningCall = emitCall(statements, state.values, state.spanValueLengths, state.referencedValues, instruction, call,
							state.temporaryNames, state.functionNames, state.localNames, state.globalNames, state.spanLengthNames, state.lineDirectives,
							state.nonReturningFunctionIds, state.boundsAbortName, fn);
					}
				case IRIOAllocate(type, IRAOwned, IRIRuntime("alloc"), {target: IRFTAbort}):
					emitOwnedAllocation(statements, state.values, state.referencedValues, instruction, type, state.temporaryNames, state.lineDirectives,
						state.boundsAbortName, fn);
				case IRIOAllocate(type, IRAShared, IRIRuntime("gc"), {target: IRFTAbort}):
					emitCollectorAllocation(statements, state.values, state.referencedValues, instruction, type, state.temporaryNames, state.lineDirectives,
						state.boundsAbortName, fn);
				case IRIORetain(place, IRIRuntime("array")):
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayRetain)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORetain(place, IRIRuntime("string-map")):
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapRetain)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORetain(place, IRIRuntime("int-map")):
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapRetain)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORetain(place, IRIRuntime("bytes")):
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNBytesRetain)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORetain(place, IRIRuntime("string")):
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringRetain)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORetain(place, IRIProgramLocal(implementationId)):
					final lifecycle = programLocalLifecycle(implementationId, instruction.id, fn.id);
					addLineDirective(statements, instruction.source, state.lineDirectives);
					emitStatusAbort(statements, ECall(EIdentifier(lifecycle.retainName), [
						EUnary(AddressOf, placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values))
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIRuntime("array")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayRelease)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIRuntime("string-map")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapRelease)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIRuntime("int-map")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapRelease)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIRuntime("bytes")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNBytesRelease)), [
						placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values)
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIRuntime("string")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringRelease)), [
						EUnary(AddressOf, placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values))
					]), state.boundsAbortName, instruction.id, fn.id);
				case IRIORelease(place, IRIProgramLocal(implementationId)):
					final cleanup = arrayElementCleanups.get(implementationId);
					if (cleanup != null)
						statements.push(SExpr(ECall(EIdentifier(cleanup.destroyName), [
							ENull,
							EUnary(AddressOf, placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values))
						])));
					else {
						final lifecycle = programLocalLifecycle(implementationId, instruction.id, fn.id);
						statements.push(SExpr(ECall(EIdentifier(lifecycle.destroyName), [
							EUnary(AddressOf, placeExpression(place, fn, state.localNames, state.globalNames, state.spanLengthNames, state.values))
						])));
					}
				case IRIOLifetime(_, _, _, _):
					// Direct stack-object lifetime transitions are semantic proof only.
				case _:
					fail('HxcIR instruction `${instruction.id}` in `${fn.id}` is outside the sequenced direct-value function subset');
			}
			if (instruction.result != null)
				emitManagedRootUpdate(statements, state, instruction.result.id, fn);
			if (terminatedByNonReturningCall)
				break;
		}
		if (terminatedByNonReturningCall) {
			final terminator = requireTerminator(block.terminator, fn.id);
			switch terminator.kind {
				case IRTReturn(_, cleanup) if (cleanup.length == 0):
				case _:
					fail('non-returning call in `${fn.id}` cannot replace its non-return terminator or cleanup');
			}
		}
		return terminatedByNonReturningCall;
	}

	static function addTerminatorLineDirective(statements:Array<CStmt>, block:HxcIRBlock, enabled:Bool, functionId:String):Void {
		final terminator = requireTerminator(block.terminator, functionId);
		addLineDirective(statements, terminator.source, enabled);
	}

	static function requireBlock(fn:HxcIRFunction, blockId:String):HxcIRBlock {
		for (block in fn.blocks)
			if (block.id == blockId)
				return block;
		return fail('function `${fn.id}` cannot resolve HxcIR block `$blockId`');
	}

	static function referencedValueIds(fn:HxcIRFunction):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		if (fn.managedRoots != null)
			for (root in fn.managedRoots)
				referenced.set(root.valueId, true);
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				switch instruction.kind {
					case IRIOStore(place, valueId) | IRIOInitialize(place, valueId, _, _):
						referenced.set(valueId, true);
						markPlaceValues(place, referenced);
					case IRIOConvert(valueId, _, _, _, _):
						referenced.set(valueId, true);
					case IRIOLoad(place) | IRIOAddress(place) | IRIOBorrowClassField(place):
						markPlaceValues(place, referenced);
					case IRIOConstructAggregate(_, fields):
						for (field in fields) {
							referenced.set(field.valueId, true);
						}
					case IRIOConstructInterface(_, objectValueId, _):
						referenced.set(objectValueId, true);
					case IRIOProject(valueId, _):
						referenced.set(valueId, true);
					case IRIOConstructTag(_, _, payload):
						for (valueId in payload)
							referenced.set(valueId, true);
					case IRIOMatchTag(valueId, _) | IRIOProjectTag(valueId, _, _, _):
						referenced.set(valueId, true);
					case IRIOInitializeFixedArray(_, valueIds, _, _):
						for (valueId in valueIds) {
							referenced.set(valueId, true);
						}
					case IRIOInitializeSpan(place, sourceArray, _, _):
						markPlaceValues(place, referenced);
						markPlaceValues(sourceArray, referenced);
					case IRIOBoundsCheck(collection, indexValueId, _):
						referenced.set(indexValueId, true);
						markPlaceValues(collection, referenced);
					case IRIONullCheck(valueId, _):
						referenced.set(valueId, true);
					case IRIOUnary(_, valueId, _):
						referenced.set(valueId, true);
					case IRIOBinary(_, leftValueId, rightValueId, _):
						referenced.set(leftValueId, true);
						referenced.set(rightValueId, true);
					case IRIOCall(call):
						for (argument in call.arguments) {
							referenced.set(argument, true);
						}
						switch call.dispatch {
							case IRCDVirtual(_, receiverValueId) | IRCDInterface(_, _, receiverValueId): referenced.set(receiverValueId, true);
							case IRCDClosure(callableValueId): referenced.set(callableValueId, true);
							case _:
						}
					case _:
				}
			}
			if (block.terminator != null) {
				switch block.terminator.kind {
					case IRTReturn(valueId, _) if (valueId != null):
						referenced.set(valueId, true);
					case IRTBranch(conditionValueId, _, _):
						referenced.set(conditionValueId, true);
					case IRTSwitch(valueId, _, _):
						referenced.set(valueId, true);
					case IRTTagSwitch(valueId, _, _):
						referenced.set(valueId, true);
					case IRTThrow(valueId, _):
						referenced.set(valueId, true);
					case _:
				}
			}
		}
		return referenced;
	}

	static function markPlaceValues(place:HxcIRPlace, referenced:Map<String, Bool>):Void {
		switch place {
			case IRPDereference(pointerValueId):
				referenced.set(pointerValueId, true);
			case IRPField(base, _):
				markPlaceValues(base, referenced);
			case IRPIndex(base, indexValueId):
				markPlaceValues(base, referenced);
				referenced.set(indexValueId, true);
			case IRPLocal(_) | IRPGlobal(_):
		}
	}

	static function referencedLocalIds(fn:HxcIRFunction):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				switch instruction.kind {
					case IRIOLoad(place) | IRIOStore(place, _) | IRIOAddress(place) | IRIOBorrowClassField(place) | IRIOBoundsCheck(place, _, _) |
						IRIODeclareUninitialized(place) | IRIODeclareManagedCarrier(place, _) | IRIOAcquireManagedCarrier(place, _, _) |
						IRIOMoveManagedCarrier(place) | IRIODefaultInitialize(place, _, _) | IRIOBindVirtualTable(place, _) | IRIOLifetime(place, _, _, _):
						markReferencedLocals(place, referenced);
					case IRIOInitializeSpan(place, sourceArray, _, _):
						markReferencedLocals(place, referenced);
						markReferencedLocals(sourceArray, referenced);
					case _:
				}
			}
		}
		for (region in fn.cleanupRegions) {
			for (action in region.actions) {
				switch action.kind {
					case IRCADestroy(place, _, _) | IRCARelease(place, _) | IRCADeallocate(place, _):
						markReferencedLocals(place, referenced);
					case IRCAFinally(_):
				}
			}
		}
		return referenced;
	}

	/** Local storage named by a typed cleanup action, independent of ordinary uses. */
	static function cleanupReferencedLocalIds(fn:HxcIRFunction):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (region in fn.cleanupRegions)
			for (action in region.actions)
				switch action.kind {
					case IRCADestroy(place, _, _) | IRCARelease(place, _) | IRCADeallocate(place, _):
						markReferencedLocals(place, referenced);
					case IRCAFinally(_):
				}
		return referenced;
	}

	static function markReferencedLocals(place:HxcIRPlace, referenced:Map<String, Bool>):Void {
		switch place {
			case IRPLocal(localId):
				referenced.set(localId, true);
			case IRPField(base, _) | IRPIndex(base, _):
				markReferencedLocals(base, referenced);
			case IRPGlobal(_) | IRPDereference(_):
		}
	}

	static function referencedSpanLengthIds(fn:HxcIRFunction):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				switch instruction.kind {
					case IRIOLoad(IRPLocal(localId)):
						switch requireLocal(fn, localId).type {
							case IRTSpan(_, _): referenced.set(localId, true);
							case _:
						}
					case IRIOBoundsCheck(IRPLocal(localId), _, IRBPCheckedAbort(_, _)):
						switch requireLocal(fn, localId).type {
							case IRTSpan(_, _): referenced.set(localId, true);
							case _:
						}
					case _:
				}
			}
		}
		return referenced;
	}

	function emitLoad(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			sourceExpression:CExpr, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String, coalesce:Bool):Void {
		final result = requireResult(instruction, functionId);
		if (coalesce) {
			values.set(result.id, sourceExpression);
			if (!referencedValues.exists(result.id)) {
				addLineDirective(statements, instruction.source, lineDirectives);
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), sourceExpression)));
			}
			return;
		}
		final temporaryName = temporaryNames.get(result.id);
		addLineDirective(statements, instruction.source, lineDirectives);
		if (temporaryName == null) {
			if (referencedValues.exists(result.id)) {
				fail('referenced load result `${result.id}` in `$functionId` has no finalized stable-value temporary');
			}
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), sourceExpression)));
			return;
		}
		final declaration = typedDeclarator(result.type, DName(temporaryName));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(sourceExpression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
		if (!referencedValues.exists(result.id)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporaryName))));
		}
	}

	/**
		Allocate one compiler-proven concrete value through hxrt's checked allocator.

		The finalized result temporary is also the stable root for the private
		allocator local name. Appending a reserved compiler suffix cannot collide
		with another emitted temporary because the symbol registry has already made
		the base temporary unique in this function.
	**/
	function emitOwnedAllocation(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			type:HxcIRTypeRef, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final result = requireResult(instruction, fn.id);
		if (typeKey(result.type) != typeKey(IRTPointer(type, false)))
			return fail('owned allocation `${instruction.id}` in `${fn.id}` must return a non-null pointer to its allocated type');
		final temporary = temporaryNames.get(result.id);
		if (temporary == null)
			return fail('owned allocation `${instruction.id}` in `${fn.id}` has no finalized result temporary');
		final allocator = new CIdentifier(temporary.value + "_allocator");
		final allocated = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: allocated.type,
			declarator: allocated.declarator,
			initializer: IExpr(ENull),
			attributes: []
		}));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNAllocatorType))),
			declarator: DName(allocator),
			initializer: IExpr(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), [])),
			attributes: []
		}));
		final allocatedType = typedDeclarator(type, DName(null));
		addLineDirective(statements, instruction.source, lineDirectives);
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNAllocate)), [
			EUnary(AddressOf, EIdentifier(allocator)),
			ESizeOfType(allocatedType.type, allocatedType.declarator),
			EAlignOfType(allocatedType.type, allocatedType.declarator),
			ECast(new CType(TVoid), DPointer(DPointer(DName(null), []), []), EUnary(AddressOf, EIdentifier(temporary)))
		]), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			return fail('owned allocation `${instruction.id}` in `${fn.id}` produced an unreferenced owner');
	}

	/** Allocate one zeroed, stable payload using its finalized exact descriptor. */
	function emitCollectorAllocation(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			type:HxcIRTypeRef, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _: return fail('collector allocation `${instruction.id}` in `${fn.id}` lost its managed instance payload');
		};
		final result = requireResult(instruction, fn.id);
		if (typeKey(result.type) != typeKey(IRTPointer(type, false)))
			return fail('collector allocation `${instruction.id}` in `${fn.id}` must return its exact non-null payload pointer');
		final temporary = temporaryNames.get(result.id);
		if (temporary == null)
			return fail('collector allocation `${instruction.id}` in `${fn.id}` has no finalized result temporary');
		final descriptor = managedDescriptorNames.get(instanceId);
		if (descriptor == null)
			return fail('collector allocation `${instruction.id}` in `${fn.id}` has no descriptor for `$instanceId`');
		final program = managedProgram;
		if (program == null)
			return fail('collector allocation `${instruction.id}` in `${fn.id}` has no executable collector context');
		final declaration = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(ENull),
			attributes: []
		}));
		addLineDirective(statements, instruction.source, lineDirectives);
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNGcAllocate)), [
			EUnary(AddressOf, EIdentifier(program.collector)),
			EUnary(AddressOf, EIdentifier(descriptor)),
			ECast(new CType(TVoid), DPointer(DPointer(DName(null), []), []), EUnary(AddressOf, EIdentifier(temporary)))
		]), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			return fail('collector allocation `${instruction.id}` in `${fn.id}` produced an unreferenced managed value');
	}

	function emitSpanLoad(statements:Array<CStmt>, values:Map<String, CExpr>, spanValueLengths:Map<String, CExpr>, referencedValues:Map<String, Bool>,
			instruction:HxcIRInstruction, place:HxcIRPlace, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		final result = requireResult(instruction, fn.id);
		final pointer = placeExpression(place, fn, localNames, globalNames, spanLengthNames, values);
		final length = collectionLengthExpression(place, fn, localNames, globalNames, spanLengthNames);
		values.set(result.id, pointer);
		spanValueLengths.set(result.id, length);
		if (!referencedValues.exists(result.id)) {
			addLineDirective(statements, instruction.source, lineDirectives);
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), pointer)));
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), length)));
		}
	}

	function emitAddress(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			place:HxcIRPlace, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, shouldInline:Bool):Void {
		final result = requireResult(instruction, fn.id);
		final pointee = switch result.type {
			case IRTPointer(type, false): type;
			case _: return fail('address `${instruction.id}` in `${fn.id}` lost its validated non-null pointer result');
		};
		final expression = EUnary(AddressOf, placeExpression(place, fn, localNames, globalNames, spanLengthNames, values));
		if (shouldInline) {
			values.set(result.id, expression);
			return;
		}
		final temporaryName = temporaryNames.get(result.id);
		addLineDirective(statements, instruction.source, lineDirectives);
		if (temporaryName == null) {
			if (referencedValues.exists(result.id)) {
				fail('referenced address result `${result.id}` in `${fn.id}` has no finalized C temporary');
			}
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), expression)));
			return;
		}
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(pointee),
			declarator: DPointer(DName(temporaryName), []),
			initializer: IExpr(expression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
		if (!referencedValues.exists(result.id)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporaryName))));
		}
	}

	function emitAggregateConstruction(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			instanceId:String, fields:Array<HxcIRNamedValue>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String,
			coalesce:Bool):Void {
		final resolvedOrder = aggregateFieldOrder.get(instanceId);
		final expectedOrder:Array<String> = resolvedOrder == null ? fail('aggregate construction `${instruction.id}` in `$functionId` has no finalized direct-record layout') : resolvedOrder;
		if (expectedOrder.length != fields.length) {
			fail('aggregate construction `${instruction.id}` in `$functionId` has no finalized direct-record layout');
		}
		final initializers:Array<CInitializerItem> = [];
		for (index in 0...fields.length) {
			final field = fields[index];
			if (field.name != expectedOrder[index]) {
				fail('aggregate construction `${instruction.id}` in `$functionId` lost canonical field order');
			}
			initializers.push({
				designators: [
					DField(requireDirectFieldName(instanceId, field.name, instruction.id, functionId))
				],
				value: IExpr(requireValue(values, field.valueId, functionId))
			});
		}
		final result = requireResult(instruction, functionId);
		final expression = ECompoundLiteral(cType(result.type), DName(null), IList(initializers));
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, functionId, coalesce);
	}

	function emitInterfaceConstruction(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			interfaceInstanceId:String, objectValueId:String, tableId:String, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool,
			functionId:String):Void {
		final layout = requireInterfaceLayout(interfaceInstanceId);
		final table = requireVirtualTable(tableId);
		if (table.layout.id != layout.id)
			fail('interface construction `${instruction.id}` in `$functionId` selected a table for another interface');
		final result = requireResult(instruction, functionId);
		final expression = ECompoundLiteral(cType(result.type), DName(null), IList([
			{
				designators: [DField(requireInterfaceObjectMember(layout))],
				value: IExpr(requireValue(values, objectValueId, functionId))
			},
			{
				designators: [DField(requireInterfaceTableMember(layout))],
				value: IExpr(EUnary(AddressOf, EIdentifier(table.cName)))
			}
		]));
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, functionId, false);
	}

	function emitAggregateProjection(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			valueId:String, fieldName:String, fn:HxcIRFunction, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, coalesce:Bool):Void {
		final instanceId = switch valueType(fn, valueId) {
			case IRTInstance(id): id;
			case _: return fail('aggregate projection `${instruction.id}` in `${fn.id}` lost its validated instance value');
		};
		final expression = EMember(requireValue(values, valueId, fn.id), requireDirectFieldName(instanceId, fieldName, instruction.id, fn.id), false);
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id, coalesce);
	}

	function emitEnumConstruction(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			instanceId:String, tagName:String, payload:Array<String>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String,
			coalesce:Bool):Void {
		final discriminant = EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName));
		final representation = requireEnumRepresentation(instanceId);
		final expression:CExpr = switch representation {
			case CBECNative:
				if (payload.length != 0)
					fail('native enum construction `${instruction.id}` in `$functionId` unexpectedly carries payload');
				discriminant;
			case CBECTagged:
				final initializers:Array<CInitializerItem> = [
					{
						designators: [DField(requireEnumTagMember(instanceId))],
						value: IExpr(discriminant)
					}
				];
				final payloadNames = requireEnumPayloadNames(instanceId, tagName);
				if (payloadNames.length != payload.length) {
					fail('tagged enum construction `${instruction.id}` in `$functionId` lost its validated payload layout');
				}
				for (index in 0...payload.length) {
					initializers.push({
						designators: [
							DField(requireEnumPayloadMember(instanceId)),
							DField(requireEnumCaseUnionMember(instanceId, tagName)),
							DField(requireEnumPayloadFieldName(instanceId, tagName, payloadNames[index]))
						],
						value: IExpr(requireValue(values, payload[index], functionId))
					});
				}
				ECompoundLiteral(cType(IRTInstance(instanceId)), DName(null), IList(initializers));
		};
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, functionId, coalesce);
	}

	function emitEnumMatch(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			valueId:String, tagName:String, fn:HxcIRFunction, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, coalesce:Bool):Void {
		final instanceId = requireEnumInstanceId(valueType(fn, valueId), instruction.id, fn.id);
		final expression = EBinary(Equal, enumTagExpression(requireValue(values, valueId, fn.id), instanceId),
			EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName)));
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id, coalesce);
	}

	function emitEnumProjection(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			valueId:String, tagName:String, payloadIndex:Int, fn:HxcIRFunction, temporaryNames:Map<String, CIdentifier>, boundsAbortName:Null<CIdentifier>,
			lineDirectives:Bool):Void {
		final instanceId = requireEnumInstanceId(valueType(fn, valueId), instruction.id, fn.id);
		if (requireEnumRepresentation(instanceId) != CBECTagged) {
			fail('payload projection `${instruction.id}` in `${fn.id}` requires a tagged enum');
		}
		final value = requireValue(values, valueId, fn.id);
		final discriminant = EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SIf(EBinary(NotEqual, enumTagExpression(value, instanceId), discriminant),
			SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, instruction.id, fn.id)), [])), null));
		final payloadNames = requireEnumPayloadNames(instanceId, tagName);
		if (payloadIndex < 0 || payloadIndex >= payloadNames.length) {
			fail('payload projection `${instruction.id}` in `${fn.id}` has invalid field index `$payloadIndex`');
		}
		final expression = EMember(EMember(EMember(value, requireEnumPayloadMember(instanceId), false), requireEnumCaseUnionMember(instanceId, tagName), false),
			requireEnumPayloadFieldName(instanceId, tagName, payloadNames[payloadIndex]), false);
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id, false);
	}

	function emitDefaultInitialize(statements:Array<CStmt>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>, instruction:HxcIRInstruction,
			localId:String, from:HxcIRInitializationState, to:HxcIRInitializationState, fn:HxcIRFunction, localNames:Map<String, CIdentifier>,
			lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId) || from != IRISUninitialized || to != IRISInitializing && to != IRISInitialized) {
			fail('default initializer `${instruction.id}` in `${fn.id}` has invalid declaration or lifetime state');
		}
		final local = requireLocal(fn, localId);
		switch local.type {
			case IRTInstance(instanceId) if (classTags.exists(instanceId) || aggregateTags.exists(instanceId)):
			case _:
				return fail('default initializer `${instruction.id}` in `${fn.id}` does not target direct record or concrete-class storage');
		}
		final declaration = typedDeclarator(local.type, DName(requireLocalName(localNames, localId, fn.id)));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IList([{designators: [], value: IExpr(EInt(CIntegerLiteral.decimal("0")))}]),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(requireLocalName(localNames, localId, fn.id)))));
		}
	}

	function emitBindVirtualTable(statements:Array<CStmt>, instruction:HxcIRInstruction, place:HxcIRPlace, tableId:String, fn:HxcIRFunction,
			localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>, values:Map<String, CExpr>,
			lineDirectives:Bool):Void {
		if (instruction.result != null)
			fail('virtual-table bind `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		final table = requireVirtualTable(tableId);
		final instanceId = switch placeType(place, fn) {
			case IRTInstance(value): value;
			case _: return fail('virtual-table bind `${instruction.id}` in `${fn.id}` does not target concrete object storage');
		};
		if (instanceId != table.classInstanceId)
			fail('virtual-table bind `${instruction.id}` in `${fn.id}` selected table `$tableId` for the wrong concrete class');
		final path = classBasePath(instanceId, table.layout.rootInstanceId, instruction.id, fn.id, true);
		var rootObject:CExpr = placeExpression(place, fn, localNames, globalNames, spanLengthNames, values);
		for (member in path)
			rootObject = EMember(rootObject, member, false);
		final header = EMember(rootObject, requireClassDispatchHeader(table.layout.rootInstanceId), false);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SExpr(EBinary(Assign, header, EUnary(AddressOf, EIdentifier(table.cName)))));
	}

	function enumTagExpression(value:CExpr, instanceId:String):CExpr {
		return switch requireEnumRepresentation(instanceId) {
			case CBECNative: value;
			case CBECTagged: EMember(value, requireEnumTagMember(instanceId), false);
		};
	}

	function emitInitialize(statements:Array<CStmt>, values:Map<String, CExpr>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
			hoistedLocals:Map<String, Bool>, instruction:HxcIRInstruction, localId:String, valueId:String, fn:HxcIRFunction,
			localNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null) {
			fail('initializer `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		}
		if (declared.exists(localId)) {
			if (!hoistedLocals.exists(localId))
				fail('local `$localId` in `${fn.id}` is initialized more than once');
			addLineDirective(statements, instruction.source, lineDirectives);
			statements.push(SExpr(EBinary(Assign, EIdentifier(requireLocalName(localNames, localId, fn.id)), requireValue(values, valueId, fn.id))));
			return;
		}
		final local = requireLocal(fn, localId);
		final declaration = typedDeclarator(local.type, DName(requireLocalName(localNames, localId, fn.id)));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(requireValue(values, valueId, fn.id)),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(requireLocalName(localNames, localId, fn.id)))));
		}
	}

	/**
	 * Emit storage for a proven direct-value join without creating a fake value.
	 *
	 * HxcIR validation establishes that structured control flow assigns the
	 * carrier before it is read. The C declaration therefore intentionally has
	 * no initializer; each conditional arm emits one ordinary assignment.
	 */
	function emitUninitializedDeclaration(statements:Array<CStmt>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
			instruction:HxcIRInstruction, localId:String, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId)) {
			fail('uninitialized declaration `${instruction.id}` in `${fn.id}` has invalid declaration state');
		}
		final local = requireLocal(fn, localId);
		final declaration = typedDeclarator(local.type, DName(requireLocalName(localNames, localId, fn.id)));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: null,
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(requireLocalName(localNames, localId, fn.id)))));
		}
	}

	/**
	 * Assign one selected enum value and acquire exactly one owner for the join.
	 *
	 * Moving a fresh value needs only the structural C assignment. Copying a
	 * borrowed value immediately calls the enum's active-tag retain helper, so
	 * later branch cleanup cannot invalidate the joined payload.
	 */
	function emitManagedCarrierAcquisition(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, place:HxcIRPlace, valueId:String,
			acquisition:HxcIRManagedCarrierAcquisition, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>, boundsAbortName:Null<CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null)
			fail('managed carrier acquisition `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		final target = placeExpression(place, fn, localNames, globalNames, spanLengthNames, values);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SExpr(EBinary(Assign, target, requireValue(values, valueId, fn.id))));
		switch acquisition {
			case IRMCAMoveFresh:
			case IRMCARetainBorrowed(IRIProgramLocal(implementationId)):
				final lifecycle = programLocalLifecycle(implementationId, instruction.id, fn.id);
				emitStatusAbort(statements, ECall(EIdentifier(lifecycle.retainName), [EUnary(AddressOf, target)]), boundsAbortName, instruction.id, fn.id);
			case IRMCARetainBorrowed(_):
				fail('managed carrier acquisition `${instruction.id}` in `${fn.id}` has no program-local retain plan');
		}
	}

	function emitFixedArrayInitialize(statements:Array<CStmt>, values:Map<String, CExpr>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
			instruction:HxcIRInstruction, localId:String, valueIds:Array<String>, fn:HxcIRFunction, localNames:Map<String, CIdentifier>,
			lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId)) {
			fail('fixed-array initializer `${instruction.id}` in `${fn.id}` has invalid declaration state');
		}
		final local = requireLocal(fn, localId);
		final fixed = switch local.type {
			case IRTFixedArray(element, length, _): {element: element, length: length};
			case _: return fail('fixed-array initializer `${instruction.id}` in `${fn.id}` targets a non-array local');
		};
		if (valueIds.length != fixed.length) {
			fail('fixed-array initializer `${instruction.id}` in `${fn.id}` lost its validated element count');
		}
		final name = requireLocalName(localNames, localId, fn.id);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(fixed.element),
			declarator: DArray(DName(name), ABFixed(EInt(CIntegerLiteral.decimal(Std.string(fixed.length)))), []),
			initializer: IList(valueIds.map(valueId -> {
				designators: [],
				value: IExpr(requireValue(values, valueId, fn.id))
			})),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
		}
	}

	function emitZeroFixedArrayInitialize(statements:Array<CStmt>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
			instruction:HxcIRInstruction, localId:String, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId)) {
			fail('zero fixed-array initializer `${instruction.id}` in `${fn.id}` has invalid declaration state');
		}
		final local = requireLocal(fn, localId);
		final fixed = switch local.type {
			case IRTFixedArray(element, length, _): {element: element, length: length};
			case _: return fail('zero fixed-array initializer `${instruction.id}` in `${fn.id}` targets a non-array local');
		};
		switch HxcIRFixedArrayPolicy.zeroStorage(fixed.element, fixed.length) {
			case IRFASAutomatic(_, _):
			case _:
				return fail('zero fixed-array initializer `${instruction.id}` in `${fn.id}` violates its validated automatic-storage policy');
		}
		final name = requireLocalName(localNames, localId, fn.id);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(fixed.element),
			declarator: DArray(DName(name), ABFixed(EInt(CIntegerLiteral.decimal(Std.string(fixed.length)))), []),
			initializer: IList([
				{
					designators: [],
					value: IExpr(EInt(CIntegerLiteral.decimal("0")))
				}
			]),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
		}
	}

	function emitSpanInitialize(statements:Array<CStmt>, values:Map<String, CExpr>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
			referencedSpanLengths:Map<String, Bool>, instruction:HxcIRInstruction, localId:String, sourceArray:HxcIRPlace, fn:HxcIRFunction,
			localNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId)) {
			fail('span initializer `${instruction.id}` in `${fn.id}` has invalid declaration state');
		}
		final local = requireLocal(fn, localId);
		final span = switch local.type {
			case IRTSpan(element, mutable): {element: element, mutable: mutable};
			case _: return fail('span initializer `${instruction.id}` in `${fn.id}` targets a non-span local');
		};
		final name = requireLocalName(localNames, localId, fn.id);
		final lengthName = requireSpanLengthName(spanLengthNames, localId, fn.id);
		final sourceExpression = placeExpression(sourceArray, fn, localNames, globalNames, spanLengthNames, values);
		final sourceLength = collectionLengthExpression(sourceArray, fn, localNames, globalNames, spanLengthNames);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: span.mutable ? cType(span.element) : constType(span.element),
			declarator: DPointer(DName(name), []),
			initializer: IExpr(sourceExpression),
			attributes: []
		}));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: new CType(TSizeT),
			declarator: DName(lengthName),
			initializer: IExpr(sourceLength),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
		}
		if (!referencedSpanLengths.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(lengthName))));
		}
	}

	function emitSpanValueInitialize(statements:Array<CStmt>, values:Map<String, CExpr>, spanValueLengths:Map<String, CExpr>, declared:Map<String, Bool>,
			referencedLocals:Map<String, Bool>, referencedSpanLengths:Map<String, Bool>, instruction:HxcIRInstruction, localId:String, valueId:String,
			fn:HxcIRFunction, localNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId)) {
			fail('span value initializer `${instruction.id}` in `${fn.id}` has invalid declaration state');
		}
		final local = requireLocal(fn, localId);
		final span = switch local.type {
			case IRTSpan(element, mutable): {element: element, mutable: mutable};
			case _: return fail('span value initializer `${instruction.id}` in `${fn.id}` targets a non-span local');
		};
		final name = requireLocalName(localNames, localId, fn.id);
		final lengthName = requireSpanLengthName(spanLengthNames, localId, fn.id);
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: span.mutable ? cType(span.element) : constType(span.element),
			declarator: DPointer(DName(name), []),
			initializer: IExpr(requireValue(values, valueId, fn.id)),
			attributes: []
		}));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: new CType(TSizeT),
			declarator: DName(lengthName),
			initializer: IExpr(requireSpanValueLength(spanValueLengths, valueId, fn.id)),
			attributes: []
		}));
		declared.set(localId, true);
		if (!referencedLocals.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
		}
		if (!referencedSpanLengths.exists(localId)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(lengthName))));
		}
	}

	function emitBoundsCheck(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, collection:HxcIRPlace, indexValueId:String,
			fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>,
			boundsAbortName:Null<CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null) {
			fail('bounds check `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		}
		final abortName:CIdentifier = requireBoundsAbortName(boundsAbortName, instruction.id, fn.id);
		final index = requireValue(values, indexValueId, fn.id);
		final negative = EBinary(Less, index, EInt(CIntegerLiteral.decimal("0")));
		final outOfRange = EBinary(GreaterEqual, ECast(new CType(TSizeT), DName(null), index),
			collectionLengthExpression(collection, fn, localNames, globalNames, spanLengthNames));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SIf(EBinary(LogicalOr, negative, outOfRange), SExpr(ECall(EIdentifier(abortName), [])), null));
	}

	function emitNullCheck(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, valueId:String,
			boundsAbortName:Null<CIdentifier>, lineDirectives:Bool, fn:HxcIRFunction):Void {
		final functionId = fn.id;
		if (instruction.result != null)
			fail('null check `${instruction.id}` in `$functionId` unexpectedly defines a value');
		final absent = switch valueType(fn, valueId) {
			case IRTNullable(_, IRNTagged):
				final optional = requireOptional(valueType(fn, valueId));
				EUnary(LogicalNot, EMember(requireValue(values, valueId, functionId), optional.presenceName, false));
			case IRTString | IRTManagedString:
				EBinary(Equal, EMember(requireValue(values, valueId, functionId), new CIdentifier("data"), false), ENull);
			case _: EBinary(Equal, requireValue(values, valueId, functionId), ENull);
		};
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SIf(absent, SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, instruction.id, functionId)), [])), null));
	}

	function classUpcastExpression(value:CExpr, sourceType:Null<HxcIRTypeRef>, targetType:HxcIRTypeRef, instructionId:String, functionId:String):CExpr {
		final source = switch sourceType {
			case IRTPointer(IRTInstance(instanceId), nullable): {instanceId: instanceId, nullable: nullable};
			case _: return fail('class upcast `$instructionId` in `$functionId` lost its source instance pointer');
		};
		final target = switch targetType {
			case IRTPointer(IRTInstance(instanceId), nullable): {instanceId: instanceId, nullable: nullable};
			case _: return fail('class upcast `$instructionId` in `$functionId` lost its target instance pointer');
		};
		if (source.nullable != target.nullable)
			return fail('class upcast `$instructionId` in `$functionId` changed pointer nullability');
		final path = classBasePath(source.instanceId, target.instanceId, instructionId, functionId, false);
		var projected:CExpr = value;
		for (index in 0...path.length)
			projected = EMember(projected, path[index], index == 0);
		final address = EUnary(AddressOf, projected);
		return source.nullable ? EConditional(EBinary(Equal, value, ENull), ENull, address) : address;
	}

	function emitTerminator(statements:Array<CStmt>, values:Map<String, CExpr>, terminator:HxcIRTerminator, labelNames:Map<String, CIdentifier>,
			fn:HxcIRFunction, boundsAbortName:Null<CIdentifier>, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>):Void {
		final functionId = fn.id;
		switch terminator.kind {
			case IRTReturn(valueId, cleanup):
				emitCleanupSteps(statements, cleanup, fn, values, localNames, globalNames, spanLengthNames, boundsAbortName);
				emitManagedRootFramePop(statements, fn, boundsAbortName);
				switch fn.failureConvention {
					case IRFCInfallible:
						statements.push(SReturn(valueId == null ? null : requireValue(values, valueId, functionId)));
					case IRFCStatus(_):
						if (valueId != null || fn.returnType != IRTVoid)
							fail('status-returning function `$functionId` lost its Void semantic result');
						statements.push(SReturn(EBool(true)));
				}
			case IRTJump(edge):
				requirePlainEdge(edge, functionId);
				statements.push(SGoto(requireLabelName(labelNames, edge.targetBlockId, functionId)));
			case IRTBranch(conditionValueId, whenTrue, whenFalse):
				requirePlainEdge(whenTrue, functionId);
				requirePlainEdge(whenFalse, functionId);
				statements.push(SIf(requireValue(values, conditionValueId, functionId),
					SGoto(requireLabelName(labelNames, whenTrue.targetBlockId, functionId)),
					SGoto(requireLabelName(labelNames, whenFalse.targetBlockId, functionId))));
			case IRTSwitch(valueId, cases, defaultEdge):
				final emittedCases:Array<CCase> = [];
				for (item in cases) {
					requirePlainEdge(item.edge, functionId);
					emittedCases.push({
						values: [constantExpression(item.value)],
						isDefault: false,
						body: [SGoto(requireLabelName(labelNames, item.edge.targetBlockId, functionId))]
					});
				}
				requirePlainEdge(defaultEdge, functionId);
				emittedCases.push({
					values: [],
					isDefault: true,
					body: [SGoto(requireLabelName(labelNames, defaultEdge.targetBlockId, functionId))]
				});
				statements.push(SSwitch(requireValue(values, valueId, functionId), emittedCases));
			case IRTTagSwitch(valueId, cases, defaultEdge):
				final instanceId = requireEnumInstanceId(valueType(fn, valueId), "terminator", functionId);
				final emittedCases:Array<CCase> = [];
				for (item in cases) {
					requirePlainEdge(item.edge, functionId);
					emittedCases.push({
						values: [EIdentifier(requireEnumCaseDiscriminant(instanceId, item.tagName))],
						isDefault: false,
						body: [SGoto(requireLabelName(labelNames, item.edge.targetBlockId, functionId))]
					});
				}
				if (defaultEdge != null) {
					requirePlainEdge(defaultEdge, functionId);
					emittedCases.push({
						values: [],
						isDefault: true,
						body: [SGoto(requireLabelName(labelNames, defaultEdge.targetBlockId, functionId))]
					});
				}
				statements.push(SSwitch(enumTagExpression(requireValue(values, valueId, functionId), instanceId), emittedCases));
			case IRTThrow(valueId, failure):
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), requireValue(values, valueId, functionId))));
				emitCleanupSteps(statements, failure.cleanup, fn, values, localNames, globalNames, spanLengthNames, boundsAbortName);
				emitManagedRootFramePop(statements, fn, boundsAbortName);
				emitFailureTarget(statements, failure, fn, boundsAbortName, "throw");
			case IRTUnreachable:
				statements.push(SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, "unreachable", functionId)), [])));
		}
	}

	function validateConstructionCleanupRegions(fn:HxcIRFunction):Void {
		for (region in fn.cleanupRegions) {
			if (region.id != "cleanup.construction" || region.parentId != null)
				fail('function `${fn.id}` has a cleanup region outside direct stack construction');
			for (action in region.actions) {
				if (action.idempotence != IRCExactlyOnce)
					fail('construction cleanup `${action.id}` in `${fn.id}` must execute exactly once');
				switch action.kind {
					case IRCADestroy(IRPLocal(localId), from, IRISDestroyed):
						if (from != IRISInitializing && from != IRISInitialized)
							fail('construction cleanup `${action.id}` in `${fn.id}` has an invalid source state');
						switch requireLocal(fn, localId).type {
							case IRTInstance(instanceId) if (classTags.exists(instanceId)):
							case _:
								fail('construction cleanup `${action.id}` in `${fn.id}` does not own direct class storage');
						}
					case IRCARelease(place, IRIRuntime("array")):
						switch placeType(place, fn) {
							case IRTInstance(instanceId) if (arrayElementTypes.exists(instanceId)):
							case _:
								fail('Array cleanup `${action.id}` in `${fn.id}` does not own a managed Array place');
						}
					case IRCARelease(place, IRIRuntime("string-map")):
						switch placeType(place, fn) {
							case IRTInstance(instanceId) if (stringMapValueTypes.exists(instanceId)):
							case _:
								fail('StringMap cleanup `${action.id}` in `${fn.id}` does not own a managed StringMap place');
						}
					case IRCARelease(place, IRIRuntime("int-map")):
						switch placeType(place, fn) {
							case IRTInstance(instanceId) if (intMapInstanceIds.exists(instanceId)):
							case _:
								fail('IntMap cleanup `${action.id}` in `${fn.id}` does not own a managed IntMap place');
						}
					case IRCARelease(place, IRIRuntime("bytes")):
						switch placeType(place, fn) {
							case IRTInstance(instanceId) if (bytesInstanceIds.exists(instanceId)):
							case _:
								fail('Bytes cleanup `${action.id}` in `${fn.id}` does not own a managed Bytes place');
						}
					case IRCARelease(place, IRIRuntime("string")):
						if (placeType(place, fn) != IRTManagedString)
							fail('String cleanup `${action.id}` in `${fn.id}` does not own a managed String place');
					case IRCARelease(place, IRIProgramLocal(implementationId)):
						final cleanup = arrayElementCleanups.get(implementationId);
						final enumLifecycle = enumArrayLifecycles.get(implementationId);
						final aggregateLifecycle = aggregateLifecycles.get(implementationId);
						final optionalLifecycle = optionalLifecycles.get(implementationId);
						final actualType = placeType(place, fn);
						if (cleanup != null) {
							if (actualType == null || typeKey(actualType) != typeKey(cleanup.elementType))
								fail('managed Array element cleanup `${action.id}` in `${fn.id}` does not own the planned element type');
						} else if (enumLifecycle != null) {
							if (actualType == null || typeKey(actualType) != typeKey(IRTInstance(enumLifecycle.instanceId)))
								fail('managed enum cleanup `${action.id}` in `${fn.id}` does not own the planned enum type');
						} else if (aggregateLifecycle != null) {
							if (actualType == null || typeKey(actualType) != typeKey(IRTInstance(aggregateLifecycle.instanceId)))
								fail('managed aggregate cleanup `${action.id}` in `${fn.id}` does not own the planned record type');
						} else if (optionalLifecycle != null) {
							if (actualType == null || typeKey(actualType) != typeKey(optionalLifecycle.type))
								fail('managed optional cleanup `${action.id}` in `${fn.id}` does not own the planned optional type');
						} else {
							fail('program-local cleanup `${action.id}` in `${fn.id}` names unknown plan `$implementationId`');
						}
					case _:
						fail('construction cleanup `${action.id}` in `${fn.id}` is outside the direct stack-object subset');
				}
			}
		}
	}

	function emitCleanupSteps(statements:Array<CStmt>, steps:Array<HxcIRCleanupStep>, fn:HxcIRFunction, values:Map<String, CExpr>,
			localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>,
			boundsAbortName:Null<CIdentifier>):Void {
		for (step in steps) {
			final action = requireCleanupAction(fn, step);
			switch action.kind {
				case IRCADestroy(IRPLocal(_), IRISInitializing | IRISInitialized, IRISDestroyed):
					// Direct class storage currently contains only borrowed/direct fields, so
					// destruction is a proven no-op. The ordered HxcIR edge remains authoritative.
				case IRCARelease(place, IRIRuntime("array")):
					emitStatusAbort(statements,
						ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayRelease)),
							[placeExpression(place, fn, localNames, globalNames, spanLengthNames, values)]),
						boundsAbortName, action.id, fn.id);
				case IRCARelease(place, IRIRuntime("string-map")):
					emitStatusAbort(statements,
						ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapRelease)),
							[placeExpression(place, fn, localNames, globalNames, spanLengthNames, values)]),
						boundsAbortName, action.id, fn.id);
				case IRCARelease(place, IRIRuntime("int-map")):
					emitStatusAbort(statements,
						ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapRelease)),
							[placeExpression(place, fn, localNames, globalNames, spanLengthNames, values)]),
						boundsAbortName, action.id, fn.id);
				case IRCARelease(place, IRIRuntime("bytes")):
					emitStatusAbort(statements,
						ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNBytesRelease)),
							[placeExpression(place, fn, localNames, globalNames, spanLengthNames, values)]),
						boundsAbortName, action.id, fn.id);
				case IRCARelease(place, IRIRuntime("string")):
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringRelease)), [
						EUnary(AddressOf, placeExpression(place, fn, localNames, globalNames, spanLengthNames, values))
					]), boundsAbortName, action.id, fn.id);
				case IRCARelease(place, IRIProgramLocal(implementationId)):
					final cleanup = arrayElementCleanups.get(implementationId);
					if (cleanup != null) {
						statements.push(SExpr(ECall(EIdentifier(cleanup.destroyName), [
							ENull,
							EUnary(AddressOf, placeExpression(place, fn, localNames, globalNames, spanLengthNames, values))
						])));
					} else {
						final lifecycle = programLocalLifecycle(implementationId, action.id, fn.id);
						statements.push(SExpr(ECall(EIdentifier(lifecycle.destroyName), [
							EUnary(AddressOf, placeExpression(place, fn, localNames, globalNames, spanLengthNames, values))
						])));
					}
				case _:
					fail('cleanup `${step.regionId}.${step.actionId}` in `${fn.id}` is not directly emittable');
			}
		}
	}

	function emitFailureTarget(statements:Array<CStmt>, failure:HxcIRFailureEdge, fn:HxcIRFunction, boundsAbortName:Null<CIdentifier>, owner:String):Void {
		switch failure.target {
			case IRFTPropagate:
				switch fn.failureConvention {
					case IRFCStatus(kind) if (kind == failure.kind): statements.push(SReturn(EBool(false)));
					case _: fail('$owner in `${fn.id}` cannot propagate without a matching status convention');
				}
			case IRFTAbort:
				statements.push(SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, owner, fn.id)), [])));
			case IRFTBlock(blockId):
				fail('$owner in `${fn.id}` has unsupported failure continuation block `$blockId`');
		}
	}

	static function requireCleanupAction(fn:HxcIRFunction, step:HxcIRCleanupStep):HxcIRCleanupAction {
		for (region in fn.cleanupRegions) {
			if (region.id == step.regionId) {
				for (action in region.actions) {
					if (action.id == step.actionId)
						return action;
				}
			}
		}
		throw new CBodyEmissionError('function `${fn.id}` cannot resolve cleanup `${step.regionId}.${step.actionId}`');
	}

	static function requirePlainEdge(edge:HxcIRBlockEdge, functionId:String):Void {
		if (edge.arguments.length != 0 || edge.cleanup.length != 0) {
			fail('function `$functionId` requires block arguments or cleanup outside the sequenced direct-value subset');
		}
	}

	function placeExpression(place:HxcIRPlace, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>, ?values:Map<String, CExpr>):CExpr {
		return switch place {
			case IRPLocal(localId): EIdentifier(requireLocalName(localNames, localId, fn.id));
			case IRPGlobal(globalId): EIdentifier(requireGlobalName(globalNames, globalId, fn.id));
			case IRPDereference(pointerValueId):
				if (values == null) {
					return fail('function `${fn.id}` attempted to emit unresolved pointer value `$pointerValueId`');
				}
				EUnary(Dereference, requireValue(values, pointerValueId, fn.id));
			case IRPField(base, fieldName):
				final instanceId = switch placeType(base, fn) {
					case IRTInstance(id): id;
					case _: return fail('function `${fn.id}` lost the aggregate/class type of field place `$fieldName`');
				};
				final baseExpression = placeExpression(base, fn, localNames, globalNames, spanLengthNames, values);
				if (aggregateTags.exists(instanceId) || imports.typeByInstance(instanceId) != null) {
					EMember(baseExpression, requireDirectFieldName(instanceId, fieldName, "place", fn.id), false);
				} else {
					classFieldExpression(baseExpression, instanceId, fieldName, "place", fn.id);
				}
			case IRPIndex(base, indexValueId):
				if (values == null) {
					return fail('function `${fn.id}` attempted to emit unresolved collection index `$indexValueId`');
				}
				final indexExpression = ECast(new CType(TSizeT), DName(null), requireValue(values, indexValueId, fn.id));
				EIndex(placeExpression(base, fn, localNames, globalNames, spanLengthNames, values), indexExpression);
		};
	}

	function collectionLengthExpression(place:HxcIRPlace, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			spanLengthNames:Map<String, CIdentifier>):CExpr {
		return switch place {
			case IRPLocal(localId):
				final local = requireLocal(fn, localId);
				switch local.type {
					case IRTFixedArray(_, _, _):
						final array = EIdentifier(requireLocalName(localNames, localId, fn.id));
						EBinary(Divide, EUnary(SizeOfExpr, array), EUnary(SizeOfExpr, EIndex(array, EInt(CIntegerLiteral.decimal("0")))));
					case IRTSpan(_, _): EIdentifier(requireSpanLengthName(spanLengthNames, localId, fn.id));
					case _: fail('function `${fn.id}` requested a length for non-collection local `$localId`');
				}
			case IRPGlobal(globalId):
				fail('function `${fn.id}` does not yet admit fixed-array/span global `$globalId`');
			case _:
				switch placeType(place, fn) {
					case IRTFixedArray(_, length, _): EInt(CIntegerLiteral.decimal(Std.string(length)));
					case _: fail('function `${fn.id}` requested a collection length from a non-array field place');
				}
		};
	}

	function constType(type:HxcIRTypeRef):CType {
		final base = cType(type);
		final qualifiers = base.qualifiers.copy();
		qualifiers.push(QConst);
		return new CType(base.spec, qualifiers);
	}

	function recordPureResult(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			result:HxcIRResult, expression:CExpr, lineDirectives:Bool, functionId:String):Void {
		values.set(result.id, expression);
		if (!referencedValues.exists(result.id)) {
			addLineDirective(statements, instruction.source, lineDirectives);
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), requireValue(values, result.id, functionId))));
		}
	}

	static function operationExpression(operationId:String, implementation:HxcIRImplementation, operands:Array<CExpr>, helperNames:Map<String, CIdentifier>,
			instructionId:String, functionId:String):CExpr {
		return switch implementation {
			case IRIStatic:
				staticOperationExpression(operationId, operands, instructionId, functionId);
			case IRIProgramLocal(helperId):
				helperCall(helperId, operands, helperNames, instructionId, functionId);
			case IRIRuntime(featureId):
				fail('primitive operation `$instructionId` in `$functionId` unexpectedly selected runtime feature `$featureId`');
		};
	}

	static function staticOperationExpression(operationId:String, operands:Array<CExpr>, instructionId:String, functionId:String):CExpr {
		if (operands.length == 1) {
			final value = operands[0];
			return switch operationId {
				case "haxe.u32.negate": widenedUInt32Binary(Subtract, uint32Constant("0"), value);
				case "haxe.u32.bit-not": castUInt32(EUnary(BitwiseNot, castUInt64(castUInt32(value))));
				case "haxe.f64.negate": EUnary(Minus, value);
				case "haxe.bool.not": EUnary(LogicalNot, value);
				case _: fail('unary instruction `$instructionId` in `$functionId` has unsupported direct operation `$operationId`');
			};
		}
		if (operands.length != 2) {
			return fail('primitive instruction `$instructionId` in `$functionId` has invalid operand count `${operands.length}`');
		}
		final left = operands[0];
		final right = operands[1];
		return switch operationId {
			case "hxc.size.add-one.span-index-proven": EBinary(Add, left, right);
			case "hxc.size.less.span-index": EBinary(Less, left, right);
			case "haxe.u32.add": widenedUInt32Binary(Add, left, right);
			case "haxe.u32.subtract": widenedUInt32Binary(Subtract, left, right);
			case "haxe.u32.multiply": widenedUInt32Binary(Multiply, left, right);
			case "haxe.f64.add": EBinary(Add, left, right);
			case "haxe.f64.subtract": EBinary(Subtract, left, right);
			case "haxe.f64.multiply": EBinary(Multiply, left, right);
			case "haxe.u32.shift-left.masked": widenedUInt32Shift(ShiftLeft, left, right);
			case "haxe.u32.shift-right.masked" | "haxe.u32.unsigned-shift-right.masked": widenedUInt32Shift(ShiftRight, left, right);
			case "haxe.u32.bit-and": widenedUInt32Binary(BitAnd, left, right);
			case "haxe.u32.bit-or": widenedUInt32Binary(BitOr, left, right);
			case "haxe.u32.bit-xor": widenedUInt32Binary(BitXor, left, right);
			case "haxe.bool.equal" | "haxe.i32.equal" | "haxe.u32.equal" | "haxe.f64.equal" | "haxe.enum-tag.equal": EBinary(Equal, left, right);
			case "haxe.class-reference.equal" | "haxe.array-reference.equal" | "haxe.string-map-reference.equal": EBinary(Equal, left, right);
			case "haxe.bool.not-equal" | "haxe.i32.not-equal" | "haxe.u32.not-equal" | "haxe.f64.not-equal" | "haxe.enum-tag.not-equal":
				EBinary(NotEqual, left, right);
			case "haxe.class-reference.not-equal" | "haxe.array-reference.not-equal" | "haxe.string-map-reference.not-equal":
				EBinary(NotEqual, left, right);
			case "haxe.string.equal": stringViewEqualExpression(left, right, false, false);
			case "haxe.string.equal.left-non-null": stringViewEqualExpression(left, right, true, false);
			case "haxe.string.equal.right-non-null": stringViewEqualExpression(left, right, false, true);
			case "haxe.string.equal.non-null": stringViewEqualExpression(left, right, true, true);
			case "haxe.string.not-equal": EUnary(LogicalNot, stringViewEqualExpression(left, right, false, false));
			case "haxe.string.not-equal.left-non-null": EUnary(LogicalNot, stringViewEqualExpression(left, right, true, false));
			case "haxe.string.not-equal.right-non-null": EUnary(LogicalNot, stringViewEqualExpression(left, right, false, true));
			case "haxe.string.not-equal.non-null": EUnary(LogicalNot, stringViewEqualExpression(left, right, true, true));
			case "haxe.i32.less" | "haxe.u32.less" | "haxe.f64.less": EBinary(Less, left, right);
			case "haxe.i32.less-equal" | "haxe.u32.less-equal" | "haxe.f64.less-equal": EBinary(LessEqual, left, right);
			case "haxe.i32.greater" | "haxe.u32.greater" | "haxe.f64.greater": EBinary(Greater, left, right);
			case "haxe.i32.greater-equal" | "haxe.u32.greater-equal" | "haxe.f64.greater-equal": EBinary(GreaterEqual, left, right);
			case _: fail('binary instruction `$instructionId` in `$functionId` has unsupported direct operation `$operationId`');
		};
	}

	/**
		Compare Haxe String contents without allocating or comparing data pointers.

		A null data pointer is Haxe `null`, while every real String--including the
		empty String--has a non-null byte address. Two nulls compare equal; exactly
		one null compares unequal. Non-null values compare by canonical UTF-8 bytes,
		and zero-length values avoid calling `memcmp`.
	**/
	static function stringViewEqualExpression(left:CExpr, right:CExpr, leftKnownNonNull:Bool, rightKnownNonNull:Bool):CExpr {
		final data = new CIdentifier("data");
		final byteLength = new CIdentifier("byte_length");
		final leftData = EMember(left, data, false);
		final rightData = EMember(right, data, false);
		final leftIsNull = EBinary(Equal, leftData, ENull);
		final rightIsNull = EBinary(Equal, rightData, ENull);
		final leftIsNotNull = EBinary(NotEqual, leftData, ENull);
		final rightIsNotNull = EBinary(NotEqual, rightData, ENull);
		final leftLength = EMember(left, byteLength, false);
		final rightLength = EMember(right, byteLength, false);
		final sameLength = EBinary(Equal, leftLength, rightLength);
		final empty = EBinary(Equal, leftLength, EInt(CIntegerLiteral.decimal("0")));
		final sameBytes = EBinary(Equal, ECall(EIdentifier(new CIdentifier("memcmp")), [leftData, rightData, leftLength]), EInt(CIntegerLiteral.decimal("0")));
		final bothNonNullEqual = EBinary(LogicalAnd, sameLength, EBinary(LogicalOr, empty, sameBytes));
		if (leftKnownNonNull && rightKnownNonNull)
			return bothNonNullEqual;
		if (leftKnownNonNull)
			return EBinary(LogicalAnd, rightIsNotNull, bothNonNullEqual);
		if (rightKnownNonNull)
			return EBinary(LogicalAnd, leftIsNotNull, bothNonNullEqual);
		return EConditional(EBinary(LogicalOr, leftIsNull, rightIsNull), EBinary(Equal, leftData, rightData), bothNonNullEqual);
	}

	/**
		Keep UInt operations direct while defeating C integer promotions on targets
		where every uint32_t value is representable by a wider signed int.
	 */
	static function widenedUInt32Binary(operation:CBinaryOp, left:CExpr, right:CExpr):CExpr
		return castUInt32(EBinary(operation, castUInt64(castUInt32(left)), castUInt64(castUInt32(right))));

	static function widenedUInt32Shift(operation:CBinaryOp, value:CExpr, count:CExpr):CExpr
		return castUInt32(EBinary(operation, castUInt64(castUInt32(value)), maskedShiftCount(count)));

	static function maskedShiftCount(value:CExpr):CExpr
		return EBinary(BitAnd, castUInt32(value), uint32Constant("31"));

	static function castUInt32(value:CExpr):CExpr
		return ECast(new CType(TInt(32, false)), DName(null), value);

	static function castUInt64(value:CExpr):CExpr
		return ECast(new CType(TInt(64, false)), DName(null), value);

	static function uint32Constant(value:String):CExpr
		return castUInt32(EInt(CIntegerLiteral.decimal(value)));

	static function helperCall(helperId:String, operands:Array<CExpr>, helperNames:Map<String, CIdentifier>, instructionId:String, functionId:String):CExpr {
		final name = helperNames.get(helperId);
		if (name == null) {
			return fail('primitive instruction `$instructionId` in `$functionId` has no selected helper `$helperId`');
		}
		return ECall(EIdentifier(name), operands);
	}

	public function cType(type:HxcIRTypeRef):CType {
		return switch type {
			case IRTVoid: new CType(TVoid);
			case IRTBool: new CType(TBool);
			case IRTInt(width, signed): new CType(TInt(width, signed));
			case IRTAbiInteger(IRAKSize): new CType(TSizeT);
			case IRTAbiInteger(IRAKPtrDiff): new CType(TNamed(new CIdentifier("ptrdiff_t")));
			case IRTAbiInteger(IRAKIntPtr): new CType(TNamed(new CIdentifier("intptr_t")));
			case IRTAbiInteger(IRAKUIntPtr): new CType(TNamed(new CIdentifier("uintptr_t")));
			case IRTFloat(32): new CType(TFloat);
			case IRTFloat(64): new CType(TDouble);
			case IRTString | IRTManagedString: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringType)));
			case IRTNullable(_, IRNTagged): new CType(TStruct(requireOptional(type).cTag));
			case IRTInstance(instanceId):
				if (arrayElementTypes.exists(instanceId))
					throw new CBodyEmissionError('managed Array instance `$instanceId` requires pointer declarator context');
				if (intMapInstanceIds.exists(instanceId))
					throw new CBodyEmissionError('managed IntMap instance `$instanceId` requires pointer declarator context');
				if (stringMapValueTypes.exists(instanceId))
					throw new CBodyEmissionError('managed StringMap instance `$instanceId` requires pointer declarator context');
				if (bytesInstanceIds.exists(instanceId))
					throw new CBodyEmissionError('managed Bytes instance `$instanceId` requires pointer declarator context');
				final imported = imports.typeByInstance(instanceId);
				if (imported != null) {
					switch imported.prepared.kind {
						case CITStruct: new CType(TStruct(imported.cName));
						case CITEnum: new CType(TNamed(imported.cName));
						case CITTypedef: new CType(TNamed(imported.cName));
					}
				} else {
					final interfaceLayout = interfaceLayoutsByInstance.get(instanceId);
					if (interfaceLayout != null) {
						final valueTag = interfaceLayout.cValueTag;
						if (valueTag == null)
							throw new CBodyEmissionError('interface instance `$instanceId` lost its finalized value tag');
						new CType(TStruct(valueTag));
					} else {
						final aggregateTag = aggregateTags.get(instanceId);
						if (aggregateTag != null) {
							new CType(TStruct(aggregateTag));
						} else {
							final classTag = classTags.get(instanceId);
							if (classTag != null) {
								new CType(TStruct(classTag));
							} else {
								switch requireEnumRepresentation(instanceId) {
									case CBECNative: new CType(TEnum(requireEnumValueTag(instanceId)));
									case CBECTagged: new CType(TStruct(requireEnumValueTag(instanceId)));
								}
							}
						}
					}
				}
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` is outside the admitted direct-value C body subset');
		};
	}

	public function typedDeclarator(type:HxcIRTypeRef, inner:CDeclarator):CTypedDeclarator {
		return switch type {
			case IRTInstance(instanceId) if (arrayElementTypes.exists(instanceId)):
				{type: new CType(TStruct(new CIdentifier("hxc_array_ref"))), declarator: DPointer(inner, [])};
			case IRTInstance(instanceId) if (intMapInstanceIds.exists(instanceId)):
				{type: new CType(TStruct(new CIdentifier("hxc_int_bool_map_ref"))), declarator: DPointer(inner, [])};
			case IRTInstance(instanceId) if (stringMapValueTypes.exists(instanceId)):
				{type: new CType(TStruct(new CIdentifier("hxc_string_map_ref"))), declarator: DPointer(inner, [])};
			case IRTInstance(instanceId) if (bytesInstanceIds.exists(instanceId)):
				{type: new CType(TStruct(new CIdentifier("hxc_bytes_ref"))), declarator: DPointer(inner, [])};
			case IRTCString:
				{type: new CType(TChar(null), [QConst]), declarator: DPointer(inner, [])};
			case IRTFunction(parameters, result):
				final cParameters:Array<CParam> = [];
				for (parameter in parameters) {
					final value = typedDeclarator(parameter, DName(null));
					cParameters.push({type: value.type, declarator: value.declarator, attributes: []});
				}
				// C requires parentheses around `*name` before the function suffix:
				// `Result (*name)(Args)`, not `Result *name(Args)`.
				final callable = DFunction(DGroup(DPointer(inner, [])), FPPrototype(cParameters, false));
				typedDeclarator(result, callable);
			case IRTPointer(pointee, _):
				final nested = typedDeclarator(pointee, DPointer(inner, []));
				{type: nested.type, declarator: nested.declarator};
			case IRTFixedArray(element, length, _):
				final nested = typedDeclarator(element, DArray(inner, ABFixed(EInt(CIntegerLiteral.decimal(Std.string(length)))), []));
				{type: nested.type, declarator: nested.declarator};
			case _:
				{type: cType(type), declarator: inner};
		};
	}

	public function functionDeclarator(fn:HxcIRFunction, inner:CDeclarator):CTypedDeclarator {
		return switch fn.failureConvention {
			case IRFCInfallible: typedDeclarator(fn.returnType, inner);
			case IRFCStatus(_): {type: new CType(TBool), declarator: inner};
		};
	}

	public function globalInitializer(global:HxcIRGlobal):Null<CInitializer> {
		return switch global.initialization {
			case IRGIConstant(value): IExpr(constantExpression(value));
			case IRGIUninitialized | IRGIDeferred(_): null;
		};
	}

	public function requiredGlobalHeaders(global:HxcIRGlobal):Array<String> {
		final headers:Array<String> = [];
		addTypeHeaders(headers, global.type);
		return headers;
	}

	public function requiredHeaders(fn:HxcIRFunction):Array<String> {
		final headers:Array<String> = [];
		if (fn.managedRoots != null && fn.managedRoots.length > 0) {
			addUnique(headers, "hxrt/gc.h");
			addUnique(headers, "stdlib.h");
		}
		switch fn.failureConvention {
			case IRFCStatus(_):
				addUnique(headers, "stdbool.h");
			case IRFCInfallible:
		}
		addTypeHeaders(headers, fn.returnType);
		for (parameter in fn.parameters) {
			addTypeHeaders(headers, parameter.type);
		}
		for (local in fn.locals) {
			addTypeHeaders(headers, local.type);
		}
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				if (instruction.result != null) {
					addTypeHeaders(headers, instruction.result.type);
				}
				switch instruction.kind {
					case IRIOCall(call) if (isHostedOutputDispatch(call.dispatch)):
						addUnique(headers, "hxrt/io.h");
						addUnique(headers, "stdlib.h");
					case IRIOCall({dispatch: IRCDRuntime("string-scalar", _)}):
						addUnique(headers, "hxrt/string_scalar.h");
					case IRIOBinary("haxe.string.equal" | "haxe.string.equal.left-non-null" | "haxe.string.equal.right-non-null" | "haxe.string.equal.non-null" | "haxe.string.not-equal" | "haxe.string.not-equal.left-non-null" | "haxe.string.not-equal.right-non-null" | "haxe.string.not-equal.non-null",
						_, _, IRIStatic):
						addUnique(headers, "string.h");
					case IRIOBoundsCheck(_, _, IRBPCheckedAbort(_, _)):
						addUnique(headers, "stddef.h");
						addUnique(headers, "stdlib.h");
					case IRIOProjectTag(_, _, _, IRTCPCheckedAbort(_, _)):
						addUnique(headers, "stdlib.h");
					case IRIONullCheck(_, IRNCPCheckedAbort(_, _)):
						addUnique(headers, "stddef.h");
						addUnique(headers, "stdlib.h");
					case IRIOCall({failure: {target: IRFTAbort}}):
						addUnique(headers, "stdlib.h");
					case IRIOAllocate(_, _, IRIRuntime("alloc"), _):
						addUnique(headers, "hxrt/allocator.h");
						addUnique(headers, "stdlib.h");
					case _:
				}
			}
			if (block.terminator != null) {
				switch block.terminator.kind {
					case IRTThrow(_, {target: IRFTAbort}) | IRTUnreachable:
						addUnique(headers, "stdlib.h");
					case _:
				}
			}
		}
		headers.sort(compareUtf8);
		return headers;
	}

	public function parameters(fn:HxcIRFunction, names:Map<String, CIdentifier>, ?spanLengthNames:Map<String, CIdentifier>):Array<CParam> {
		final result:Array<CParam> = [];
		final resolvedSpanLengthNames:Map<String, CIdentifier> = spanLengthNames == null ? [] : spanLengthNames;
		for (parameter in fn.parameters) {
			switch parameter.type {
				case IRTSpan(element, mutable):
					result.push({
						type: mutable ? cType(element) : constType(element),
						declarator: DPointer(DName(requireParameterName(names, parameter.id, fn.id)), []),
						attributes: []
					});
					result.push({
						type: new CType(TSizeT),
						declarator: DName(requireSpanLengthName(resolvedSpanLengthNames, parameter.id, fn.id)),
						attributes: []
					});
				case _:
					final declaration = typedDeclarator(parameter.type, DName(requireParameterName(names, parameter.id, fn.id)));
					result.push({
						type: declaration.type,
						declarator: declaration.declarator,
						attributes: []
					});
			}
		}
		return result;
	}

	public function aggregateDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in aggregateInstanceOrder)
			result.push(aggregateDefinition(instanceId));
		return result;
	}

	/** Aggregate tags are dependency-neutral and may be shared before definitions. */
	public function aggregateForwardDeclarations():Array<CDecl>
		return [
			for (instanceId in aggregateInstanceOrder)
				DForwardStruct(requireAggregateTag(instanceId), [])
		];

	/** Stable semantic order used when project layout assigns type definitions. */
	public function orderedAggregateInstanceIds():Array<String>
		return aggregateInstanceOrder.copy();

	/** One complete aggregate definition; ownership remains a project-plan fact. */
	public function aggregateDefinition(instanceId:String):CDecl {
		final order = requireAggregateFieldOrder(instanceId);
		final fields:Array<CField> = [];
		for (fieldName in order) {
			final declaration = typedDeclarator(requireAggregateFieldIrType(instanceId, fieldName),
				DName(requireAggregateFieldName(instanceId, fieldName, "definition", instanceId)));
			fields.push({
				type: declaration.type,
				declarator: declaration.declarator,
				bitWidth: null,
				alignments: [],
				attributes: []
			});
		}
		return DStruct(requireAggregateTag(instanceId), fields, []);
	}

	/** Forward declarations for direct optional values used across module headers. */
	public function optionalForwardDeclarations():Array<CDecl>
		return [
			for (planId in optionalPlanOrder)
				DForwardStruct(requireOptionalPlan(planId).cTag, [])
		];

	/** Stable semantic plan order used by project-layout assignment. */
	public function orderedOptionalPlanIds():Array<String>
		return optionalPlanOrder.copy();

	/** One readable `{ has_value, value }` C definition for a nullable record. */
	public function optionalDefinition(planId:String):CDecl {
		final optional = requireOptionalPlan(planId);
		final payload = typedDeclarator(optional.prepared.payload.irType, DName(optional.payloadName));
		return DStruct(optional.cTag, [
			{
				type: new CType(TBool),
				declarator: DName(optional.presenceName),
				bitWidth: null,
				alignments: [],
				attributes: []
			},
			{
				type: payload.type,
				declarator: payload.declarator,
				bitWidth: null,
				alignments: [],
				attributes: []
			}
		], []);
	}

	public function optionalPayloadType(planId:String):HxcIRTypeRef
		return requireOptionalPlan(planId).prepared.payload.irType;

	/** Resolve a tagged nullable type to the stable plan that owns its C struct. */
	public function optionalPlanId(type:HxcIRTypeRef):String
		return requireOptional(type).prepared.planId;

	public function virtualTableForwardDeclarations():Array<CDecl> {
		final result:Array<CDecl> = [];
		final ids = [for (id in virtualLayouts.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids) {
			final layout = requireVirtualLayout(id);
			result.push(DForwardStruct(layout.cTag, []));
			if (layout.cValueTag != null)
				result.push(DForwardStruct(layout.cValueTag, []));
		}
		return result;
	}

	public function virtualTableDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		final ids = [for (id in virtualLayouts.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids) {
			final layout = requireVirtualLayout(id);
			final fields:Array<CField> = [];
			for (slot in layout.slots) {
				final parameters:Array<CParam> = [];
				final receiver = dispatchReceiverDeclarator(layout, slot, DName(null));
				parameters.push({type: receiver.type, declarator: receiver.declarator, attributes: []});
				for (parameterType in slot.parameterTypes) {
					final parameter = typedDeclarator(parameterType, DName(null));
					parameters.push({type: parameter.type, declarator: parameter.declarator, attributes: []});
				}
				final declaration = typedDeclarator(slot.returnType, DFunction(DGroup(DPointer(DName(slot.cMember), [])), FPPrototype(parameters, false)));
				fields.push({
					type: declaration.type,
					declarator: declaration.declarator,
					bitWidth: null,
					alignments: [],
					attributes: []
				});
			}
			if (fields.length == 0)
				fail('virtual layout `${layout.id}` has no reachable slots');
			result.push(DStruct(layout.cTag, fields, []));
			if (layout.cValueTag != null) {
				final objectMember = requireInterfaceObjectMember(layout);
				final tableMember = requireInterfaceTableMember(layout);
				result.push(DStruct(layout.cValueTag, [
					{
						type: new CType(TVoid),
						declarator: DPointer(DName(objectMember), []),
						bitWidth: null,
						alignments: [],
						attributes: []
					},
					{
						type: new CType(TStruct(layout.cTag), [QConst]),
						declarator: DPointer(DName(tableMember), []),
						bitWidth: null,
						alignments: [],
						attributes: []
					}
				], []));
			}
		}
		return result;
	}

	public function virtualThunkPrototypes():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (thunk in virtualThunks) {
			final declaration = virtualThunkDeclarator(thunk, DFunction(DName(thunk.cName), FPPrototype(virtualThunkParameters(thunk), false)));
			result.push(DPrototype([SStatic], [], declaration.type, declaration.declarator, []));
		}
		return result;
	}

	/**
		Declare program-local Array element callbacks in the shared private header.

		The signatures use only runtime-owned `void *`/status types, so declarations
		do not force a record definition into the common header. Definitions are
		emitted after every private record layout is complete.
	**/
	public function arrayElementLifecyclePrototypes():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (value in canonicalManagedAggregates()) {
			result.push(enumLifecyclePrototype(requireAggregateLifecycleName(value.retainName, value, "retain"),
				requireAggregateLifecycleParameter(value.retainParameterName, value, "retain"), false));
			result.push(enumLifecyclePrototype(requireAggregateLifecycleName(value.destroyName, value, "destroy"),
				requireAggregateLifecycleParameter(value.destroyParameterName, value, "destroy"), true));
		}
		for (value in canonicalManagedEnums()) {
			result.push(enumLifecyclePrototype(requireEnumLifecycleName(value.retainName, value, "retain"),
				requireEnumLifecycleParameter(value.retainParameterName, value, "retain"), false));
			result.push(enumLifecyclePrototype(requireEnumLifecycleName(value.destroyName, value, "destroy"),
				requireEnumLifecycleParameter(value.destroyParameterName, value, "destroy"), true));
			if (value.prepared.recursive) {
				result.push(enumRecursivePointerPrototype(recursiveCloneName(value), recursiveCloneParameter(value), true));
				result.push(enumRecursivePointerPrototype(recursiveDestroyName(value), recursiveDestroyParameter(value), false));
			}
		}
		for (value in canonicalManagedOptionals()) {
			result.push(enumLifecyclePrototype(requireOptionalLifecycleName(value.retainName, value, "retain"),
				requireOptionalLifecycleParameter(value.retainParameterName, value, "retain"), false));
			result.push(enumLifecyclePrototype(requireOptionalLifecycleName(value.destroyName, value, "destroy"),
				requireOptionalLifecycleParameter(value.destroyParameterName, value, "destroy"), true));
		}
		for (array in canonicalManagedArrays()) {
			result.push(arrayLifecyclePrototype(requireArrayCallbackName(array.copyName, array, "copy"), array.copyParameterNames, false));
			result.push(arrayLifecyclePrototype(requireArrayCallbackName(array.assignName, array, "assign"), array.assignParameterNames, false));
			result.push(arrayLifecyclePrototype(requireArrayCallbackName(array.destroyName, array, "destroy"), array.destroyParameterNames, true));
		}
		for (map in canonicalManagedStringMaps()) {
			result.push(arrayLifecyclePrototype(requireStringMapCallbackName(map.copyName, map, "copy"), map.copyParameterNames, false));
			result.push(arrayLifecyclePrototype(requireStringMapCallbackName(map.assignName, map, "assign"), map.assignParameterNames, false));
			result.push(arrayLifecyclePrototype(requireStringMapCallbackName(map.destroyName, map, "destroy"), map.destroyParameterNames, true));
		}
		return result;
	}

	/** Emit the typed retain/release policy for every managed Array element. */
	public function arrayElementLifecycleDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (value in canonicalManagedAggregates()) {
			result.push(aggregateRetainDefinition(value));
			result.push(aggregateDestroyDefinition(value));
		}
		for (value in canonicalManagedEnums()) {
			result.push(enumRetainDefinition(value));
			result.push(enumDestroyDefinition(value));
			if (value.prepared.recursive) {
				result.push(enumRecursiveCloneDefinition(value));
				result.push(enumRecursiveDestroyDefinition(value));
			}
		}
		for (value in canonicalManagedOptionals()) {
			result.push(optionalRetainDefinition(value));
			result.push(optionalDestroyDefinition(value));
		}
		for (array in canonicalManagedArrays()) {
			result.push(arrayCopyDefinition(array));
			result.push(arrayAssignDefinition(array));
			result.push(arrayDestroyDefinition(array));
		}
		for (map in canonicalManagedStringMaps()) {
			result.push(stringMapCopyDefinition(map));
			result.push(stringMapAssignDefinition(map));
			result.push(stringMapDestroyDefinition(map));
		}
		return result;
	}

	/** Private-header declarations for descriptors referenced by split modules. */
	public function managedObjectDescriptorDeclarations():Array<CDecl>
		return new CObjectDescriptorEmitter().externDeclarations(managedObjectDescriptorSpecs());

	/**
		Emit exact trace/finalizer functions followed by immutable descriptors.

		Only collector-selected instances appear here. Direct classes and ordinary
		reference-counted Arrays therefore keep their prior C output byte shape and do
		not pull object/collector names into runtime-free programs.
	**/
	public function managedObjectDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in classInstanceOrder) {
			final value = classesByInstance.get(instanceId);
			if (value != null && value.prepared.managedByCollector) {
				if (value.traceName != null)
					result.push(classTraceDefinition(value));
				if (value.finalizerName != null)
					result.push(classFinalizerDefinition(value));
			}
		}
		final arrays = [for (value in arraysByInstance) if (value.prepared.managedByCollector) value];
		arrays.sort((left, right) -> compareUtf8(left.prepared.digest, right.prepared.digest));
		for (array in arrays) {
			result.push(arrayTraceDefinition(array));
			result.push(arrayFinalizerDefinition(array));
		}
		for (declaration in new CObjectDescriptorEmitter().declarations(managedObjectDescriptorSpecs()))
			result.push(declaration);
		return result;
	}

	function managedObjectDescriptorSpecs():Array<CObjectDescriptorSpec> {
		final result:Array<CObjectDescriptorSpec> = [];
		for (instanceId in classInstanceOrder) {
			final value = classesByInstance.get(instanceId);
			if (value == null || !value.prepared.managedByCollector)
				continue;
			if (value.descriptorName == null)
				throw new CBodyEmissionError('managed class `$instanceId` lost its descriptor');
			result.push(new CObjectDescriptorSpec('class.${value.prepared.digest}', value.descriptorName,
				{type: new CType(TStruct(value.cTag)), declarator: DName(null)}, value.traceName, value.finalizerName, true));
		}
		for (array in arraysByInstance) {
			if (!array.prepared.managedByCollector)
				continue;
			if (array.descriptorName == null || array.traceName == null || array.finalizerName == null)
				throw new CBodyEmissionError('collector-managed Array `${array.prepared.instanceId}` lost descriptor callbacks');
			result.push(new CObjectDescriptorSpec('array.${array.prepared.digest}', array.descriptorName,
				{type: new CType(TStruct(new CIdentifier("hxc_array_ref"))), declarator: DName(null)}, array.traceName, array.finalizerName, true));
		}
		return result;
	}

	function classTraceDefinition(value:CLoweredBodyClass):CDecl {
		if (value.traceName == null)
			throw new CBodyEmissionError('managed class `${value.prepared.instanceId}` lost its trace function');
		final objectName = new CIdentifier(value.traceName.value + "_object");
		final visitName = new CIdentifier(value.traceName.value + "_visit");
		final contextName = new CIdentifier(value.traceName.value + "_context");
		final typedName = new CIdentifier(value.traceName.value + "_typed");
		final typed = EIdentifier(typedName);
		final statements:Array<CStmt> = [
			SDecl({
				storage: [],
				alignments: [],
				type: new CType(TStruct(value.cTag), [QConst]),
				declarator: DPointer(DName(typedName), []),
				initializer: IExpr(ECast(new CType(TStruct(value.cTag), [QConst]), DPointer(DName(null), []), EIdentifier(objectName))),
				attributes: []
			})
		];
		appendClassTraceStatements(statements, value.prepared.instanceId, EUnary(Dereference, typed), visitName, contextName);
		return DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(value.traceName), FPPrototype([
				{type: new CType(TVoid, [QConst]), declarator: DPointer(DName(objectName), []), attributes: []},
				{type: new CType(TNamed(new CIdentifier("hxc_trace_visit_fn"))), declarator: DName(visitName), attributes: []},
				{type: new CType(TVoid), declarator: DPointer(DName(contextName), []), attributes: []}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function appendClassTraceStatements(statements:Array<CStmt>, instanceId:String, object:CExpr, visitName:CIdentifier, contextName:CIdentifier):Void {
		final base = classBaseInstances.get(instanceId);
		if (base != null)
			appendClassTraceStatements(statements, base, EMember(object, requireClassBaseMember(instanceId), false), visitName, contextName);
		for (fieldName in requireClassFieldOrder(instanceId)) {
			final type = requireClassFieldType(instanceId, fieldName);
			final field = EMember(object, requireClassFieldName(instanceId, fieldName), false);
			appendManagedTraceStatements(statements, field, type, visitName, contextName);
		}
	}

	/**
		Visit every exact collector pointer reachable through one finite C value.

		This is the heap-container counterpart to HxcIR managed-root projections.
		Tags and optional presence are checked before an overlapping or inactive
		payload is read. Recursive enum nodes remain outside this helper until they
		have a separately validated recursive trace contract.
	**/
	function appendManagedTraceStatements(statements:Array<CStmt>, value:CExpr, type:HxcIRTypeRef, visitName:CIdentifier, contextName:CIdentifier):Void {
		final directManaged = switch type {
			case IRTPointer(IRTInstance(target), _) | IRTInstance(target): managedDescriptorNames.exists(target);
			case _: false;
		};
		if (directManaged) {
			statements.push(SIf(EBinary(NotEqual, value, ENull), SExpr(ECall(EIdentifier(visitName), [EIdentifier(contextName), value])), null));
			return;
		}
		final interfaceInstanceId = switch type {
			case IRTInstance(instanceId) if (interfaceLayoutsByInstance.exists(instanceId)): instanceId;
			case _: null;
		};
		if (interfaceInstanceId != null) {
			final layout = requireInterfaceLayout(interfaceInstanceId);
			final object = EMember(value, requireInterfaceObjectMember(layout), false);
			statements.push(SIf(EBinary(NotEqual, object, ENull), SExpr(ECall(EIdentifier(visitName), [EIdentifier(contextName), object])), null));
			return;
		}
		switch type {
			case IRTInstance(instanceId) if (aggregateFieldOrder.exists(instanceId)):
				for (fieldName in requireAggregateFieldOrder(instanceId))
					appendManagedTraceStatements(statements,
						EMember(value, requireAggregateFieldName(instanceId, fieldName, "managed-trace", instanceId), false),
						requireAggregateFieldIrType(instanceId, fieldName), visitName, contextName);
			case IRTInstance(instanceId) if (enumsByInstance.exists(instanceId) && requireEnumRepresentation(instanceId) == CBECTagged):
				final cases:Array<CCase> = [];
				for (tagName in requireEnumCaseOrder(instanceId)) {
					final body:Array<CStmt> = [];
					for (payloadName in requireEnumPayloadNames(instanceId, tagName)) {
						final payload = EMember(EMember(EMember(value, requireEnumPayloadMember(instanceId), false),
							requireEnumCaseUnionMember(instanceId, tagName), false),
							requireEnumPayloadFieldName(instanceId, tagName, payloadName), false);
						appendManagedTraceStatements(body, payload, requireEnumPayloadFieldType(instanceId, tagName, payloadName), visitName, contextName);
					}
					body.push(SBreak);
					cases.push({values: [EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName))], isDefault: false, body: body});
				}
				statements.push(SSwitch(enumTagExpression(value, instanceId), cases));
			case IRTNullable(_, IRNTagged):
				final optional = requireOptional(type);
				final body:Array<CStmt> = [];
				appendManagedTraceStatements(body, EMember(value, optional.payloadName, false), optional.prepared.payload.irType, visitName, contextName);
				if (body.length > 0)
					statements.push(SIf(EMember(value, optional.presenceName, false), SBlock(body), null));
			case _:
		}
	}

	function arrayTraceDefinition(array:CLoweredBodyArray):CDecl {
		if (array.traceName == null)
			throw new CBodyEmissionError('collector-managed Array `${array.prepared.instanceId}` lost its trace function');
		final objectName = new CIdentifier(array.traceName.value + "_object");
		final visitName = new CIdentifier(array.traceName.value + "_visit");
		final contextName = new CIdentifier(array.traceName.value + "_context");
		final typedName = new CIdentifier(array.traceName.value + "_typed");
		final indexName = new CIdentifier(array.traceName.value + "_index");
		final typed = EIdentifier(typedName);
		final value = EMember(typed, CBodyRuntimeNames.identifier(CBRNArrayValueMember), true);
		final storage = EMember(EMember(value, new CIdentifier("storage"), false), new CIdentifier("memory"), false);
		final element = typedDeclarator(array.prepared.element.irType, DName(null));
		final elementStorage = storagePointerDeclarator(element, true);
		final elements = ECast(elementStorage.type, elementStorage.declarator, storage);
		final loop:Array<CStmt> = [];
		appendManagedTraceStatements(loop, EIndex(elements, EIdentifier(indexName)), array.prepared.element.irType, visitName, contextName);
		loop.push(SExpr(EUnary(PostIncrement, EIdentifier(indexName))));
		return DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(array.traceName), FPPrototype([
				{type: new CType(TVoid, [QConst]), declarator: DPointer(DName(objectName), []), attributes: []},
				{type: new CType(TNamed(new CIdentifier("hxc_trace_visit_fn"))), declarator: DName(visitName), attributes: []},
				{type: new CType(TVoid), declarator: DPointer(DName(contextName), []), attributes: []}
			], false)),
			body: SBlock([
				SDecl({
					storage: [],
					alignments: [],
					type: new CType(TStruct(new CIdentifier("hxc_array_ref")), [QConst]),
					declarator: DPointer(DName(typedName), []),
					initializer: IExpr(ECast(new CType(TStruct(new CIdentifier("hxc_array_ref")), [QConst]), DPointer(DName(null), []),
						EIdentifier(objectName))),
					attributes: []
				}),
				SDecl({
					storage: [],
					alignments: [],
					type: new CType(TSizeT),
					declarator: DName(indexName),
					initializer: IExpr(EInt(CIntegerLiteral.decimal("0"))),
					attributes: []
				}),
				SWhile(EBinary(Less, EIdentifier(indexName), EMember(value, new CIdentifier("length"), false)), SBlock(loop))
			]),
			attributes: []
		});
	}

	function classFinalizerDefinition(value:CLoweredBodyClass):CDecl {
		if (value.finalizerName == null)
			throw new CBodyEmissionError('managed class `${value.prepared.instanceId}` lost its finalizer function');
		final objectName = new CIdentifier(value.finalizerName.value + "_object");
		final typedName = new CIdentifier(value.finalizerName.value + "_typed");
		final statements:Array<CStmt> = [
			SDecl({
				storage: [],
				alignments: [],
				type: new CType(TStruct(value.cTag)),
				declarator: DPointer(DName(typedName), []),
				initializer: IExpr(ECast(new CType(TStruct(value.cTag)), DPointer(DName(null), []), EIdentifier(objectName))),
				attributes: []
			})
		];
		appendClassFinalizerStatements(statements, value.prepared.instanceId, EUnary(Dereference, EIdentifier(typedName)));
		return DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(value.finalizerName), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(objectName), []), attributes: []}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function appendClassFinalizerStatements(statements:Array<CStmt>, instanceId:String, object:CExpr):Void {
		final base = classBaseInstances.get(instanceId);
		if (base != null)
			appendClassFinalizerStatements(statements, base, EMember(object, requireClassBaseMember(instanceId), false));
		for (fieldName in requireClassFieldOrder(instanceId)) {
			final type = requireClassFieldType(instanceId, fieldName);
			final managedByCollector = switch type {
				case IRTPointer(IRTInstance(target), _) | IRTInstance(target): managedDescriptorNames.exists(target);
				case _: false;
			};
			final interfaceReference = switch type {
				case IRTInstance(target): interfaceLayoutsByInstance.exists(target);
				case _: false;
			};
			if (managedByCollector || interfaceReference)
				continue;
			final field = EMember(object, requireClassFieldName(instanceId, fieldName), false);
			appendManagedReleases(statements, managedValueOperations(field, type));
		}
	}

	function arrayFinalizerDefinition(array:CLoweredBodyArray):CDecl {
		if (array.finalizerName == null)
			throw new CBodyEmissionError('collector-managed Array `${array.prepared.instanceId}` lost its finalizer');
		final objectName = new CIdentifier(array.finalizerName.value + "_object");
		return DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(array.finalizerName), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(objectName), []), attributes: []}
			], false)),
			body: SBlock([
				ignoreExpression(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayDisposeInPlace)), [
					ECast(new CType(TStruct(new CIdentifier("hxc_array_ref"))), DPointer(DName(null), []), EIdentifier(objectName))
				]))
			]),
			attributes: []
		});
	}

	function aggregateRetainDefinition(value:CLoweredBodyAggregate):CDecl {
		final name = requireAggregateLifecycleName(value.retainName, value, "retain");
		final parameter = requireAggregateLifecycleParameter(value.retainParameterName, value, "retain");
		final status = requireAggregateLifecycleStatus(value.retainStatusName, value);
		final recordValue = aggregateLifecycleStorageValue(value, EIdentifier(parameter));
		final operations = managedValueOperations(recordValue, IRTInstance(value.prepared.instanceId));
		final statements:Array<CStmt> = [statusDeclaration(status)];
		for (index in 0...operations.length) {
			statements.push(SExpr(EBinary(Assign, EIdentifier(status), operations[index].retain)));
			final rollback:Array<CStmt> = [];
			var prior = index;
			while (prior != 0) {
				prior--;
				rollback.push(ignoreExpression(operations[prior].release));
			}
			rollback.push(SReturn(EIdentifier(status)));
			statements.push(SIf(EBinary(NotEqual, EIdentifier(status), EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SBlock(rollback), null));
		}
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype([
				{
					type: new CType(TVoid),
					declarator: DPointer(DName(parameter), []),
					attributes: []
				}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function aggregateDestroyDefinition(value:CLoweredBodyAggregate):CDecl {
		final name = requireAggregateLifecycleName(value.destroyName, value, "destroy");
		final parameter = requireAggregateLifecycleParameter(value.destroyParameterName, value, "destroy");
		final recordValue = aggregateLifecycleStorageValue(value, EIdentifier(parameter));
		final operations = managedValueOperations(recordValue, IRTInstance(value.prepared.instanceId));
		final statements:Array<CStmt> = [];
		appendManagedReleases(statements, operations);
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function aggregateLifecycleStorageValue(value:CLoweredBodyAggregate, rawPointer:CExpr):CExpr {
		final type = new CType(TStruct(value.cTag));
		return EUnary(Dereference, ECast(type, DPointer(DName(null), []), rawPointer));
	}

	function optionalRetainDefinition(value:CLoweredBodyOptional):CDecl {
		final name = requireOptionalLifecycleName(value.retainName, value, "retain");
		final parameter = requireOptionalLifecycleParameter(value.retainParameterName, value, "retain");
		final status = requireOptionalLifecycleStatus(value.retainStatusName, value);
		final optionalValue = optionalLifecycleStorageValue(value, EIdentifier(parameter));
		final payload = EMember(optionalValue, value.payloadName, false);
		final statements:Array<CStmt> = [statusDeclaration(status)];
		statements.push(SIf(EUnary(LogicalNot, EMember(optionalValue, value.presenceName, false)),
			SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), null));
		statements.push(SExpr(EBinary(Assign, EIdentifier(status), managedOptionalRetainCall(value, payload))));
		statements.push(SIf(EBinary(NotEqual, EIdentifier(status), EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SReturn(EIdentifier(status)),
			null));
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype([
				{
					type: new CType(TVoid),
					declarator: DPointer(DName(parameter), []),
					attributes: []
				}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function optionalDestroyDefinition(value:CLoweredBodyOptional):CDecl {
		final name = requireOptionalLifecycleName(value.destroyName, value, "destroy");
		final parameter = requireOptionalLifecycleParameter(value.destroyParameterName, value, "destroy");
		final optionalValue = optionalLifecycleStorageValue(value, EIdentifier(parameter));
		final payload = EMember(optionalValue, value.payloadName, false);
		final statements:Array<CStmt> = [
			SIf(EMember(optionalValue, value.presenceName, false), SBlock([SExpr(managedOptionalDestroyCall(value, payload))]), null)
		];
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function optionalLifecycleStorageValue(value:CLoweredBodyOptional, rawPointer:CExpr):CExpr {
		final type = new CType(TStruct(value.cTag));
		return EUnary(Dereference, ECast(type, DPointer(DName(null), []), rawPointer));
	}

	/** Retain the one closed managed payload family selected by this optional. */
	function managedOptionalRetainCall(value:CLoweredBodyOptional, payload:CExpr):CExpr {
		final preparedAggregate = value.prepared.payload.aggregateValue();
		if (preparedAggregate != null && preparedAggregate.managedLifetime) {
			final aggregate = aggregatesByInstance.get(preparedAggregate.instanceId);
			if (aggregate == null)
				throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost finalized record `${preparedAggregate.instanceId}`');
			return ECall(EIdentifier(requireAggregateLifecycleName(aggregate.retainName, aggregate, "retain")), [EUnary(AddressOf, payload)]);
		}
		final preparedEnum = value.prepared.payload.enumValue();
		if (preparedEnum != null && preparedEnum.managedLifetime) {
			final enumValue = enumsByInstance.get(preparedEnum.instanceId);
			if (enumValue == null)
				throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost finalized enum `${preparedEnum.instanceId}`');
			return ECall(EIdentifier(requireEnumLifecycleName(enumValue.retainName, enumValue, "retain")), [EUnary(AddressOf, payload)]);
		}
		throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its managed record or enum payload');
	}

	/** Destroy the present payload through the same family chosen for retain. */
	function managedOptionalDestroyCall(value:CLoweredBodyOptional, payload:CExpr):CExpr {
		final preparedAggregate = value.prepared.payload.aggregateValue();
		if (preparedAggregate != null && preparedAggregate.managedLifetime) {
			final aggregate = aggregatesByInstance.get(preparedAggregate.instanceId);
			if (aggregate == null)
				throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost finalized record `${preparedAggregate.instanceId}`');
			return ECall(EIdentifier(requireAggregateLifecycleName(aggregate.destroyName, aggregate, "destroy")), [EUnary(AddressOf, payload)]);
		}
		final preparedEnum = value.prepared.payload.enumValue();
		if (preparedEnum != null && preparedEnum.managedLifetime) {
			final enumValue = enumsByInstance.get(preparedEnum.instanceId);
			if (enumValue == null)
				throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost finalized enum `${preparedEnum.instanceId}`');
			return ECall(EIdentifier(requireEnumLifecycleName(enumValue.destroyName, enumValue, "destroy")), [EUnary(AddressOf, payload)]);
		}
		throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its managed record or enum payload');
	}

	function enumLifecyclePrototype(name:CIdentifier, parameter:CIdentifier, destroy:Bool):CDecl {
		final returnType = destroy ? new CType(TVoid) : new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType)));
		return DPrototype([], [], returnType, DFunction(DName(name), FPPrototype([
			{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
		], false)), []);
	}

	function enumRecursivePointerPrototype(name:CIdentifier, parameter:CIdentifier, returnsStatus:Bool):CDecl {
		final returnType = returnsStatus ? new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))) : new CType(TVoid);
		return DPrototype([], [], returnType, DFunction(DName(name), FPPrototype([
			{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
		], false)), []);
	}

	function enumRetainDefinition(value:CLoweredBodyEnum):CDecl {
		final name = requireEnumLifecycleName(value.retainName, value, "retain");
		final parameter = requireEnumLifecycleParameter(value.retainParameterName, value, "retain");
		final status = requireEnumLifecycleStatus(value.retainStatusName, value);
		final enumValue = enumLifecycleStorageValue(value, EIdentifier(parameter), false);
		final cases:Array<CCase> = [];
		for (tagCase in value.cases) {
			final operations = enumManagedOperations(value, tagCase, enumValue);
			final body:Array<CStmt> = [];
			for (index in 0...operations.length) {
				body.push(SExpr(EBinary(Assign, EIdentifier(status), operations[index].retain)));
				final rollback:Array<CStmt> = [];
				var prior = index;
				while (prior != 0) {
					prior--;
					rollback.push(ignoreExpression(operations[prior].release));
				}
				rollback.push(SReturn(EIdentifier(status)));
				body.push(SIf(EBinary(NotEqual, EIdentifier(status), EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SBlock(rollback), null));
			}
			body.push(SBreak);
			cases.push({values: [EIdentifier(tagCase.discriminant)], isDefault: false, body: body});
		}
		final statements:Array<CStmt> = [
			statusDeclaration(status),
			SSwitch(enumTagExpression(enumValue, value.prepared.instanceId), cases),
			SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk)))
		];
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype([
				{
					type: new CType(TVoid),
					declarator: DPointer(DName(parameter), []),
					attributes: []
				}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function enumDestroyDefinition(value:CLoweredBodyEnum):CDecl {
		final name = requireEnumLifecycleName(value.destroyName, value, "destroy");
		final parameter = requireEnumLifecycleParameter(value.destroyParameterName, value, "destroy");
		final enumValue = enumLifecycleStorageValue(value, EIdentifier(parameter), false);
		final cases:Array<CCase> = [];
		for (tagCase in value.cases) {
			final operations = enumManagedOperations(value, tagCase, enumValue);
			final body:Array<CStmt> = [];
			var index = operations.length;
			while (index != 0) {
				index--;
				body.push(ignoreExpression(operations[index].release));
			}
			body.push(SBreak);
			cases.push({values: [EIdentifier(tagCase.discriminant)], isDefault: false, body: body});
		}
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
			], false)),
			body: SBlock([SSwitch(enumTagExpression(enumValue, value.prepared.instanceId), cases)]),
			attributes: []
		});
	}

	function enumManagedOperations(value:CLoweredBodyEnum, tagCase:reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnumCase,
			enumValue:CExpr):Array<CBodyEmitterManagedOperation> {
		final result:Array<CBodyEmitterManagedOperation> = [];
		for (payload in tagCase.payload) {
			final field = EMember(EMember(EMember(enumValue, requireEnumPayloadMember(value.prepared.instanceId), false),
				requireEnumCaseUnionMember(value.prepared.instanceId, tagCase.prepared.name), false),
				payload.cName, false);
			if (payload.prepared.indirect) {
				final nestedPrepared = payload.prepared.valueType.enumValue();
				final nested = nestedPrepared == null ? null : enumsByInstance.get(nestedPrepared.instanceId);
				if (nested == null || !nested.prepared.recursive)
					throw new CBodyEmissionError('recursive enum `${value.prepared.instanceId}` lost nested payload `${payload.prepared.name}`');
				result.push({
					retain: ECall(EIdentifier(recursiveCloneName(nested)), [EUnary(AddressOf, field)]),
					release: ECall(EIdentifier(recursiveDestroyName(nested)), [EUnary(AddressOf, field)])
				});
			} else {
				final aggregatePrepared = payload.prepared.valueType.aggregateValue();
				if (aggregatePrepared != null && aggregatePrepared.managedLifetime) {
					final aggregate = aggregatesByInstance.get(aggregatePrepared.instanceId);
					if (aggregate == null)
						throw new CBodyEmissionError('managed enum `${value.prepared.instanceId}` lost record payload `${aggregatePrepared.instanceId}`');
					result.push({
						retain: ECall(EIdentifier(requireAggregateLifecycleName(aggregate.retainName, aggregate, "retain")), [EUnary(AddressOf, field)]),
						release: ECall(EIdentifier(requireAggregateLifecycleName(aggregate.destroyName, aggregate, "destroy")), [EUnary(AddressOf, field)])
					});
					continue;
				}
				for (operation in managedValueOperations(field, payload.prepared.storageType()))
					result.push(operation);
			}
		}
		return result;
	}

	/** Deep-copy one unique recursive enum node after its containing value was copied. */
	function enumRecursiveCloneDefinition(value:CLoweredBodyEnum):CDecl {
		final name = recursiveCloneName(value);
		final parameter = recursiveCloneParameter(value);
		final typedSlotName = derivedLifecycleName(name, "typed_slot");
		final sourceName = derivedLifecycleName(name, "source");
		final copyName = derivedLifecycleName(name, "copy");
		final allocatorName = derivedLifecycleName(name, "allocator");
		final statusName = derivedLifecycleName(name, "operation_status");
		final enumType = new CType(TStruct(value.valueTag));
		final typedSlot = EIdentifier(typedSlotName);
		final source = EIdentifier(sourceName);
		final copy = EIdentifier(copyName);
		final allocator = EIdentifier(allocatorName);
		final status = EIdentifier(statusName);
		final statements:Array<CStmt> = [
			SDecl({
				storage: [],
				alignments: [],
				type: enumType,
				declarator: DPointer(DPointer(DName(typedSlotName), []), []),
				initializer: IExpr(ECast(enumType, DPointer(DPointer(DName(null), []), []), EIdentifier(parameter))),
				attributes: []
			}),
			SDecl({
				storage: [],
				alignments: [],
				type: enumType,
				declarator: DPointer(DName(sourceName), []),
				initializer: IExpr(EUnary(Dereference, typedSlot)),
				attributes: []
			}),
			SDecl({
				storage: [],
				alignments: [],
				type: enumType,
				declarator: DPointer(DName(copyName), []),
				initializer: IExpr(ENull),
				attributes: []
			}),
			SDecl({
				storage: [],
				alignments: [],
				type: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNAllocatorType))),
				declarator: DName(allocatorName),
				initializer: IExpr(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), [])),
				attributes: []
			}),
			statusDeclaration(statusName),
			SExpr(EBinary(Assign, status,
				ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNAllocate)),
					[
						EUnary(AddressOf, allocator),
						ESizeOfType(enumType, DName(null)),
						EAlignOfType(enumType, DName(null)),
						ECast(new CType(TVoid), DPointer(DPointer(DName(null), []), []), EUnary(AddressOf, copy))
					]))),
			SIf(EBinary(NotEqual, status, EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SBlock([SReturn(status)]), null),
			SExpr(EBinary(Assign, EUnary(Dereference, copy), EUnary(Dereference, source))),
			SExpr(EBinary(Assign, status, ECall(EIdentifier(requireEnumLifecycleName(value.retainName, value, "retain")), [copy]))),
			SIf(EBinary(NotEqual, status, EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SBlock([
				ignoreExpression(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNFree)),
					[
						EUnary(AddressOf, allocator),
						copy,
						ESizeOfType(enumType, DName(null)),
						EAlignOfType(enumType, DName(null))
					])),
				SReturn(status)
			]), null),
			SExpr(EBinary(Assign, EUnary(Dereference, typedSlot), copy)),
			SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk)))
		];
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype([
				{
					type: new CType(TVoid),
					declarator: DPointer(DName(parameter), []),
					attributes: []
				}
			], false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	/** Destroy and free one unique recursive enum node, then clear its owner slot. */
	function enumRecursiveDestroyDefinition(value:CLoweredBodyEnum):CDecl {
		final name = recursiveDestroyName(value);
		final parameter = recursiveDestroyParameter(value);
		final typedSlotName = derivedLifecycleName(name, "typed_slot");
		final ownedName = derivedLifecycleName(name, "owned");
		final allocatorName = derivedLifecycleName(name, "allocator");
		final enumType = new CType(TStruct(value.valueTag));
		final typedSlot = EIdentifier(typedSlotName);
		final owned = EIdentifier(ownedName);
		final allocator = EIdentifier(allocatorName);
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype([
				{type: new CType(TVoid), declarator: DPointer(DName(parameter), []), attributes: []}
			], false)),
			body: SBlock([
				SDecl({
					storage: [],
					alignments: [],
					type: enumType,
					declarator: DPointer(DPointer(DName(typedSlotName), []), []),
					initializer: IExpr(ECast(enumType, DPointer(DPointer(DName(null), []), []), EIdentifier(parameter))),
					attributes: []
				}),
				SDecl({
					storage: [],
					alignments: [],
					type: enumType,
					declarator: DPointer(DName(ownedName), []),
					initializer: IExpr(EUnary(Dereference, typedSlot)),
					attributes: []
				}),
				SDecl({
					storage: [],
					alignments: [],
					type: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNAllocatorType))),
					declarator: DName(allocatorName),
					initializer: IExpr(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), [])),
					attributes: []
				}),
				SExpr(ECall(EIdentifier(requireEnumLifecycleName(value.destroyName, value, "destroy")), [owned])),
				ignoreExpression(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNFree)), [
					      EUnary(AddressOf, allocator),                             owned,
					ESizeOfType(enumType, DName(null)), EAlignOfType(enumType, DName(null))
				])),
				SExpr(EBinary(Assign, EUnary(Dereference, typedSlot), ENull))
			]),
			attributes: []
		});
	}

	static function recursiveCloneName(value:CLoweredBodyEnum):CIdentifier
		return derivedLifecycleName(requireEnumLifecycleName(value.retainName, value, "retain"), "recursive_clone");

	static function recursiveDestroyName(value:CLoweredBodyEnum):CIdentifier
		return derivedLifecycleName(requireEnumLifecycleName(value.destroyName, value, "destroy"), "recursive_destroy");

	static function recursiveCloneParameter(value:CLoweredBodyEnum):CIdentifier
		return derivedLifecycleName(recursiveCloneName(value), "slot");

	static function recursiveDestroyParameter(value:CLoweredBodyEnum):CIdentifier
		return derivedLifecycleName(recursiveDestroyName(value), "slot");

	static function derivedLifecycleName(base:CIdentifier, suffix:String):CIdentifier
		return new CIdentifier(base.value + "_" + suffix);

	function enumLifecycleStorageValue(value:CLoweredBodyEnum, rawPointer:CExpr, readOnly:Bool):CExpr {
		final type = new CType(TStruct(value.valueTag), readOnly ? [QConst] : []);
		return EUnary(Dereference, ECast(type, DPointer(DName(null), []), rawPointer));
	}

	function arrayLifecyclePrototype(name:CIdentifier, parameters:Array<CIdentifier>, destroy:Bool):CDecl {
		final returnType = destroy ? new CType(TVoid) : new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType)));
		return DPrototype([], [], returnType, DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, destroy), false)), []);
	}

	function arrayCopyDefinition(array:CLoweredBodyArray):CDecl {
		final name = requireArrayCallbackName(array.copyName, array, "copy");
		final parameters = requireArrayCallbackParameters(array.copyParameterNames, 3, array, "copy");
		final statusName = requireArrayStatusName(array.copyStatusName, array, "copy");
		final source = arrayElementStorageValue(array, EIdentifier(parameters[2]), true);
		final destination = arrayElementStorageValue(array, EIdentifier(parameters[1]), false);
		final statements:Array<CStmt> = [ignoreExpression(EIdentifier(parameters[0])), statusDeclaration(statusName)];
		// Retain the destination copy, not the caller's source. Most managed leaves
		// merely increment a counter, but recursive enum retain replaces pointer
		// fields with deep copies. Mutating a const source would steal its owner.
		statements.push(SExpr(EBinary(Assign, destination, source)));
		appendArrayElementRetains(statements, destination, array, statusName);
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, false), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function arrayAssignDefinition(array:CLoweredBodyArray):CDecl {
		final name = requireArrayCallbackName(array.assignName, array, "assign");
		final parameters = requireArrayCallbackParameters(array.assignParameterNames, 3, array, "assign");
		final statusName = requireArrayStatusName(array.assignStatusName, array, "assign");
		final source = arrayElementStorageValue(array, EIdentifier(parameters[2]), true);
		final destination = arrayElementStorageValue(array, EIdentifier(parameters[1]), false);
		final statements:Array<CStmt> = [
			ignoreExpression(EIdentifier(parameters[0])),
			SIf(EBinary(Equal, EIdentifier(parameters[1]), EIdentifier(parameters[2])), SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), null),
			statusDeclaration(statusName)
		];
		// Build a fully retained replacement before touching the old destination.
		// This provides strong failure behavior and lets retain deep-copy fields in
		// place without ever casting away the source callback's const contract.
		final replacementName = derivedLifecycleName(name, "replacement");
		final replacement = EIdentifier(replacementName);
		final replacementType = typedDeclarator(array.prepared.element.irType, DName(replacementName));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: replacementType.type,
			declarator: replacementType.declarator,
			initializer: IExpr(source),
			attributes: []
		}));
		appendArrayElementRetains(statements, replacement, array, statusName);
		appendArrayElementReleases(statements, destination, array);
		statements.push(SExpr(EBinary(Assign, destination, replacement)));
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, false), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function arrayDestroyDefinition(array:CLoweredBodyArray):CDecl {
		final name = requireArrayCallbackName(array.destroyName, array, "destroy");
		final parameters = requireArrayCallbackParameters(array.destroyParameterNames, 2, array, "destroy");
		final element = arrayElementStorageValue(array, EIdentifier(parameters[1]), false);
		final statements:Array<CStmt> = [ignoreExpression(EIdentifier(parameters[0]))];
		appendArrayElementReleases(statements, element, array);
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, true), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	/** Construct one owned StringMap slot or lookup result from a borrowed value. */
	function stringMapCopyDefinition(map:CLoweredBodyStringMap):CDecl {
		final name = requireStringMapCallbackName(map.copyName, map, "copy");
		final parameters = requireStringMapCallbackParameters(map.copyParameterNames, 3, map, "copy");
		final statusName = requireStringMapStatusName(map.copyStatusName, map, "copy");
		final source = stringMapValueStorage(map, EIdentifier(parameters[2]), true);
		final destination = stringMapValueStorage(map, EIdentifier(parameters[1]), false);
		final statements:Array<CStmt> = [
			ignoreExpression(EIdentifier(parameters[0])),
			statusDeclaration(statusName),
			SExpr(EBinary(Assign, destination, source))
		];
		appendManagedRetains(statements, managedValueOperations(destination, map.prepared.value.irType), statusName);
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, false), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	/**
		Replace one StringMap slot only after a complete retained copy exists.

		The temporary is important when old and new records share a nested Array:
		retaining first prevents release of the old slot from destroying storage
		that the replacement still needs.
	**/
	function stringMapAssignDefinition(map:CLoweredBodyStringMap):CDecl {
		final name = requireStringMapCallbackName(map.assignName, map, "assign");
		final parameters = requireStringMapCallbackParameters(map.assignParameterNames, 3, map, "assign");
		final statusName = requireStringMapStatusName(map.assignStatusName, map, "assign");
		final source = stringMapValueStorage(map, EIdentifier(parameters[2]), true);
		final destination = stringMapValueStorage(map, EIdentifier(parameters[1]), false);
		final replacementName = derivedLifecycleName(name, "replacement");
		final replacement = EIdentifier(replacementName);
		final replacementType = typedDeclarator(map.prepared.value.irType, DName(replacementName));
		final statements:Array<CStmt> = [
			ignoreExpression(EIdentifier(parameters[0])),
			SIf(EBinary(Equal, EIdentifier(parameters[1]), EIdentifier(parameters[2])), SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), null),
			statusDeclaration(statusName),
			SDecl({
				storage: [],
				alignments: [],
				type: replacementType.type,
				declarator: replacementType.declarator,
				initializer: IExpr(source),
				attributes: []
			})
		];
		appendManagedRetains(statements, managedValueOperations(replacement, map.prepared.value.irType), statusName);
		appendManagedReleases(statements, managedValueOperations(destination, map.prepared.value.irType));
		statements.push(SExpr(EBinary(Assign, destination, replacement)));
		statements.push(SReturn(EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, false), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	/** Release every managed field owned by one live StringMap value slot. */
	function stringMapDestroyDefinition(map:CLoweredBodyStringMap):CDecl {
		final name = requireStringMapCallbackName(map.destroyName, map, "destroy");
		final parameters = requireStringMapCallbackParameters(map.destroyParameterNames, 2, map, "destroy");
		final value = stringMapValueStorage(map, EIdentifier(parameters[1]), false);
		final statements:Array<CStmt> = [ignoreExpression(EIdentifier(parameters[0]))];
		appendManagedReleases(statements, managedValueOperations(value, map.prepared.value.irType));
		return DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TVoid),
			declarator: DFunction(DName(name), FPPrototype(arrayLifecycleParameters(parameters, true), false)),
			body: SBlock(statements),
			attributes: []
		});
	}

	function appendManagedRetains(statements:Array<CStmt>, operations:Array<CBodyEmitterManagedOperation>, statusName:CIdentifier):Void {
		for (index in 0...operations.length) {
			statements.push(SExpr(EBinary(Assign, EIdentifier(statusName), operations[index].retain)));
			final rollback:Array<CStmt> = [];
			var prior = index;
			while (prior != 0) {
				prior--;
				rollback.push(ignoreExpression(operations[prior].release));
			}
			rollback.push(SReturn(EIdentifier(statusName)));
			statements.push(SIf(EBinary(NotEqual, EIdentifier(statusName), EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))), SBlock(rollback), null));
		}
	}

	function appendArrayElementRetains(statements:Array<CStmt>, value:CExpr, array:CLoweredBodyArray, statusName:CIdentifier):Void {
		switch array.prepared.lifecycle {
			case CBAELEnum(enumValue):
				final lowered = enumsByInstance.get(enumValue.instanceId);
				if (lowered == null)
					throw new CBodyEmissionError('managed Array `${array.prepared.semanticKey}` lost enum `${enumValue.instanceId}`');
				statements.push(SExpr(EBinary(Assign, EIdentifier(statusName),
					ECall(EIdentifier(requireEnumLifecycleName(lowered.retainName, lowered, "retain")), [EUnary(AddressOf, value)]))));
				statements.push(SIf(EBinary(NotEqual, EIdentifier(statusName), EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))),
					SReturn(EIdentifier(statusName)), null));
			case _:
				appendManagedRetains(statements, managedValueOperations(value, array.prepared.element.irType), statusName);
		}
	}

	function appendArrayElementReleases(statements:Array<CStmt>, value:CExpr, array:CLoweredBodyArray):Void {
		switch array.prepared.lifecycle {
			case CBAELEnum(enumValue):
				final lowered = enumsByInstance.get(enumValue.instanceId);
				if (lowered == null)
					throw new CBodyEmissionError('managed Array `${array.prepared.semanticKey}` lost enum `${enumValue.instanceId}`');
				statements.push(SExpr(ECall(EIdentifier(requireEnumLifecycleName(lowered.destroyName, lowered, "destroy")), [EUnary(AddressOf, value)])));
			case _:
				appendManagedReleases(statements, managedValueOperations(value, array.prepared.element.irType));
		}
	}

	function appendManagedReleases(statements:Array<CStmt>, operations:Array<CBodyEmitterManagedOperation>):Void {
		var index = operations.length;
		while (index != 0) {
			index--;
			statements.push(ignoreExpression(operations[index].release));
		}
	}

	function managedValueOperations(value:CExpr, type:HxcIRTypeRef):Array<CBodyEmitterManagedOperation> {
		return switch type {
			case IRTManagedString: [
					{
						retain: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringRetain)), [value]),
						release: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringRelease)), [EUnary(AddressOf, value)])
					}
				];
			case IRTInstance(instanceId) if (bytesInstanceIds.exists(instanceId)): [
					{
						retain: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNBytesRetain)), [value]),
						release: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNBytesRelease)), [value])
					}
				];
			case IRTInstance(instanceId) if (arrayElementTypes.exists(instanceId)):
				final array = arraysByInstance.get(instanceId);
				if (array != null && array.prepared.managedByCollector) []; else [
					{
						retain: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayRetain)), [value]),
						release: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayRelease)), [value])
					}
				];
			case IRTInstance(instanceId) if (stringMapValueTypes.exists(instanceId)): [
					{
						retain: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapRetain)), [value]),
						release: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapRelease)), [value])
					}
				];
			case IRTInstance(instanceId) if (intMapInstanceIds.exists(instanceId)): [
					{
						retain: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapRetain)), [value]),
						release: ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapRelease)), [value])
					}
				];
			case IRTInstance(instanceId) if (enumRepresentations.get(instanceId) == CBECNative): [];
			case IRTInstance(instanceId) if (enumsByInstance.exists(instanceId)):
				final nested = enumsByInstance.get(instanceId);
				if (nested == null || !nested.prepared.managedLifetime) []; else [
					{
						retain: ECall(EIdentifier(requireEnumLifecycleName(nested.retainName, nested, "retain")), [EUnary(AddressOf, value)]),
						release: ECall(EIdentifier(requireEnumLifecycleName(nested.destroyName, nested, "destroy")), [EUnary(AddressOf, value)])
					}
				];
			case IRTInstance(instanceId) if (aggregateFieldOrder.exists(instanceId)):
				final result:Array<CBodyEmitterManagedOperation> = [];
				for (fieldName in requireAggregateFieldOrder(instanceId)) {
					final field = EMember(value, requireAggregateFieldName(instanceId, fieldName, "array-lifecycle", instanceId), false);
					for (operation in managedValueOperations(field, requireAggregateFieldIrType(instanceId, fieldName)))
						result.push(operation);
				}
				result;
			case IRTNullable(_, IRNTagged):
				final optional = requireOptional(type);
				if (!optional.prepared.managedLifetime) []; else [
					{
						retain: ECall(EIdentifier(requireOptionalLifecycleName(optional.retainName, optional, "retain")), [EUnary(AddressOf, value)]),
						release: ECall(EIdentifier(requireOptionalLifecycleName(optional.destroyName, optional, "destroy")), [EUnary(AddressOf, value)])
					}
				];
			case IRTInstance(instanceId) if (interfaceLayoutsByInstance.exists(instanceId)): [];
			case IRTBool | IRTInt(_,
				_) | IRTAbiInteger(_) | IRTFloat(_) | IRTString | IRTCString | IRTPointer(_, _) | IRTFixedArray(_, _, _) | IRTSpan(_, _): [];
			case _:
				throw new CBodyEmissionError('managed Array element lifecycle reached unsupported nested type `${typeKey(type)}`');
		};
	}

	function arrayElementStorageValue(array:CLoweredBodyArray, rawPointer:CExpr, readOnly:Bool):CExpr {
		final element = typedDeclarator(array.prepared.element.irType, DName(null));
		final storage = storagePointerDeclarator(element, readOnly);
		return EUnary(Dereference, ECast(storage.type, storage.declarator, rawPointer));
	}

	function stringMapValueStorage(map:CLoweredBodyStringMap, rawPointer:CExpr, readOnly:Bool):CExpr {
		final value = typedDeclarator(map.prepared.value.irType, DName(null));
		final storage = storagePointerDeclarator(value, readOnly);
		return EUnary(Dereference, ECast(storage.type, storage.declarator, rawPointer));
	}

	/**
		Build the typed pointer used to read or write one erased container slot.

		Runtime callbacks receive storage as `void *` or `const void *`, but the
		generated helper must recover the exact element type before dereferencing
		it. For a by-value element such as a record, read-only storage is
		`const Record *`: the record itself is const. For a pointer-valued element
		such as `Array<Int>`, it is `ArrayRef * const *`: the stored pointer value
		is const, not the Array object it points at. Putting `const` on the base
		struct would instead create `const ArrayRef **`, which both changes the
		pointee contract and triggers Clang's `-Wcast-qual` diagnostic.

		The extra outer `DPointer` represents the address of the container slot.
		C declarator qualifiers live on that pointer when the stored value already
		has pointer shape; direct values keep the qualifier on their base type.
	**/
	static function storagePointerDeclarator(value:CTypedDeclarator, readOnly:Bool):CTypedDeclarator {
		if (!readOnly)
			return {type: value.type, declarator: DPointer(value.declarator, [])};
		return switch value.declarator {
			case DPointer(_, _):
				{type: value.type, declarator: DPointer(value.declarator, [QConst])};
			case _:
				final qualifiers = value.type.qualifiers.copy();
				if (!qualifiers.contains(QConst))
					qualifiers.push(QConst);
				{
					type: new CType(value.type.spec, qualifiers),
					declarator: DPointer(value.declarator, [])
				};
		};
	}

	static function arrayLifecycleParameters(names:Array<CIdentifier>, destroy:Bool):Array<CParam> {
		final expected = destroy ? 2 : 3;
		if (names.length != expected)
			throw new CBodyEmissionError('Array element callback expected $expected finalized parameters but received ${names.length}');
		final result:Array<CParam> = [
			{type: new CType(TVoid), declarator: DPointer(DName(names[0]), []), attributes: []},
			{type: new CType(TVoid), declarator: DPointer(DName(names[1]), []), attributes: []}
		];
		if (!destroy)
			result.push({type: new CType(TVoid, [QConst]), declarator: DPointer(DName(names[2]), []), attributes: []});
		return result;
	}

	static function statusDeclaration(name:CIdentifier):CStmt
		return SDecl({
			storage: [],
			alignments: [],
			type: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStatusType))),
			declarator: DName(name),
			initializer: null,
			attributes: []
		});

	static function ignoreExpression(value:CExpr):CStmt
		return SExpr(ECast(new CType(TVoid), DName(null), value));

	function canonicalManagedArrays():Array<CLoweredBodyArray> {
		final result = [for (value in arraysByInstance) if (value.hasLifecycle()) value];
		result.sort((left, right) -> compareUtf8(left.prepared.digest, right.prepared.digest));
		return result;
	}

	function canonicalManagedStringMaps():Array<CLoweredBodyStringMap> {
		final result = [for (value in stringMapsByInstance) if (value.hasLifecycle()) value];
		result.sort((left, right) -> compareUtf8(left.prepared.digest, right.prepared.digest));
		return result;
	}

	function canonicalManagedAggregates():Array<CLoweredBodyAggregate> {
		final result:Array<CLoweredBodyAggregate> = [];
		for (instanceId in aggregateInstanceOrder) {
			final value = aggregatesByInstance.get(instanceId);
			if (value != null && value.prepared.managedLifetime)
				result.push(value);
		}
		return result;
	}

	function canonicalManagedEnums():Array<CLoweredBodyEnum> {
		final result = [for (value in enumsByInstance) if (value.prepared.managedLifetime) value];
		result.sort((left, right) -> compareUtf8(left.prepared.digest, right.prepared.digest));
		return result;
	}

	function canonicalManagedOptionals():Array<CLoweredBodyOptional> {
		final result:Array<CLoweredBodyOptional> = [];
		for (planId in optionalPlanOrder) {
			final value = optionalsByPlan.get(planId);
			if (value != null && value.prepared.managedLifetime)
				result.push(value);
		}
		return result;
	}

	static function requireEnumLifecycleName(name:Null<CIdentifier>, value:CLoweredBodyEnum, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed enum `${value.prepared.instanceId}` lost its $operation helper name');
		return name;
	}

	static function requireAggregateLifecycleName(name:Null<CIdentifier>, value:CLoweredBodyAggregate, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed aggregate `${value.prepared.instanceId}` lost its $operation helper name');
		return name;
	}

	static function requireAggregateLifecycleParameter(name:Null<CIdentifier>, value:CLoweredBodyAggregate, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed aggregate `${value.prepared.instanceId}` lost its $operation parameter name');
		return name;
	}

	static function requireAggregateLifecycleStatus(name:Null<CIdentifier>, value:CLoweredBodyAggregate):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed aggregate `${value.prepared.instanceId}` lost its retain status local');
		return name;
	}

	static function requireOptionalLifecycleName(name:Null<CIdentifier>, value:CLoweredBodyOptional, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its $operation helper name');
		return name;
	}

	static function requireOptionalLifecycleParameter(name:Null<CIdentifier>, value:CLoweredBodyOptional, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its $operation parameter name');
		return name;
	}

	static function requireOptionalLifecycleStatus(name:Null<CIdentifier>, value:CLoweredBodyOptional):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed optional `${value.prepared.planId}` lost its retain status local');
		return name;
	}

	static function requireEnumLifecycleParameter(name:Null<CIdentifier>, value:CLoweredBodyEnum, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed enum `${value.prepared.instanceId}` lost its $operation parameter name');
		return name;
	}

	static function requireEnumLifecycleStatus(name:Null<CIdentifier>, value:CLoweredBodyEnum):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed enum `${value.prepared.instanceId}` lost its retain status local');
		return name;
	}

	function requireEnumArrayLifecycle(implementationId:String, actionId:String, functionId:String):CBodyEmitterEnumArrayLifecycle {
		final value = enumArrayLifecycles.get(implementationId);
		if (value == null)
			throw new CBodyEmissionError('program-local enum lifecycle `$implementationId` for `$actionId` in `$functionId` is not finalized');
		return value;
	}

	function programLocalLifecycle(implementationId:String, actionId:String, functionId:String):CBodyEmitterProgramLocalLifecycle {
		final aggregate = aggregateLifecycles.get(implementationId);
		if (aggregate != null)
			return aggregate;
		final optional = optionalLifecycles.get(implementationId);
		if (optional != null)
			return {
				instanceId: typeKey(optional.type),
				retainName: optional.retainName,
				destroyName: optional.destroyName
			};
		return requireEnumArrayLifecycle(implementationId, actionId, functionId);
	}

	static function requireArrayCallbackName(name:Null<CIdentifier>, array:CLoweredBodyArray, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed Array `${array.prepared.semanticKey}` lost its $operation callback name');
		return name;
	}

	static function requireArrayCallbackParameters(names:Array<CIdentifier>, expected:Int, array:CLoweredBodyArray, operation:String):Array<CIdentifier> {
		if (names.length != expected)
			throw new CBodyEmissionError('managed Array `${array.prepared.semanticKey}` lost its $operation callback parameters');
		return names;
	}

	static function requireArrayStatusName(name:Null<CIdentifier>, array:CLoweredBodyArray, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed Array `${array.prepared.semanticKey}` lost its $operation callback status local');
		return name;
	}

	static function requireStringMapCallbackName(name:Null<CIdentifier>, map:CLoweredBodyStringMap, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed StringMap `${map.prepared.semanticKey}` lost its $operation callback name');
		return name;
	}

	static function requireStringMapCallbackParameters(names:Array<CIdentifier>, expected:Int, map:CLoweredBodyStringMap, operation:String):Array<CIdentifier> {
		if (names.length != expected)
			throw new CBodyEmissionError('managed StringMap `${map.prepared.semanticKey}` lost its $operation callback parameters');
		return names;
	}

	static function requireStringMapStatusName(name:Null<CIdentifier>, map:CLoweredBodyStringMap, operation:String):CIdentifier {
		if (name == null)
			throw new CBodyEmissionError('managed StringMap `${map.prepared.semanticKey}` lost its $operation callback status local');
		return name;
	}

	public function virtualTableObjectDeclarations():Array<CDecl> {
		final result:Array<CDecl> = [];
		final ids = [for (id in virtualTables.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids) {
			final table = requireVirtualTable(id);
			result.push(DVariable({
				storage: [SExtern],
				alignments: [],
				type: new CType(TStruct(table.layout.cTag), [QConst]),
				declarator: DName(table.cName),
				initializer: null,
				attributes: []
			}));
		}
		return result;
	}

	public function virtualTableObjects(functionNames:Map<String, CIdentifier>):Array<CDecl> {
		final result:Array<CDecl> = [];
		final ids = [for (id in virtualTables.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids) {
			final table = requireVirtualTable(id);
			final initializers:Array<CInitializerItem> = [];
			for (entry in table.entries) {
				final implementation:CExpr = if (entry.implementationFunctionId == null) {
					EInt(CIntegerLiteral.decimal("0"));
				} else if (entry.thunk != null) {
					EIdentifier(entry.thunk.cName);
				} else {
					EIdentifier(requireFunctionName(functionNames, entry.implementationFunctionId, 'virtual table `$id`'));
				};
				initializers.push({designators: [DField(entry.slot.cMember)], value: IExpr(implementation)});
			}
			result.push(DVariable({
				storage: [],
				alignments: [],
				type: new CType(TStruct(table.layout.cTag), [QConst]),
				declarator: DName(table.cName),
				initializer: IList(initializers),
				attributes: []
			}));
		}
		return result;
	}

	public function virtualThunkDefinitions(functionNames:Map<String, CIdentifier>):Array<CDecl> {
		final result:Array<CDecl> = [];
		for (thunk in virtualThunks) {
			final declaration = virtualThunkDeclarator(thunk, DFunction(DName(thunk.cName), FPPrototype(virtualThunkParameters(thunk), false)));
			final implementationReceiver = typedDeclarator(IRTPointer(IRTInstance(thunk.implementationOwnerInstanceId), true), DName(null));
			final arguments:Array<CExpr> = [
				ECast(implementationReceiver.type, implementationReceiver.declarator, EIdentifier(thunk.receiverName))
			];
			for (name in thunk.argumentNames)
				arguments.push(EIdentifier(name));
			final call = ECall(EIdentifier(requireFunctionName(functionNames, thunk.implementationFunctionId, thunk.id)), arguments);
			final statements:Array<CStmt> = switch thunk.slot.returnType {
				case IRTVoid: [SExpr(call), SReturn(null)];
				case _: [SReturn(call)];
			};
			result.push(DFunction({
				storage: [SStatic],
				functionSpecifiers: [],
				returnType: declaration.type,
				declarator: declaration.declarator,
				body: SBlock(statements),
				attributes: []
			}));
		}
		return result;
	}

	function virtualThunkParameters(thunk:CBodyEmitterVirtualThunk):Array<CParam> {
		final result:Array<CParam> = [];
		final receiver = dispatchReceiverDeclarator(requireLayoutForSlot(thunk.slot), thunk.slot, DName(thunk.receiverName));
		result.push({type: receiver.type, declarator: receiver.declarator, attributes: []});
		for (index in 0...thunk.slot.parameterTypes.length) {
			final parameter = typedDeclarator(thunk.slot.parameterTypes[index], DName(thunk.argumentNames[index]));
			result.push({type: parameter.type, declarator: parameter.declarator, attributes: []});
		}
		return result;
	}

	function virtualThunkDeclarator(thunk:CBodyEmitterVirtualThunk, inner:CDeclarator):CTypedDeclarator
		return typedDeclarator(thunk.slot.returnType, inner);

	function dispatchReceiverDeclarator(layout:CBodyEmitterVirtualLayout, slot:CBodyEmitterVirtualSlot, inner:CDeclarator):CTypedDeclarator
		return layout.cValueTag == null ? typedDeclarator(IRTPointer(IRTInstance(slot.ownerInstanceId), true), inner) : {
			type: new CType(TVoid),
			declarator: DPointer(inner, [])
		};

	public function classDefinitions():Array<CDecl> {
		final result = classForwardDeclarations();
		for (instanceId in classInstanceOrder)
			result.push(classDefinition(instanceId));
		return result;
	}

	/** Stable semantic order used when project layout assigns class definitions. */
	public function orderedClassInstanceIds():Array<String>
		return classInstanceOrder.copy();

	/** Forward declarations are dependency-neutral and may live in a common header. */
	public function classForwardDeclarations():Array<CDecl>
		return [
			for (instanceId in classInstanceOrder)
				DForwardStruct(requireClassTag(instanceId), [])
		];

	/** One complete class definition without its dependency-neutral forward. */
	public function classDefinition(instanceId:String):CDecl {
		final fields:Array<CField> = [];
		final dispatchLayoutId = classDispatchLayoutIds.get(instanceId);
		if (dispatchLayoutId != null) {
			final layout = requireVirtualLayout(dispatchLayoutId);
			fields.push({
				type: new CType(TStruct(layout.cTag), [QConst]),
				declarator: DPointer(DName(requireClassDispatchHeader(instanceId)), []),
				bitWidth: null,
				alignments: [],
				attributes: []
			});
		}
		final baseInstance = classBaseInstances.get(instanceId);
		if (baseInstance != null) {
			fields.push({
				type: cType(IRTInstance(baseInstance)),
				declarator: DName(requireClassBaseMember(instanceId)),
				bitWidth: null,
				alignments: [],
				attributes: []
			});
		}
		final order = requireClassFieldOrder(instanceId);
		for (fieldName in order) {
			final type = requireClassFieldType(instanceId, fieldName);
			final name = requireClassFieldName(instanceId, fieldName);
			final declaration = typedDeclarator(type, DName(name));
			fields.push({
				type: declaration.type,
				declarator: declaration.declarator,
				bitWidth: null,
				alignments: [],
				attributes: []
			});
		}
		final anchor = classEmptyAnchors.get(instanceId);
		if (anchor != null) {
			fields.push({
				type: new CType(TChar(false)),
				declarator: DName(anchor),
				bitWidth: null,
				alignments: [],
				attributes: []
			});
		}
		if (fields.length == 0)
			fail('class instance `$instanceId` would emit an invalid empty strict-C11 struct');
		return DStruct(requireClassTag(instanceId), fields, []);
	}

	public function enumDefinitions():Array<CDecl> {
		final result = enumForwardDeclarations();
		for (instanceId in enumInstanceOrder)
			for (declaration in enumDefinitionsFor(instanceId))
				result.push(declaration);
		return result;
	}

	/** Stable semantic order used when project layout assigns enum definitions. */
	public function orderedEnumInstanceIds():Array<String>
		return enumInstanceOrder.copy();

	/** Tagged-value forwards are dependency-neutral; native enums stay complete. */
	public function enumForwardDeclarations():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in enumInstanceOrder)
			if (requireEnumRepresentation(instanceId) == CBECTagged)
				result.push(DForwardStruct(requireEnumValueTag(instanceId), []));
		return result;
	}

	/**
		Whether a generated instance has a strict-C11 forward declaration.

		Native enums deliberately return false: ISO C11 cannot forward-declare them,
		so a header declaration that names one still needs its defining header.
	**/
	public function typeInstanceIsForwardDeclarable(instanceId:String):Bool {
		if (aggregateTags.exists(instanceId) || classTags.exists(instanceId))
			return true;
		final representation = enumRepresentations.get(instanceId);
		return representation != null && representation == CBECTagged;
	}

	/** Complete declarations for one enum instance, excluding its common forward. */
	public function enumDefinitionsFor(instanceId:String):Array<CDecl> {
		final result:Array<CDecl> = [];
		final enumerators = requireEnumCaseOrder(instanceId).map(caseName -> {
			name: requireEnumCaseDiscriminant(instanceId, caseName),
			value: EInt(CIntegerLiteral.decimal(Std.string(requireEnumCaseValue(instanceId, caseName)))),
			attributes: []
		});
		switch requireEnumRepresentation(instanceId) {
			case CBECNative:
				result.push(DEnum(requireEnumValueTag(instanceId), enumerators, []));
			case CBECTagged:
				result.push(DEnum(requireEnumDiscriminantTag(instanceId), enumerators, []));
				for (caseName in requireEnumCaseOrder(instanceId)) {
					final payloadNames = requireEnumPayloadNames(instanceId, caseName);
					if (payloadNames.length == 0)
						continue;
					final fields:Array<CField> = [];
					for (payloadName in payloadNames) {
						final declaration = typedDeclarator(requireEnumPayloadFieldType(instanceId, caseName, payloadName),
							DName(requireEnumPayloadFieldName(instanceId, caseName, payloadName)));
						fields.push({
							type: declaration.type,
							declarator: declaration.declarator,
							bitWidth: null,
							alignments: [],
							attributes: []
						});
					}
					result.push(DStruct(requireEnumCasePayloadStructTag(instanceId, caseName), fields, []));
				}
				final unionFields:Array<CField> = [];
				for (caseName in requireEnumCaseOrder(instanceId)) {
					if (requireEnumPayloadNames(instanceId, caseName).length == 0)
						continue;
					unionFields.push({
						type: new CType(TStruct(requireEnumCasePayloadStructTag(instanceId, caseName))),
						declarator: DName(requireEnumCaseUnionMember(instanceId, caseName)),
						bitWidth: null,
						alignments: [],
						attributes: []
					});
				}
				if (unionFields.length == 0)
					fail('tagged enum `$instanceId` has no payload union member');
				result.push(DUnion(requireEnumPayloadUnionTag(instanceId), unionFields, []));
				result.push(DStruct(requireEnumValueTag(instanceId), [
					{
						type: new CType(TEnum(requireEnumDiscriminantTag(instanceId))),
						declarator: DName(requireEnumTagMember(instanceId)),
						bitWidth: null,
						alignments: [],
						attributes: []
					},
					{
						type: new CType(TUnion(requireEnumPayloadUnionTag(instanceId))),
						declarator: DName(requireEnumPayloadMember(instanceId)),
						bitWidth: null,
						alignments: [],
						attributes: []
					}
				], []));
		}

		return result;
	}

	public function aggregateLayoutAssertions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in aggregateInstanceOrder) {
			final tag = requireAggregateTag(instanceId);
			final structType = new CType(TStruct(tag));
			final order = requireAggregateFieldOrder(instanceId);
			for (index in 0...order.length) {
				final fieldName = order[index];
				final member = requireAggregateFieldName(instanceId, fieldName, "layout", instanceId);
				final field = typedDeclarator(requireAggregateFieldIrType(instanceId, fieldName), DName(null));
				final offset = EOffsetOf(structType, DName(null), member);
				if (index == 0) {
					result.push(DStaticAssert(EBinary(Equal, offset, EInt(CIntegerLiteral.decimal("0"))),
						'closed record ${tag.value} first field begins at offset zero'));
				} else {
					final previousName = order[index - 1];
					final previousMember = requireAggregateFieldName(instanceId, previousName, "layout", instanceId);
					final previous = typedDeclarator(requireAggregateFieldIrType(instanceId, previousName), DName(null));
					result.push(DStaticAssert(EBinary(GreaterEqual, offset,
						EBinary(Add, EOffsetOf(structType, DName(null), previousMember), ESizeOfType(previous.type, previous.declarator))),
						'closed record ${tag.value} field $index follows the prior field without overlap'));
				}
				result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(structType, DName(null)), EAlignOfType(field.type, field.declarator)),
					'closed record ${tag.value} alignment admits field $index'));
			}
			final lastIndex = order.length - 1;
			final lastName = order[lastIndex];
			final lastMember = requireAggregateFieldName(instanceId, lastName, "layout", instanceId);
			final last = typedDeclarator(requireAggregateFieldIrType(instanceId, lastName), DName(null));
			result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)),
				EBinary(Add, EOffsetOf(structType, DName(null), lastMember), ESizeOfType(last.type, last.declarator))),
				'closed record ${tag.value} size contains its final field'));
		}
		return result;
	}

	public function classLayoutAssertions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in classInstanceOrder) {
			final tag = requireClassTag(instanceId);
			final structType = new CType(TStruct(tag));
			final baseInstance = classBaseInstances.get(instanceId);
			var previousMember:Null<CIdentifier> = null;
			var previousDeclaration:Null<CTypedDeclarator> = null;
			final dispatchLayoutId = classDispatchLayoutIds.get(instanceId);
			if (dispatchLayoutId != null) {
				final header = requireClassDispatchHeader(instanceId);
				final layout = requireVirtualLayout(dispatchLayoutId);
				final headerDeclaration:CTypedDeclarator = {
					type: new CType(TStruct(layout.cTag), [QConst]),
					declarator: DPointer(DName(null), [])
				};
				result.push(DStaticAssert(EBinary(Equal, EOffsetOf(structType, DName(null), header), EInt(CIntegerLiteral.decimal("0"))),
					'class ${tag.value} virtual-table pointer begins at offset zero'));
				result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(structType, DName(null)),
					EAlignOfType(headerDeclaration.type, headerDeclaration.declarator)),
					'class ${tag.value} alignment admits its virtual-table pointer'));
				previousMember = header;
				previousDeclaration = headerDeclaration;
			}
			if (baseInstance != null) {
				final member = requireClassBaseMember(instanceId);
				final baseType = cType(IRTInstance(baseInstance));
				result.push(DStaticAssert(EBinary(Equal, EOffsetOf(structType, DName(null), member), EInt(CIntegerLiteral.decimal("0"))),
					'class ${tag.value} base subobject begins at offset zero'));
				result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(structType, DName(null)), EAlignOfType(baseType, DName(null))),
					'class ${tag.value} alignment admits its base subobject'));
				result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)), ESizeOfType(baseType, DName(null))),
					'class ${tag.value} contains its complete base subobject'));
				previousMember = member;
				previousDeclaration = {type: baseType, declarator: DName(null)};
			}
			final order = requireClassFieldOrder(instanceId);
			for (index in 0...order.length) {
				final fieldName = order[index];
				final member = requireClassFieldName(instanceId, fieldName);
				final fieldType = requireClassFieldType(instanceId, fieldName);
				final typed = typedDeclarator(fieldType, DName(null));
				final offset = EOffsetOf(structType, DName(null), member);
				if (previousMember == null) {
					result.push(DStaticAssert(EBinary(Equal, offset, EInt(CIntegerLiteral.decimal("0"))),
						'class ${tag.value} first storage field begins at offset zero'));
				} else {
					final prior = requireClassPriorDeclaration(previousDeclaration, tag);
					result.push(DStaticAssert(EBinary(GreaterEqual, offset,
						EBinary(Add, EOffsetOf(structType, DName(null), previousMember), ESizeOfType(prior.type, prior.declarator))),
						'class ${tag.value} field $index follows the prior storage without overlap'));
				}
				result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(structType, DName(null)), EAlignOfType(typed.type, typed.declarator)),
					'class ${tag.value} alignment admits field $index'));
				previousMember = member;
				previousDeclaration = typed;
			}
			final anchor = classEmptyAnchors.get(instanceId);
			if (anchor != null) {
				result.push(DStaticAssert(EBinary(Equal, EOffsetOf(structType, DName(null), anchor), EInt(CIntegerLiteral.decimal("0"))),
					'class ${tag.value} strict-C empty-storage anchor begins at zero'));
				result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)), EInt(CIntegerLiteral.decimal("1"))),
					'class ${tag.value} strict-C empty-storage anchor occupies one byte'));
			} else if (previousMember != null && previousDeclaration != null) {
				final last = previousDeclaration;
				result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)),
					EBinary(Add, EOffsetOf(structType, DName(null), previousMember), ESizeOfType(last.type, last.declarator))),
					'class ${tag.value} size contains its final storage member'));
			}
		}
		return result;
	}

	public function enumLayoutAssertions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in enumInstanceOrder) {
			for (caseName in requireEnumCaseOrder(instanceId)) {
				result.push(DStaticAssert(EBinary(Equal, EIdentifier(requireEnumCaseDiscriminant(instanceId, caseName)),
					EInt(CIntegerLiteral.decimal(Std.string(requireEnumCaseValue(instanceId, caseName))))),
					'enum ${requireEnumValueTag(instanceId).value} case $caseName retains its Haxe discriminant'));
			}
			if (requireEnumRepresentation(instanceId) == CBECNative)
				continue;
			final structType = new CType(TStruct(requireEnumValueTag(instanceId)));
			final tagType = new CType(TEnum(requireEnumDiscriminantTag(instanceId)));
			final unionType = new CType(TUnion(requireEnumPayloadUnionTag(instanceId)));
			final tagMember = requireEnumTagMember(instanceId);
			final payloadMember = requireEnumPayloadMember(instanceId);
			result.push(DStaticAssert(EBinary(Equal, EOffsetOf(structType, DName(null), tagMember), EInt(CIntegerLiteral.decimal("0"))),
				'tagged enum ${requireEnumValueTag(instanceId).value} begins with its discriminant'));
			result.push(DStaticAssert(EBinary(GreaterEqual, EOffsetOf(structType, DName(null), payloadMember), ESizeOfType(tagType, DName(null))),
				'tagged enum ${requireEnumValueTag(instanceId).value} payload follows its discriminant'));
			result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)),
				EBinary(Add, EOffsetOf(structType, DName(null), payloadMember), ESizeOfType(unionType, DName(null)))),
				'tagged enum ${requireEnumValueTag(instanceId).value} contains its payload union'));
			for (caseName in requireEnumCaseOrder(instanceId)) {
				final payloadNames = requireEnumPayloadNames(instanceId, caseName);
				if (payloadNames.length == 0)
					continue;
				final caseStructType = new CType(TStruct(requireEnumCasePayloadStructTag(instanceId, caseName)));
				final unionMember = requireEnumCaseUnionMember(instanceId, caseName);
				result.push(DStaticAssert(EBinary(Equal, EOffsetOf(unionType, DName(null), unionMember), EInt(CIntegerLiteral.decimal("0"))),
					'tagged enum ${requireEnumValueTag(instanceId).value} case $caseName begins at union offset zero'));
				for (index in 0...payloadNames.length) {
					final payloadName = payloadNames[index];
					final fieldName = requireEnumPayloadFieldName(instanceId, caseName, payloadName);
					final fieldType = requireEnumPayloadFieldType(instanceId, caseName, payloadName);
					final typed = typedDeclarator(fieldType, DName(null));
					final offset = EOffsetOf(caseStructType, DName(null), fieldName);
					if (index == 0) {
						result.push(DStaticAssert(EBinary(Equal, offset, EInt(CIntegerLiteral.decimal("0"))),
							'tagged enum ${requireEnumValueTag(instanceId).value} case $caseName first payload begins at zero'));
					} else {
						final previousName = payloadNames[index - 1];
						final previousField = requireEnumPayloadFieldName(instanceId, caseName, previousName);
						final previousType = typedDeclarator(requireEnumPayloadFieldType(instanceId, caseName, previousName), DName(null));
						result.push(DStaticAssert(EBinary(GreaterEqual, offset,
							EBinary(Add, EOffsetOf(caseStructType, DName(null), previousField), ESizeOfType(previousType.type, previousType.declarator))),
							'tagged enum ${requireEnumValueTag(instanceId).value} case $caseName payload $index follows its predecessor'));
					}
					result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(caseStructType, DName(null)), EAlignOfType(typed.type, typed.declarator)),
						'tagged enum ${requireEnumValueTag(instanceId).value} case $caseName admits payload $index alignment'));
				}
			}
		}
		return result;
	}

	function addTypeHeaders(headers:Array<String>, type:HxcIRTypeRef, ?visitedInstances:Map<String, Bool>):Void {
		final visited:Map<String, Bool> = visitedInstances == null ? [] : visitedInstances;
		switch type {
			case IRTBool:
				addUnique(headers, "stdbool.h");
			case IRTInt(_, _):
				addUnique(headers, "stdint.h");
			case IRTAbiInteger(IRAKSize):
				addUnique(headers, "stddef.h");
			case IRTAbiInteger(IRAKPtrDiff):
				addUnique(headers, "stddef.h");
			case IRTAbiInteger(IRAKIntPtr) | IRTAbiInteger(IRAKUIntPtr):
				addUnique(headers, "stdint.h");
			case IRTString:
				addUnique(headers, "hxrt/string_literal.h");
			case IRTManagedString:
				addUnique(headers, "hxrt/string.h");
			case IRTCString:
			case IRTInstance(instanceId):
				if (arrayElementTypes.exists(instanceId)) {
					addUnique(headers, "hxrt/array.h");
					addTypeHeaders(headers, requireArrayElementType(instanceId), visited);
					return;
				}
				if (intMapInstanceIds.exists(instanceId)) {
					addUnique(headers, "hxrt/int_map.h");
					return;
				}
				final stringMapValueType = stringMapValueTypes.get(instanceId);
				if (stringMapValueType != null) {
					addUnique(headers, "hxrt/string_map.h");
					addTypeHeaders(headers, stringMapValueType, visited);
					return;
				}
				if (bytesInstanceIds.exists(instanceId)) {
					addUnique(headers, "hxrt/bytes.h");
					return;
				}
				if (imports.typeByInstance(instanceId) != null)
					return;
				if (visited.exists(instanceId))
					return;
				visited.set(instanceId, true);
				final order = aggregateFieldOrder.get(instanceId);
				if (order != null) {
					for (fieldName in order) {
						final fieldType = aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
						if (fieldType == null) {
							throw new CBodyEmissionError('direct aggregate instance `$instanceId` lost field type `$fieldName`');
						}
						addTypeHeaders(headers, fieldType, visited);
					}
				} else if (enumRepresentations.exists(instanceId)) {
					for (caseName in requireEnumCaseOrder(instanceId)) {
						for (payloadName in requireEnumPayloadNames(instanceId, caseName)) {
							addTypeHeaders(headers, requireEnumPayloadFieldType(instanceId, caseName, payloadName), visited);
						}
					}
				} else if (classTags.exists(instanceId)) {
					final base = classBaseInstances.get(instanceId);
					if (base != null)
						addTypeHeaders(headers, IRTInstance(base), visited);
					final fields = classFieldOrder.get(instanceId);
					if (fields == null)
						throw new CBodyEmissionError('class instance `$instanceId` lost its finalized storage order');
					for (fieldName in fields) {
						final fieldType = classFieldTypes.get(classFieldKey(instanceId, fieldName));
						if (fieldType == null)
							throw new CBodyEmissionError('class instance `$instanceId` lost field type `$fieldName`');
						addTypeHeaders(headers, fieldType, visited);
					}
				} else if (interfaceLayoutsByInstance.exists(instanceId)) {
					// The private fat value contains only `void *` and an interface-table pointer.
				} else {
					throw new CBodyEmissionError('direct instance `$instanceId` has no finalized C layout');
				}
			case IRTPointer(pointee, _):
				addUnique(headers, "stddef.h");
				addTypeHeaders(headers, pointee, visited);
			case IRTFixedArray(element, _, _):
				addTypeHeaders(headers, element, visited);
			case IRTSpan(element, _):
				addTypeHeaders(headers, element, visited);
				addUnique(headers, "stddef.h");
			case IRTNullable(payload, IRNTagged):
				addUnique(headers, "stdbool.h");
				addTypeHeaders(headers, payload, visited);
			case IRTFunction(parameters, result):
				for (parameter in parameters)
					addTypeHeaders(headers, parameter, visited);
				addTypeHeaders(headers, result, visited);
			case IRTVoid | IRTFloat(32) | IRTFloat(64):
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` has no admitted strict-C direct-value header mapping');
		}
	}

	function constantExpression(value:HxcIRConstant):CExpr {
		return switch value {
			case IRCInt(text): integerExpression(text);
			case IRCFloat(text): floatExpression(text);
			case IRCBool(value): EBool(value);
			case IRCString(text, byteLength): stringLiteralExpression(text, byteLength);
			case IRCCStringLiteral(text, byteLength):
				if (byteLength < 0 || text.indexOf("\x00") != -1)
					fail("validated C string literal lost its length or NUL-free invariant");
				EString(text);
			case IRCNativeConstant(constantId):
				final constant = imports.constantById(constantId);
				constant == null ? fail('native constant `$constantId` has no finalized import') : EIdentifier(constant.cName);
			case IRCNull: ENull;
		};
	}

	/**
	 * Render a literal with the representation selected for its result type.
	 *
	 * Pointer-like values can use C's `NULL` token directly. A nullable record is
	 * a struct, however, so its absent value is `{ .has_value = false }`; C then
	 * zero-initializes the unused payload without reading or inventing a value.
	 */
	function constantExpressionForType(value:HxcIRConstant, type:HxcIRTypeRef):CExpr {
		return switch [value, type] {
			case [IRCNull, IRTNullable(_, IRNTagged)]: directOptionalNullExpression(type);
			case [IRCNull, IRTString]: stringNullExpression();
			case [IRCNull, IRTManagedString]: stringNullExpression();
			case _: constantExpression(value);
		};
	}

	/** Build the struct-valued C carrier for Haxe's nullable String reference. */
	static function stringNullExpression():CExpr
		return ECompoundLiteral(new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringType))), DName(null), IList([
			{designators: [DField(new CIdentifier("data"))], value: IExpr(ENull)},
			{designators: [DField(new CIdentifier("byte_length"))], value: IExpr(EInt(CIntegerLiteral.decimal("0", ISUnsigned)))},
			{designators: [DField(new CIdentifier("has_trailing_nul"))], value: IExpr(EBool(false))},
			{designators: [DField(new CIdentifier("owner"))], value: IExpr(ENull)}
		]));

	function directOptionalNullExpression(type:HxcIRTypeRef):CExpr {
		final optional = requireOptional(type);
		return ECompoundLiteral(cType(type), DName(null), IList([{designators: [DField(optional.presenceName)], value: IExpr(EBool(false))}]));
	}

	function directOptionalValueExpression(type:HxcIRTypeRef, value:CExpr):CExpr {
		final optional = requireOptional(type);
		return ECompoundLiteral(cType(type), DName(null), IList([
			{designators: [DField(optional.presenceName)], value: IExpr(EBool(true))},
			{designators: [DField(optional.payloadName)], value: IExpr(value)}
		]));
	}

	static function stringLiteralExpression(text:String, byteLength:Int):CExpr {
		if (byteLength < 0) {
			return fail('validated UTF-8 literal has negative byte length `$byteLength`');
		}
		return ECompoundLiteral(new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringType))), DName(null), IList([
			{
				designators: [],
				value: IExpr(ECast(new CType(TInt(8, false), [QConst]), DPointer(DName(null), []), EString(text)))
			},
			{designators: [], value: IExpr(EInt(CIntegerLiteral.decimal(Std.string(byteLength))))},
			{designators: [], value: IExpr(EBool(true))},
			{designators: [], value: IExpr(ENull)}
		]));
	}

	static function integerExpression(text:String):CExpr {
		final negative = StringTools.startsWith(text, "-");
		final magnitude = negative ? text.substr(1) : text;
		final literal:CExpr = EInt(CIntegerLiteral.decimal(magnitude));
		return negative ? EUnary(Minus, literal) : literal;
	}

	static function floatExpression(text:String):CExpr {
		final negative = StringTools.startsWith(text, "-");
		var unsigned = negative ? text.substr(1) : text;
		var exponent:Null<Int> = null;
		var exponentIndex = unsigned.indexOf("e");
		if (exponentIndex == -1) {
			exponentIndex = unsigned.indexOf("E");
		}
		if (exponentIndex != -1) {
			final exponentText = unsigned.substr(exponentIndex + 1);
			exponent = Std.parseInt(exponentText);
			if (exponent == null) {
				fail('floating constant `$text` has an exponent outside the C AST integer range');
			}
			unsigned = unsigned.substr(0, exponentIndex);
		}
		final dot = unsigned.indexOf(".");
		final whole = dot == -1 ? unsigned : unsigned.substr(0, dot);
		var fraction = dot == -1 ? "" : unsigned.substr(dot + 1);
		if (fraction == "" && exponent == null) {
			fraction = "0";
		}
		final literal:CExpr = EFloat(new CFloatLiteral(FRDecimal(whole, fraction, exponent)));
		return negative ? EUnary(Minus, literal) : literal;
	}

	function emitCall(statements:Array<CStmt>, values:Map<String, CExpr>, spanValueLengths:Map<String, CExpr>, referencedValues:Map<String, Bool>,
			instruction:HxcIRInstruction, call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>,
			localNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>, spanLengthNames:Map<String, CIdentifier>, lineDirectives:Bool,
			nonReturningFunctionIds:Null<Map<String, Bool>>, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Bool {
		final functionId = fn.id;
		var doesNotReturn = false;
		final callExpression:CExpr = switch call.dispatch {
			case IRCDDirect(targetId):
				doesNotReturn = nonReturningFunctionIds != null && nonReturningFunctionIds.exists(targetId);
				final targetName = requireFunctionName(functionNames, targetId, functionId);
				final cArguments:Array<CExpr> = [];
				for (argument in call.arguments) {
					cArguments.push(requireValue(values, argument, functionId));
					switch valueType(fn, argument) {
						case IRTSpan(_, _): cArguments.push(requireSpanValueLength(spanValueLengths, argument, functionId));
						case null: return fail('direct call `${instruction.id}` in `$functionId` cannot resolve argument `$argument`');
						case _:
					}
				}
				ECall(EIdentifier(targetName), cArguments);
			case IRCDVirtual(slotId, receiverValueId):
				for (argument in call.arguments) {
					switch valueType(fn, argument) {
						case IRTSpan(_, _): return fail('virtual call `${instruction.id}` in `$functionId` cannot carry a borrowed span');
						case _:
					}
				}
				virtualCallExpression(slotId, receiverValueId, call.arguments, values, fn, instruction.id);
			case IRCDInterface(interfaceTypeId, slotId, receiverValueId):
				for (argument in call.arguments)
					switch valueType(fn, argument) {
						case IRTSpan(_, _): return fail('interface call `${instruction.id}` in `$functionId` cannot carry a borrowed span');
						case _:
					}
				interfaceCallExpression(interfaceTypeId, slotId, receiverValueId, call.arguments, values, fn, instruction.id);
			case IRCDClosure(callableValueId):
				for (argument in call.arguments)
					switch valueType(fn, argument) {
						case IRTSpan(_, _): return fail('function-value call `${instruction.id}` in `$functionId` cannot carry a borrowed span');
						case _:
					}
				ECall(requireValue(values, callableValueId, functionId), call.arguments.map(argument -> requireValue(values, argument, functionId)));
			case IRCDNative(importId):
				final imported = imports.functionById(importId);
				if (imported == null)
					return fail('native call `${instruction.id}` in `$functionId` has no finalized import `$importId`');
				if (call.failure != null)
					return fail('direct imported call `${instruction.id}` in `$functionId` unexpectedly carries a failure edge');
				if (call.arguments.length != imported.prepared.parameters.length
					|| typeKey(call.returnType) != typeKey(imported.prepared.returnType.irType))
					return fail('native call `${instruction.id}` in `$functionId` does not match `$importId`');
				for (index in 0...call.arguments.length) {
					final actual = valueType(fn, call.arguments[index]);
					if (actual != null) {
						switch actual {
							case IRTSpan(_, _): return fail('native call `${instruction.id}` in `$functionId` cannot carry a borrowed span');
							case _:
						}
					}
					if (actual == null || typeKey(actual) != typeKey(imported.prepared.parameters[index].irType))
						return fail('native call `${instruction.id}` in `$functionId` argument $index does not match `$importId`');
				}
				ECall(EIdentifier(imported.cName), call.arguments.map(argument -> requireValue(values, argument, functionId)));
			case dispatch if (isHostedOutputDispatch(dispatch)):
				emitHostedPrintln(statements, values, instruction, call, lineDirectives, functionId);
				return false;
			case IRCDRuntime("array", _):
				emitManagedArrayCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("int-map", _):
				emitIntMapCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("string-map", _):
				emitStringMapCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("bytes", _):
				emitManagedBytesCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("string", _):
				emitManagedStringCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("string-scalar", "char-at"):
				if (call.failure != null
					|| call.arguments.length != 2
					|| (call.returnType != IRTString && call.returnType != IRTManagedString))
					return fail('String.charAt call `${instruction.id}` in `$functionId` lost its total String/Int signature');
				ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringCharAt)), call.arguments.map(argument -> requireValue(values, argument, functionId)));
			case IRCDRuntime("string-scalar", "length"):
				emitStringLengthCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case IRCDRuntime("string-scalar", "char-code-at"):
				emitStringCharCodeAtCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, fn);
				return false;
			case IRCDRuntime("string-scalar", "substring"):
				emitStringSubstringCall(statements, values, referencedValues, instruction, call, temporaryNames, lineDirectives, boundsAbortName, fn);
				return false;
			case _: return fail('call `${instruction.id}` in `$functionId` has no admitted static or runtime dispatch');
		};
		addLineDirective(statements, instruction.source, lineDirectives);
		if (call.failure != null) {
			final failure = call.failure;
			if (failure.kind != IRFException || call.returnType != IRTVoid || instruction.result != null || doesNotReturn) {
				return fail('failable direct call `${instruction.id}` in `$functionId` is outside the constructor-status subset');
			}
			final failedStatements:Array<CStmt> = [];
			emitCleanupSteps(failedStatements, failure.cleanup, fn, values, localNames, globalNames, spanLengthNames, boundsAbortName);
			// A propagated constructor failure returns from this function before its
			// terminator runs. Unlink the exact-root frame on that early exit too;
			// otherwise the collector would retain a pointer to dead C stack storage.
			emitManagedRootFramePop(failedStatements, fn, boundsAbortName);
			emitFailureTarget(failedStatements, failure, fn, boundsAbortName, 'call `${instruction.id}`');
			statements.push(SIf(EUnary(LogicalNot, callExpression), SBlock(failedStatements), null));
			return false;
		}
		if (call.returnType == IRTVoid) {
			if (instruction.result != null) {
				fail('Void call `${instruction.id}` in `$functionId` unexpectedly defines a value');
			}
			statements.push(SExpr(callExpression));
			return doesNotReturn;
		}
		if (doesNotReturn) {
			// The call cannot produce an observable result. Emitting a temporary would
			// leave an intentionally unreachable value as a warning-visible C local.
			requireResult(instruction, functionId);
			statements.push(SExpr(callExpression));
			return true;
		}

		final result = requireResult(instruction, functionId);
		final temporaryName = temporaryNames.get(result.id);
		if (temporaryName == null) {
			if (referencedValues.exists(result.id)) {
				fail('referenced call result `${result.id}` in `$functionId` has no finalized C temporary');
			}
			statements.push(SExpr(callExpression));
			return doesNotReturn;
		}
		final declaration = typedDeclarator(result.type, DName(temporaryName));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(callExpression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
		return doesNotReturn;
	}

	/** Emit the validator-approved `Map<Int, Bool>` operation family. */
	function emitIntMapCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final operation = switch call.dispatch {
			case IRCDRuntime("int-map", value): value;
			case _: return fail('IntMap emitter received a non-IntMap call in `${fn.id}`');
		};
		addLineDirective(statements, instruction.source, lineDirectives);
		switch operation {
			case "create":
				final result = requireResult(instruction, fn.id);
				final temporary = requireIntMapTemporary(temporaryNames, result.id, instruction.id, fn.id);
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: new CType(TStruct(new CIdentifier("hxc_int_bool_map_ref"))),
					declarator: DPointer(DName(temporary), []),
					initializer: IExpr(ENull),
					attributes: []
				}));
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapCreate)), [
					ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
					EUnary(AddressOf, EIdentifier(temporary))
				]), boundsAbortName, instruction.id, fn.id);
				values.set(result.id, EIdentifier(temporary));
			case "set":
				if (call.arguments.length != 3 || call.returnType != IRTVoid || instruction.result != null)
					return fail('IntMap set `${instruction.id}` in `${fn.id}` lost its map/key/value signature');
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapSet)), [
					requireValue(values, call.arguments[0], fn.id),
					requireValue(values, call.arguments[1], fn.id),
					requireValue(values, call.arguments[2], fn.id)
				]), boundsAbortName, instruction.id, fn.id);
			case "exists":
				final result = requireResult(instruction, fn.id);
				final temporary = requireIntMapTemporary(temporaryNames, result.id, instruction.id, fn.id);
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: new CType(TBool),
					declarator: DName(temporary),
					initializer: null,
					attributes: []
				}));
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNIntMapExists)), [
					requireValue(values, call.arguments[0], fn.id),
					requireValue(values, call.arguments[1], fn.id),
					EUnary(AddressOf, EIdentifier(temporary))
				]), boundsAbortName, instruction.id, fn.id);
				values.set(result.id, EIdentifier(temporary));
			case _:
				fail('IntMap call `${instruction.id}` in `${fn.id}` names unsupported operation `$operation`');
		}
	}

	static function requireIntMapTemporary(temporaryNames:Map<String, CIdentifier>, resultId:String, instructionId:String, functionId:String):CIdentifier {
		final temporary = temporaryNames.get(resultId);
		if (temporary == null)
			return fail('IntMap call `$instructionId` in `$functionId` has no finalized result temporary');
		return temporary;
	}

	/** Emit one validator-approved Map<String, V> operation through exact unboxed storage. */
	function emitStringMapCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final operation = switch call.dispatch {
			case IRCDRuntime("string-map", value): value;
			case _: return fail('StringMap emitter received a non-StringMap call in `${fn.id}`');
		};
		addLineDirective(statements, instruction.source, lineDirectives);
		switch operation {
			case "create":
				final result = requireResult(instruction, fn.id);
				final temporary = requireStringMapTemporary(temporaryNames, result.id, instruction.id, fn.id);
				final valueType = requireStringMapValueType(result.type, instruction.id, fn.id);
				final mapPlan = requireStringMapPlan(result.type, instruction.id, fn.id);
				final valueDeclaration = typedDeclarator(valueType, DName(null));
				final resultDeclaration = typedDeclarator(result.type, DName(temporary));
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: resultDeclaration.type,
					declarator: resultDeclaration.declarator,
					initializer: IExpr(ENull),
					attributes: []
				}));
				final valueOperations = ECompoundLiteral(new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringMapValueOpsType))), DName(null), IList([
					{designators: [], value: IExpr(ESizeOfType(valueDeclaration.type, valueDeclaration.declarator))},
					{designators: [], value: IExpr(EAlignOfType(valueDeclaration.type, valueDeclaration.declarator))},
					{designators: [], value: IExpr(ENull)},
					{designators: [], value: IExpr(mapPlan.copyName == null ? ENull : EIdentifier(mapPlan.copyName))},
					{designators: [], value: IExpr(mapPlan.assignName == null ? ENull : EIdentifier(mapPlan.assignName))},
					{designators: [], value: IExpr(mapPlan.destroyName == null ? ENull : EIdentifier(mapPlan.destroyName))}
				]));
				final createCall = if (mapPlan.hasLifecycle()) ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapCreateWithOps)), [
					ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
					valueOperations,
					EUnary(AddressOf, EIdentifier(temporary))
				]) else ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapCreate)), [
					ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
					ESizeOfType(valueDeclaration.type, valueDeclaration.declarator),
					EAlignOfType(valueDeclaration.type, valueDeclaration.declarator),
					EUnary(AddressOf, EIdentifier(temporary))
				]);
				emitStatusAbort(statements, createCall, boundsAbortName, instruction.id, fn.id);
				values.set(result.id, EIdentifier(temporary));
				if (!referencedValues.exists(result.id))
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
			case "set":
				if (instruction.result != null || call.returnType != IRTVoid || call.arguments.length != 3)
					return fail('StringMap set `${instruction.id}` in `${fn.id}` lost its Void map/key/value signature');
				final storedValueType = valueType(fn, call.arguments[2]);
				if (storedValueType == null)
					return fail('StringMap set `${instruction.id}` in `${fn.id}` lost its value type');
				final declaration = typedDeclarator(storedValueType, DName(null));
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapSetCopy)), [
					requireValue(values, call.arguments[0], fn.id),
					requireValue(values, call.arguments[1], fn.id),
					arrayElementPointer(requireValue(values, call.arguments[2], fn.id), storedValueType, declaration)
				]), boundsAbortName, instruction.id, fn.id);
			case "clear":
				if (instruction.result != null || call.returnType != IRTVoid || call.arguments.length != 1)
					return fail('StringMap clear `${instruction.id}` in `${fn.id}` lost its Void receiver signature');
				emitStatusAbort(statements,
					ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapClear)), [requireValue(values, call.arguments[0], fn.id)]), boundsAbortName,
					instruction.id, fn.id);
			case "exists" | "remove":
				final result = requireResult(instruction, fn.id);
				final temporary = requireStringMapTemporary(temporaryNames, result.id, instruction.id, fn.id);
				final declaration = typedDeclarator(result.type, DName(temporary));
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: declaration.type,
					declarator: declaration.declarator,
					initializer: null,
					attributes: []
				}));
				final runtimeName = operation == "exists" ? CBRNStringMapExists : CBRNStringMapRemove;
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(runtimeName)), [
					requireValue(values, call.arguments[0], fn.id),
					requireValue(values, call.arguments[1], fn.id),
					EUnary(AddressOf, EIdentifier(temporary))
				]), boundsAbortName, instruction.id, fn.id);
				values.set(result.id, EIdentifier(temporary));
				if (!referencedValues.exists(result.id))
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
			case "get":
				final result = requireResult(instruction, fn.id);
				final temporary = requireStringMapTemporary(temporaryNames, result.id, instruction.id, fn.id);
				final optional = requireOptional(result.type);
				final declaration = typedDeclarator(result.type, DName(temporary));
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: declaration.type,
					declarator: declaration.declarator,
					initializer: IExpr(directOptionalNullExpression(result.type)),
					attributes: []
				}));
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringMapGetCopy)), [
					requireValue(values, call.arguments[0], fn.id),
					requireValue(values, call.arguments[1], fn.id),
					EUnary(AddressOf, EMember(EIdentifier(temporary), optional.payloadName, false)),
					EUnary(AddressOf, EMember(EIdentifier(temporary), optional.presenceName, false))
				]), boundsAbortName, instruction.id, fn.id);
				values.set(result.id, EIdentifier(temporary));
				if (!referencedValues.exists(result.id))
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
			case _:
				fail('StringMap call `${instruction.id}` in `${fn.id}` names unsupported operation `$operation`');
		}
	}

	function requireStringMapValueType(type:HxcIRTypeRef, instructionId:String, functionId:String):HxcIRTypeRef {
		return switch type {
			case IRTInstance(instanceId):
				final value = stringMapValueTypes.get(instanceId);
				if (value == null) fail('StringMap operation `$instructionId` in `$functionId` has unknown specialization `$instanceId`'); else value;
			case _:
				fail('StringMap operation `$instructionId` in `$functionId` lost its specialized instance type');
		};
	}

	function requireStringMapPlan(type:HxcIRTypeRef, instructionId:String, functionId:String):CLoweredBodyStringMap {
		final instanceId = switch type {
			case IRTInstance(value): value;
			case _:
				return fail('StringMap operation `$instructionId` in `$functionId` lost its specialized instance type');
		};
		final plan = stringMapsByInstance.get(instanceId);
		if (plan == null)
			return fail('StringMap operation `$instructionId` in `$functionId` has no finalized value-lifecycle plan');
		if (plan.hasLifecycle() && (plan.copyName == null || plan.assignName == null || plan.destroyName == null))
			return fail('managed StringMap `$instanceId` lost its complete value callback trio');
		if (!plan.hasLifecycle() && (plan.copyName != null || plan.assignName != null || plan.destroyName != null))
			return fail('trivial StringMap `$instanceId` unexpectedly received value callbacks');
		return plan;
	}

	static function requireStringMapTemporary(temporaryNames:Map<String, CIdentifier>, resultId:String, instructionId:String, functionId:String):CIdentifier {
		final temporary = temporaryNames.get(resultId);
		if (temporary == null)
			return fail('StringMap call `$instructionId` in `$functionId` has no finalized result temporary');
		return temporary;
	}

	/** Emit one validator-approved managed Array operation through checked hxrt calls. */
	function emitManagedArrayCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final operation = switch call.dispatch {
			case IRCDRuntime("array", value): value;
			case _: return fail('managed Array emitter received a non-Array call in `${fn.id}`');
		};
		final result = requireResult(instruction, fn.id);
		final temporary = temporaryNames.get(result.id);
		if (temporary == null)
			return fail('managed Array call `${instruction.id}` in `${fn.id}` has no finalized result temporary');
		addLineDirective(statements, instruction.source, lineDirectives);
		switch operation {
			case "create-literal":
				final instanceId = requireArrayInstanceId(result.type, instruction.id, fn.id);
				final elementType = requireArrayElementType(instanceId);
				final arrayPlan = requireArrayPlan(instanceId);
				final resultDeclaration = typedDeclarator(result.type, DName(temporary));
				statements.push(SDecl({
					storage: [],
					alignments: [],
					type: resultDeclaration.type,
					declarator: resultDeclaration.declarator,
					initializer: IExpr(ENull),
					attributes: []
				}));
				final elementDeclaration = typedDeclarator(elementType, DName(null));
				final elementOperations = if (arrayPlan.hasLifecycle()) {
					if (arrayPlan.copyName == null || arrayPlan.assignName == null || arrayPlan.destroyName == null)
						return fail('managed Array `$instanceId` lost its finalized lifecycle callback names');
					ECompoundLiteral(new CType(TNamed(CBodyRuntimeNames.identifier(CBRNArrayElementOpsType))), DName(null), IList([
						{designators: [], value: IExpr(ESizeOfType(elementDeclaration.type, elementDeclaration.declarator))},
						{designators: [], value: IExpr(EAlignOfType(elementDeclaration.type, elementDeclaration.declarator))},
						{designators: [], value: IExpr(ENull)},
						{designators: [], value: IExpr(EIdentifier(arrayPlan.copyName))},
						{designators: [], value: IExpr(EIdentifier(arrayPlan.assignName))},
						{designators: [], value: IExpr(EIdentifier(arrayPlan.destroyName))}
					]));
				} else {
					ECompoundLiteral(new CType(TNamed(CBodyRuntimeNames.identifier(CBRNArrayElementOpsType))), DName(null), IList([
						{designators: [], value: IExpr(ESizeOfType(elementDeclaration.type, elementDeclaration.declarator))},
						{designators: [], value: IExpr(EAlignOfType(elementDeclaration.type, elementDeclaration.declarator))},
						{designators: [], value: IExpr(ENull)},
						{designators: [], value: IExpr(ENull)},
						{designators: [], value: IExpr(ENull)},
						{designators: [], value: IExpr(ENull)}
					]));
				};
				if (arrayPlan.prepared.managedByCollector) {
					final descriptor = arrayPlan.descriptorName;
					final program = managedProgram;
					if (descriptor == null || program == null)
						return fail('collector-managed Array `$instanceId` lost its descriptor or program context');
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNGcAllocate)), [
						EUnary(AddressOf, EIdentifier(program.collector)),
						EUnary(AddressOf, EIdentifier(descriptor)),
						ECast(new CType(TVoid), DPointer(DPointer(DName(null), []), []), EUnary(AddressOf, EIdentifier(temporary)))
					]), boundsAbortName, instruction.id, fn.id);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayInitInPlace)), [
						ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
						elementOperations,
						EIdentifier(temporary)
					]), boundsAbortName, instruction.id, fn.id);
				} else {
					final createCall = if (arrayPlan.hasLifecycle()) ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayCreate)),
						[
							ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
							elementOperations,
							EUnary(AddressOf, EIdentifier(temporary))
						]) else ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayCreateTrivial)), [
							ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []),
							ESizeOfType(elementDeclaration.type, elementDeclaration.declarator),
							EAlignOfType(elementDeclaration.type, elementDeclaration.declarator),
							EUnary(AddressOf, EIdentifier(temporary))
					]);
					emitStatusAbort(statements, createCall, boundsAbortName, instruction.id, fn.id);
				}
				for (argumentId in call.arguments) {
					final element = requireValue(values, argumentId, fn.id);
					final storage = EMember(EIdentifier(temporary), CBodyRuntimeNames.identifier(CBRNArrayValueMember), true);
					emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArrayStoragePushCopy)), [
						EUnary(AddressOf, storage),
						arrayElementPointer(element, elementType, elementDeclaration)
					]), boundsAbortName, instruction.id, fn.id);
				}
				values.set(result.id, EIdentifier(temporary));
			case "length":
				emitManagedArrayOutCall(statements, values, instruction, call, temporary, CBRNArrayLength, boundsAbortName, fn);
			case "get-checked":
				emitManagedArrayOutCall(statements, values, instruction, call, temporary, CBRNArrayGetCopy, boundsAbortName, fn);
			case "push":
				emitManagedArrayOutCall(statements, values, instruction, call, temporary, CBRNArrayPushCopy, boundsAbortName, fn);
			case "set":
				if (call.arguments.length != 3)
					return fail('Array set `${instruction.id}` in `${fn.id}` lost its three arguments');
				final arguments = call.arguments.map(valueId -> requireValue(values, valueId, fn.id));
				arguments[1] = ECast(new CType(TSizeT), DName(null), arguments[1]);
				final elementType = valueType(fn, call.arguments[2]);
				if (elementType == null)
					return fail('Array set `${instruction.id}` in `${fn.id}` lost its element type');
				final elementDeclaration = typedDeclarator(elementType, DName(null));
				arguments[2] = arrayElementPointer(arguments[2], elementType, elementDeclaration);
				emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNArraySetCopy)), arguments), boundsAbortName, instruction.id,
					fn.id);
				values.set(result.id, requireValue(values, call.arguments[2], fn.id));
				if (!referencedValues.exists(result.id))
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), requireValue(values, result.id, fn.id))));
				return;
			case _:
				fail('managed Array call `${instruction.id}` in `${fn.id}` names unsupported operation `$operation`');
		}
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
	}

	function emitManagedArrayOutCall(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, call:HxcIRCall, temporary:CIdentifier,
			runtimeName:CBodyRuntimeName, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final result = requireResult(instruction, fn.id);
		final declaration = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: null,
			attributes: []
		}));
		final arguments = call.arguments.map(valueId -> requireValue(values, valueId, fn.id));
		if (runtimeName == CBRNArrayGetCopy && arguments.length == 2)
			arguments[1] = ECast(new CType(TSizeT), DName(null), arguments[1]);
		if (runtimeName == CBRNArrayPushCopy && arguments.length == 2) {
			final elementType = valueType(fn, call.arguments[1]);
			if (elementType == null)
				return fail('Array push `${instruction.id}` in `${fn.id}` lost its element type');
			final elementDeclaration = typedDeclarator(elementType, DName(null));
			arguments[1] = arrayElementPointer(arguments[1], elementType, elementDeclaration);
		}
		arguments.push(EUnary(AddressOf, EIdentifier(temporary)));
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(runtimeName)), arguments), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
	}

	/** Emit checked Unicode-scalar String length into one Haxe `Int` temporary. */
	function emitStringLengthCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final result = requireResult(instruction, fn.id);
		final temporary = temporaryNames.get(result.id);
		if (temporary == null || call.arguments.length != 1 || typeKey(result.type) != typeKey(IRTInt(32, true)))
			return fail('String.length call `${instruction.id}` in `${fn.id}` lost its checked String/Int signature');
		final declaration = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: null,
			attributes: []
		}));
		addLineDirective(statements, instruction.source, lineDirectives);
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringLength)), [
			requireValue(values, call.arguments[0], fn.id),
			EUnary(AddressOf, EIdentifier(temporary))
		]), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
	}

	/** Emit total `charCodeAt` into the compiler's structural `Null<Int>` carrier. */
	function emitStringCharCodeAtCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, fn:HxcIRFunction):Void {
		final result = requireResult(instruction, fn.id);
		final temporary = temporaryNames.get(result.id);
		if (temporary == null || call.arguments.length != 2)
			return fail('String.charCodeAt call `${instruction.id}` in `${fn.id}` lost its String/Int signature');
		final optional = requireOptional(result.type);
		final declaration = typedDeclarator(result.type, DName(temporary));
		final target = EIdentifier(temporary);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(directOptionalNullExpression(result.type)),
			attributes: []
		}));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SExpr(EBinary(Assign, EMember(target, optional.presenceName, false),
			ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringCharCodeAt)), [
				requireValue(values, call.arguments[0], fn.id),
				requireValue(values, call.arguments[1], fn.id),
				EUnary(AddressOf, EMember(target, optional.payloadName, false))
			]))));
		values.set(result.id, target);
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), target)));
	}

	/**
		Emit Haxe substring bounds into the allocation-free runtime slicer.

		The result is a small `hxc_string` value that points into the receiver's
		bytes. Ownership is deliberately not changed here: the lowering layer adds
		a retain only when a runtime-created view can escape the expression.
	**/
	function emitStringSubstringCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final result = requireResult(instruction, fn.id);
		final temporary = temporaryNames.get(result.id);
		if (temporary == null
			|| call.arguments.length != 4
			|| (result.type != IRTString && result.type != IRTManagedString)
			|| call.returnType != result.type)
			return fail('String.substring call `${instruction.id}` in `${fn.id}` lost its checked String/Int signature');
		final declaration = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(stringNullExpression()),
			attributes: []
		}));
		addLineDirective(statements, instruction.source, lineDirectives);
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNStringSubstring)), [
			requireValue(values, call.arguments[0], fn.id),
			requireValue(values, call.arguments[1], fn.id),
			requireValue(values, call.arguments[2], fn.id),
			requireValue(values, call.arguments[3], fn.id),
			EUnary(AddressOf, EIdentifier(temporary))
		]), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
	}

	/** Emit one validator-approved fixed-length Bytes operation. */
	function emitManagedBytesCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final operation = switch call.dispatch {
			case IRCDRuntime("bytes", value): value;
			case _: return fail('managed Bytes emitter received a non-Bytes call in `${fn.id}`');
		};
		final arguments = call.arguments.map(valueId -> requireValue(values, valueId, fn.id));
		var runtimeName:CBodyRuntimeName;
		switch operation {
			case "alloc":
				runtimeName = CBRNBytesCreateZeroed;
				arguments.unshift(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []));
			case "of-string-utf8":
				runtimeName = CBRNBytesCreateUtf8Copy;
				arguments.unshift(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []));
			case "length":
				runtimeName = CBRNBytesLength;
			case "get":
				runtimeName = CBRNBytesGet;
			case "set":
				runtimeName = CBRNBytesSet;
			case "sub":
				runtimeName = CBRNBytesSub;
			case "blit":
				runtimeName = CBRNBytesBlit;
			case "fill":
				runtimeName = CBRNBytesFill;
			case "compare":
				runtimeName = CBRNBytesCompare;
			case _:
				return fail('managed Bytes call `${instruction.id}` in `${fn.id}` names unsupported operation `$operation`');
		}
		addLineDirective(statements, instruction.source, lineDirectives);
		if (call.returnType == IRTVoid) {
			if (instruction.result != null)
				return fail('Void Bytes call `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
			emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(runtimeName)), arguments), boundsAbortName, instruction.id, fn.id);
			return;
		}
		final result = requireResult(instruction, fn.id);
		final temporary = temporaryNames.get(result.id);
		if (temporary == null)
			return fail('managed Bytes call `${instruction.id}` in `${fn.id}` has no finalized result temporary');
		final declaration = typedDeclarator(result.type, DName(temporary));
		final ownsBytes = switch result.type {
			case IRTInstance(instanceId): bytesInstanceIds.exists(instanceId);
			case _: false;
		};
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: ownsBytes ? IExpr(ENull) : null,
			attributes: []
		}));
		arguments.push(EUnary(AddressOf, EIdentifier(temporary)));
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(runtimeName)), arguments), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
	}

	/** Emit one validator-approved allocation-backed String operation. */
	function emitManagedStringCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			call:HxcIRCall, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Void {
		final operation = switch call.dispatch {
			case IRCDRuntime("string", value): value;
			case _: return fail('managed String emitter received a non-String call in `${fn.id}`');
		};
		final result = requireResult(instruction, fn.id);
		if (result.type != IRTManagedString || call.returnType != IRTManagedString)
			return fail('managed String call `${instruction.id}` in `${fn.id}` lost its owned result type');
		final temporary = temporaryNames.get(result.id);
		if (temporary == null)
			return fail('managed String call `${instruction.id}` in `${fn.id}` has no finalized result temporary');
		final runtimeName = switch operation {
			case "from-scalar": CBRNStringFromScalar;
			case "concat": CBRNStringConcat;
			case _: return fail('managed String call `${instruction.id}` in `${fn.id}` names unsupported operation `$operation`');
		};
		final arguments = call.arguments.map(valueId -> requireValue(values, valueId, fn.id));
		arguments.push(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNDefaultAllocator)), []));
		final declaration = typedDeclarator(result.type, DName(temporary));
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: declaration.type,
			declarator: declaration.declarator,
			initializer: IExpr(stringNullExpression()),
			attributes: []
		}));
		arguments.push(EUnary(AddressOf, EIdentifier(temporary)));
		addLineDirective(statements, instruction.source, lineDirectives);
		emitStatusAbort(statements, ECall(EIdentifier(CBodyRuntimeNames.identifier(runtimeName)), arguments), boundsAbortName, instruction.id, fn.id);
		values.set(result.id, EIdentifier(temporary));
		if (!referencedValues.exists(result.id))
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporary))));
	}

	/**
		Pass one element by address without wrapping an aggregate inside itself.

		A scalar expression such as `12` is not addressable in C, so it becomes the
		compound literal `(int32_t){12}` first. Struct-valued carriers—including
		closed records, literal-backed Strings, tagged optionals, and tagged enums—are
		already emitted as addressable values (normally their own compound literal or
		temporary). Adding another struct initializer would incorrectly try to
		initialize the first field with the whole value.
	**/
	function arrayElementPointer(value:CExpr, type:HxcIRTypeRef, declaration:CTypedDeclarator):CExpr {
		switch type {
			case IRTString | IRTNullable(_, IRNTagged):
				return EUnary(AddressOf, value);
			case IRTInstance(instanceId):
				if (aggregateTags.exists(instanceId))
					return EUnary(AddressOf, value);
				final representation = enumRepresentations.get(instanceId);
				if (representation != null) {
					switch representation {
						case CBECTagged: return EUnary(AddressOf, value);
						case CBECNative:
					}
				}
			case _:
		}
		return EUnary(AddressOf, ECompoundLiteral(declaration.type, declaration.declarator, IList([{designators: [], value: IExpr(value)}])));
	}

	static function emitStatusAbort(statements:Array<CStmt>, call:CExpr, boundsAbortName:Null<CIdentifier>, instructionId:String, functionId:String):Void
		statements.push(SIf(EBinary(NotEqual, call, EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk))),
			SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, instructionId, functionId)), [])), null));

	function virtualCallExpression(slotId:String, receiverValueId:String, argumentValueIds:Array<String>, values:Map<String, CExpr>, fn:HxcIRFunction,
			instructionId:String):CExpr {
		final slot = requireVirtualSlot(slotId);
		final layout = requireLayoutForSlot(slot);
		final receiverType = valueType(fn, receiverValueId);
		final receiverInstanceId = switch receiverType {
			case IRTPointer(IRTInstance(instanceId), _): instanceId;
			case _: return fail('virtual call `$instructionId` in `${fn.id}` lost its class receiver type');
		};
		final rawReceiver = requireValue(values, receiverValueId, fn.id);
		final slotReceiver:CExpr = if (receiverInstanceId == slot.ownerInstanceId) {
			rawReceiver;
		} else {
			final nullable = switch receiverType {
				case IRTPointer(_, value): value;
				case _: false;
			};
			classUpcastExpression(rawReceiver, receiverType, IRTPointer(IRTInstance(slot.ownerInstanceId), nullable), instructionId, fn.id);
		};
		final rootPath = classBasePath(slot.ownerInstanceId, layout.rootInstanceId, instructionId, fn.id, true);
		var rootObject:CExpr = slotReceiver;
		for (index in 0...rootPath.length)
			rootObject = EMember(rootObject, rootPath[index], index == 0);
		final header = EMember(rootObject, requireClassDispatchHeader(layout.rootInstanceId), rootPath.length == 0);
		final functionPointer = EMember(header, slot.cMember, true);
		final arguments:Array<CExpr> = [slotReceiver];
		for (argumentValueId in argumentValueIds)
			arguments.push(requireValue(values, argumentValueId, fn.id));
		return ECall(functionPointer, arguments);
	}

	function interfaceCallExpression(interfaceTypeId:String, slotId:String, receiverValueId:String, argumentValueIds:Array<String>, values:Map<String, CExpr>,
			fn:HxcIRFunction, instructionId:String):CExpr {
		final layout = requireInterfaceLayout(interfaceTypeId);
		final slot = requireVirtualSlot(slotId);
		if (slot.ownerInstanceId != interfaceTypeId || requireLayoutForSlot(slot).id != layout.id)
			return fail('interface call `$instructionId` in `${fn.id}` selected a slot from another interface');
		final receiver = requireValue(values, receiverValueId, fn.id);
		final table = EMember(receiver, requireInterfaceTableMember(layout), false);
		final functionPointer = EMember(table, slot.cMember, true);
		final arguments:Array<CExpr> = [EMember(receiver, requireInterfaceObjectMember(layout), false)];
		for (argumentValueId in argumentValueIds)
			arguments.push(requireValue(values, argumentValueId, fn.id));
		return ECall(functionPointer, arguments);
	}

	static function isHostedOutputDispatch(dispatch:HxcIRCallDispatch):Bool {
		return switch dispatch {
			case IRCDRuntime("io", operationId): operationId == "sys-println-literal" || operationId == "trace-literal";
			case _: false;
		};
	}

	static function emitHostedPrintln(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, call:HxcIRCall, lineDirectives:Bool,
			functionId:String):Void {
		if (instruction.result != null || call.returnType != IRTVoid || call.arguments.length != 1) {
			fail('hosted output call `${instruction.id}` in `$functionId` lost its validated Void/string signature');
		}
		final failure = call.failure;
		if (failure == null || failure.kind != IRFNativeStatus || failure.target != IRFTAbort || failure.arguments.length != 0 || failure.cleanup.length != 0) {
			fail('hosted output call `${instruction.id}` in `$functionId` lost its native-status abort edge');
		}
		final callExpression = ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNPrintln)), [requireValue(values, call.arguments[0], functionId)]);
		final failed = EBinary(NotEqual, callExpression, EIdentifier(CBodyRuntimeNames.identifier(CBRNStatusOk)));
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SIf(failed, SExpr(ECall(EIdentifier(CBodyRuntimeNames.identifier(CBRNAbort)), [])), null));
	}

	static function isNonReturningSelfCall(functionId:String, call:HxcIRCall, nonReturningFunctionIds:Null<Map<String, Bool>>):Bool {
		if (nonReturningFunctionIds == null || !nonReturningFunctionIds.exists(functionId)) {
			return false;
		}
		return switch call.dispatch {
			case IRCDDirect(targetId): targetId == functionId;
			case _: false;
		};
	}

	function emitTailLoopCall(statements:Array<CStmt>, values:Map<String, CExpr>, instruction:HxcIRInstruction, call:HxcIRCall, fn:HxcIRFunction,
			parameterNames:Map<String, CIdentifier>, tailArgumentNames:Map<String, Array<CIdentifier>>, lineDirectives:Bool):Void {
		if (call.arguments.length != fn.parameters.length) {
			fail('self-tail call `${instruction.id}` in `${fn.id}` has ${call.arguments.length} arguments for ${fn.parameters.length} parameters');
		}
		var names:Array<CIdentifier> = [];
		var foundNames = false;
		for (candidateId => candidateNames in tailArgumentNames) {
			if (candidateId == instruction.id) {
				names = candidateNames;
				foundNames = true;
				break;
			}
		}
		if (!foundNames || names.length != call.arguments.length) {
			fail('self-tail call `${instruction.id}` in `${fn.id}` has no complete finalized tail-argument names');
		}
		addLineDirective(statements, instruction.source, lineDirectives);
		for (index in 0...call.arguments.length) {
			statements.push(SDecl({
				storage: [],
				alignments: [],
				type: cType(fn.parameters[index].type),
				declarator: DName(names[index]),
				initializer: IExpr(requireValue(values, call.arguments[index], fn.id)),
				attributes: []
			}));
		}
		for (index in 0...fn.parameters.length) {
			statements.push(SExpr(EBinary(Assign, EIdentifier(requireParameterName(parameterNames, fn.parameters[index].id, fn.id)),
				EIdentifier(names[index]))));
		}
		statements.push(SContinue);
	}

	function placeType(place:HxcIRPlace, fn:HxcIRFunction):Null<HxcIRTypeRef> {
		return switch place {
			case IRPLocal(localId): requireLocal(fn, localId).type;
			case IRPGlobal(_): null;
			case IRPDereference(pointerValueId):
				switch valueType(fn, pointerValueId) {
					case IRTPointer(pointee, _): pointee;
					case _: null;
				}
			case IRPField(base, fieldName):
				switch placeType(base, fn) {
					case IRTInstance(instanceId):
						final aggregateType = aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
						if (aggregateType != null) {
							aggregateType;
						} else {
							final imported = imports.typeByInstance(instanceId);
							final importedField = imported == null ? null : imported.field(fieldName);
							importedField == null ? classFieldType(instanceId, fieldName) : importedField.prepared.type.irType;
						}
					case _: null;
				}
			case IRPIndex(base, _):
				switch placeType(base, fn) {
					case IRTFixedArray(element, _, _) | IRTSpan(element, _): element;
					case _: null;
				}
		};
	}

	static function valueType(fn:HxcIRFunction, valueId:String):Null<HxcIRTypeRef> {
		for (parameter in fn.parameters) {
			if (parameter.id == valueId) {
				return parameter.type;
			}
		}
		for (block in fn.blocks) {
			for (parameter in block.parameters) {
				if (parameter.id == valueId) {
					return parameter.type;
				}
			}
			for (instruction in block.instructions) {
				if (instruction.result != null && instruction.result.id == valueId) {
					return instruction.result.type;
				}
			}
		}
		return null;
	}

	function requireAggregateTag(instanceId:String):CIdentifier {
		final tag = aggregateTags.get(instanceId);
		if (tag == null) {
			throw new CBodyEmissionError('direct aggregate instance `$instanceId` has no finalized C tag');
		}
		return tag;
	}

	function requireOptional(type:HxcIRTypeRef):CLoweredBodyOptional {
		final value = optionalsByType.get(exactTypeKey(type));
		if (value == null)
			throw new CBodyEmissionError('direct optional type `${exactTypeKey(type)}` has no finalized C plan');
		return value;
	}

	function requireOptionalPlan(planId:String):CLoweredBodyOptional {
		final value = optionalsByPlan.get(planId);
		if (value == null)
			throw new CBodyEmissionError('direct optional plan `$planId` has no finalized C plan');
		return value;
	}

	function requireAggregateFieldName(instanceId:String, fieldName:String, instructionId:String, functionId:String):CIdentifier {
		final name = aggregateFieldNames.get(aggregateFieldKey(instanceId, fieldName));
		if (name == null) {
			throw new CBodyEmissionError('aggregate use `$instructionId` in `$functionId` has no finalized member `$fieldName` for `$instanceId`');
		}
		return name;
	}

	function requireDirectFieldName(instanceId:String, fieldName:String, instructionId:String, functionId:String):CIdentifier {
		final imported = imports.typeByInstance(instanceId);
		if (imported != null) {
			final field = imported.field(fieldName);
			if (field == null)
				return fail('imported aggregate use `$instructionId` in `$functionId` has no finalized member `$fieldName` for `$instanceId`');
			return field.cName;
		}
		return requireAggregateFieldName(instanceId, fieldName, instructionId, functionId);
	}

	function requireAggregateFieldIrType(instanceId:String, fieldName:String):HxcIRTypeRef {
		final type = aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
		if (type == null) {
			throw new CBodyEmissionError('direct aggregate instance `$instanceId` has no finalized type for member `$fieldName`');
		}
		return type;
	}

	function requireAggregateFieldOrder(instanceId:String):Array<String> {
		final order = aggregateFieldOrder.get(instanceId);
		if (order == null) {
			throw new CBodyEmissionError('direct aggregate instance `$instanceId` has no finalized member order');
		}
		return order;
	}

	function classFieldExpression(base:CExpr, instanceId:String, fieldName:String, instructionId:String, functionId:String):CExpr {
		var current = instanceId;
		var expression = base;
		final seen:Map<String, Bool> = [];
		while (!seen.exists(current)) {
			seen.set(current, true);
			final name = classFieldNames.get(classFieldKey(current, fieldName));
			if (name != null)
				return EMember(expression, name, false);
			final baseInstance = classBaseInstances.get(current);
			if (baseInstance == null)
				break;
			expression = EMember(expression, requireClassBaseMember(current), false);
			current = baseInstance;
		}
		return fail('class use `$instructionId` in `$functionId` has no finalized inherited member `$fieldName` for `$instanceId`');
	}

	function classFieldType(instanceId:String, fieldName:String):Null<HxcIRTypeRef> {
		var current:Null<String> = instanceId;
		final seen:Map<String, Bool> = [];
		while (current != null && !seen.exists(current)) {
			seen.set(current, true);
			final type = classFieldTypes.get(classFieldKey(current, fieldName));
			if (type != null)
				return type;
			current = classBaseInstances.get(current);
		}
		return null;
	}

	function classBasePath(sourceInstanceId:String, targetInstanceId:String, instructionId:String, functionId:String,
			allowSameInstance:Bool):Array<CIdentifier> {
		final result:Array<CIdentifier> = [];
		var current:Null<String> = sourceInstanceId;
		final seen:Map<String, Bool> = [];
		while (current != null && current != targetInstanceId && !seen.exists(current)) {
			seen.set(current, true);
			result.push(requireClassBaseMember(current));
			current = classBaseInstances.get(current);
		}
		if (current != targetInstanceId || !allowSameInstance && result.length == 0)
			return fail('class upcast `$instructionId` in `$functionId` has no strict base-prefix path `$sourceInstanceId` -> `$targetInstanceId`');
		return result;
	}

	function requireLayoutForSlot(slot:CBodyEmitterVirtualSlot):CBodyEmitterVirtualLayout {
		for (layout in virtualLayouts) {
			for (candidate in layout.slots) {
				if (candidate.id == slot.id)
					return layout;
			}
		}
		throw new CBodyEmissionError('virtual slot `${slot.id}` has no finalized table layout');
	}

	function requireClassTag(instanceId:String):CIdentifier {
		final tag = classTags.get(instanceId);
		if (tag == null)
			throw new CBodyEmissionError('class instance `$instanceId` has no finalized C tag');
		return tag;
	}

	function requireClassBaseMember(instanceId:String):CIdentifier {
		final member = classBaseMembers.get(instanceId);
		if (member == null)
			throw new CBodyEmissionError('derived class instance `$instanceId` has no finalized base-prefix member');
		return member;
	}

	function requireClassFieldOrder(instanceId:String):Array<String> {
		final order = classFieldOrder.get(instanceId);
		if (order == null)
			throw new CBodyEmissionError('class instance `$instanceId` has no finalized storage-field order');
		return order;
	}

	function requireClassFieldName(instanceId:String, fieldName:String):CIdentifier {
		final name = classFieldNames.get(classFieldKey(instanceId, fieldName));
		if (name == null)
			throw new CBodyEmissionError('class instance `$instanceId` has no finalized storage member `$fieldName`');
		return name;
	}

	function requireClassFieldType(instanceId:String, fieldName:String):HxcIRTypeRef {
		final type = classFieldTypes.get(classFieldKey(instanceId, fieldName));
		if (type == null)
			throw new CBodyEmissionError('class instance `$instanceId` has no finalized storage type for `$fieldName`');
		return type;
	}

	function requireArrayElementType(instanceId:String):HxcIRTypeRef {
		final type = arrayElementTypes.get(instanceId);
		if (type == null)
			throw new CBodyEmissionError('managed Array instance `$instanceId` has no element representation');
		return type;
	}

	function requireArrayPlan(instanceId:String):CLoweredBodyArray {
		final value = arraysByInstance.get(instanceId);
		if (value == null)
			throw new CBodyEmissionError('managed Array instance `$instanceId` has no finalized element-lifecycle plan');
		return value;
	}

	function requireArrayInstanceId(type:HxcIRTypeRef, instructionId:String, functionId:String):String {
		return switch type {
			case IRTInstance(instanceId) if (arrayElementTypes.exists(instanceId)): instanceId;
			case _:
				throw new CBodyEmissionError('managed Array use `$instructionId` in `$functionId` lost its specialized instance type');
		};
	}

	function requireClassDispatchHeader(instanceId:String):CIdentifier {
		final value = classDispatchHeaders.get(instanceId);
		if (value == null)
			throw new CBodyEmissionError('class instance `$instanceId` has no finalized virtual-table pointer member');
		return value;
	}

	function requireVirtualLayout(id:String):CBodyEmitterVirtualLayout {
		final value = virtualLayouts.get(id);
		if (value == null)
			throw new CBodyEmissionError('virtual dispatch has no finalized layout `$id`');
		return value;
	}

	function requireInterfaceLayout(instanceId:String):CBodyEmitterVirtualLayout {
		final value = interfaceLayoutsByInstance.get(instanceId);
		if (value == null)
			throw new CBodyEmissionError('interface dispatch has no finalized value layout for `$instanceId`');
		return value;
	}

	function requireInterfaceObjectMember(layout:CBodyEmitterVirtualLayout):CIdentifier {
		final value = layout.cObjectMember;
		if (value == null)
			throw new CBodyEmissionError('interface layout `${layout.id}` lost its object member');
		return value;
	}

	function requireInterfaceTableMember(layout:CBodyEmitterVirtualLayout):CIdentifier {
		final value = layout.cTableMember;
		if (value == null)
			throw new CBodyEmissionError('interface layout `${layout.id}` lost its table member');
		return value;
	}

	function requireVirtualSlot(id:String):CBodyEmitterVirtualSlot {
		final value = virtualSlots.get(id);
		if (value == null)
			throw new CBodyEmissionError('virtual dispatch has no finalized slot `$id`');
		return value;
	}

	function requireVirtualTable(id:String):CBodyEmitterVirtualTable {
		final value = virtualTables.get(id);
		if (value == null)
			throw new CBodyEmissionError('virtual dispatch has no finalized table `$id`');
		return value;
	}

	function requireClassPriorDeclaration(value:Null<CTypedDeclarator>, tag:CIdentifier):CTypedDeclarator {
		if (value == null)
			throw new CBodyEmissionError('class ${tag.value} lost the declaration of its preceding storage member');
		return value;
	}

	function requireEnumRepresentation(instanceId:String):CBodyEnumCRepresentation {
		final representation = enumRepresentations.get(instanceId);
		if (representation == null) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no finalized C representation');
		}
		return representation;
	}

	function requireEnumValueTag(instanceId:String):CIdentifier {
		final tag = enumValueTags.get(instanceId);
		if (tag == null) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no finalized C value tag');
		}
		return tag;
	}

	function requireEnumDiscriminantTag(instanceId:String):CIdentifier {
		final tag = enumDiscriminantTags.get(instanceId);
		if (tag == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized discriminant tag');
		}
		return tag;
	}

	function requireEnumPayloadUnionTag(instanceId:String):CIdentifier {
		final tag = enumPayloadUnionTags.get(instanceId);
		if (tag == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized payload-union tag');
		}
		return tag;
	}

	function requireEnumTagMember(instanceId:String):CIdentifier {
		final name = enumTagMembers.get(instanceId);
		if (name == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized discriminant member');
		}
		return name;
	}

	function requireEnumPayloadMember(instanceId:String):CIdentifier {
		final name = enumPayloadMembers.get(instanceId);
		if (name == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized payload member');
		}
		return name;
	}

	function requireEnumCaseOrder(instanceId:String):Array<String> {
		final order = enumCaseOrder.get(instanceId);
		if (order == null) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no finalized constructor order');
		}
		return order;
	}

	function requireEnumCaseValue(instanceId:String, caseName:String):Int {
		final key = enumCaseKey(instanceId, caseName);
		if (!enumCaseValues.exists(key)) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no discriminant for constructor `$caseName`');
		}
		final value = enumCaseValues.get(key);
		return value == null ? fail('direct enum instance `$instanceId` lost discriminant `$caseName`') : value;
	}

	function requireEnumCaseDiscriminant(instanceId:String, caseName:String):CIdentifier {
		final name = enumCaseDiscriminants.get(enumCaseKey(instanceId, caseName));
		if (name == null) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no finalized discriminant for constructor `$caseName`');
		}
		return name;
	}

	function requireEnumCasePayloadStructTag(instanceId:String, caseName:String):CIdentifier {
		final tag = enumCasePayloadStructTags.get(enumCaseKey(instanceId, caseName));
		if (tag == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no payload struct for constructor `$caseName`');
		}
		return tag;
	}

	function requireEnumCaseUnionMember(instanceId:String, caseName:String):CIdentifier {
		final name = enumCaseUnionMembers.get(enumCaseKey(instanceId, caseName));
		if (name == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no payload-union member for constructor `$caseName`');
		}
		return name;
	}

	function requireEnumPayloadNames(instanceId:String, caseName:String):Array<String> {
		final names = enumPayloadNames.get(enumCaseKey(instanceId, caseName));
		if (names == null) {
			throw new CBodyEmissionError('direct enum instance `$instanceId` has no payload layout for constructor `$caseName`');
		}
		return names;
	}

	function requireEnumPayloadFieldName(instanceId:String, caseName:String, payloadName:String):CIdentifier {
		final name = enumPayloadFieldNames.get(enumPayloadKey(instanceId, caseName, payloadName));
		if (name == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized `$caseName.$payloadName` payload member');
		}
		return name;
	}

	function requireEnumPayloadFieldType(instanceId:String, caseName:String, payloadName:String):HxcIRTypeRef {
		final type = enumPayloadFieldTypes.get(enumPayloadKey(instanceId, caseName, payloadName));
		if (type == null) {
			throw new CBodyEmissionError('tagged enum instance `$instanceId` has no finalized type for `$caseName.$payloadName`');
		}
		return type;
	}

	function requireEnumInstanceId(type:Null<HxcIRTypeRef>, instructionId:String, functionId:String):String {
		return switch type {
			case IRTInstance(instanceId) if (enumRepresentations.exists(instanceId)): instanceId;
			case _:
				throw new CBodyEmissionError('enum use `$instructionId` in `$functionId` lost its finalized enum instance type');
		};
	}

	static function aggregateFieldKey(instanceId:String, fieldName:String):String
		return instanceId + "\x00" + fieldName;

	static function classFieldKey(instanceId:String, fieldName:String):String
		return instanceId + "\x00" + fieldName;

	static function enumCaseKey(instanceId:String, caseName:String):String
		return instanceId + "\x00" + caseName;

	static function enumPayloadKey(instanceId:String, caseName:String, payloadName:String):String
		return enumCaseKey(instanceId, caseName) + "\x00" + payloadName;

	static function requireResult(instruction:HxcIRInstruction, functionId:String):HxcIRResult {
		final result = instruction.result;
		if (result == null) {
			throw new CBodyEmissionError('value instruction `${instruction.id}` in `$functionId` has no result after validation');
		}
		return result;
	}

	static function requireTerminator(terminator:Null<HxcIRTerminator>, functionId:String):HxcIRTerminator {
		if (terminator == null) {
			throw new CBodyEmissionError('entry block in `$functionId` has no terminator after validation');
		}
		return terminator;
	}

	static function requireValue(values:Map<String, CExpr>, valueId:String, functionId:String):CExpr {
		final value = values.get(valueId);
		if (value == null) {
			throw new CBodyEmissionError('C lowering in `$functionId` cannot resolve HxcIR value `$valueId`');
		}
		return value;
	}

	static function requireSpanValueLength(lengths:Map<String, CExpr>, valueId:String, functionId:String):CExpr {
		final length = lengths.get(valueId);
		if (length == null) {
			throw new CBodyEmissionError('C lowering in `$functionId` cannot resolve the length of HxcIR span value `$valueId`');
		}
		return length;
	}

	static function requireLocal(fn:HxcIRFunction, localId:String):HxcIRLocal {
		for (local in fn.locals) {
			if (local.id == localId) {
				return local;
			}
		}
		throw new CBodyEmissionError('function `${fn.id}` cannot resolve HxcIR local `$localId`');
	}

	static function requireLocalName(localNames:Map<String, CIdentifier>, localId:String, functionId:String):CIdentifier {
		final name = localNames.get(localId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C name for local `$localId`');
		}
		return name;
	}

	static function requireSpanLengthName(spanLengthNames:Map<String, CIdentifier>, localId:String, functionId:String):CIdentifier {
		final name = spanLengthNames.get(localId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C length name for span local `$localId`');
		}
		return name;
	}

	static function requireBoundsAbortName(name:Null<CIdentifier>, instructionId:String, functionId:String):CIdentifier {
		if (name == null) {
			throw new CBodyEmissionError('bounds check `$instructionId` in `$functionId` has no finalized C abort symbol');
		}
		return name;
	}

	static function requireGlobalName(globalNames:Map<String, CIdentifier>, globalId:String, functionId:String):CIdentifier {
		final name = globalNames.get(globalId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C name for global `$globalId`');
		}
		return name;
	}

	static function requireLabelName(labelNames:Map<String, CIdentifier>, blockId:String, functionId:String):CIdentifier {
		final name = labelNames.get(blockId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C label for block `$blockId`');
		}
		return name;
	}

	static function requireParameterName(parameterNames:Map<String, CIdentifier>, parameterId:String, functionId:String):CIdentifier {
		final name = parameterNames.get(parameterId);
		if (name == null) {
			throw new CBodyEmissionError('function `$functionId` has no finalized C name for parameter `$parameterId`');
		}
		return name;
	}

	static function requireFunctionName(functionNames:Map<String, CIdentifier>, targetId:String, ownerId:String):CIdentifier {
		final name = functionNames.get(targetId);
		if (name == null) {
			throw new CBodyEmissionError('function `$ownerId` has no finalized C name for direct target `$targetId`');
		}
		return name;
	}

	static function addLineDirective(statements:Array<CStmt>, source:HxcSourceSpan, enabled:Bool):Void {
		if (enabled) {
			statements.push(SLineDirective({line: source.startLine, file: source.file}));
		}
	}

	static function addUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): 'int:$width:${signed ? "signed" : "unsigned"}';
			case IRTAbiInteger(kind): 'abi-int:$kind';
			case IRTFloat(width): 'float:$width';
			case IRTString: "string-utf8";
			case IRTManagedString: "managed-string-utf8";
			case IRTCString: "cstring-borrowed-literal";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(_, nullable): 'pointer:${nullable ? "nullable" : "non-null"}';
			case IRTNullable(_, representation): 'nullable:$representation';
			case IRTFunction(_, _): "function";
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId<${typeKey(element)}>';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${typeKey(element)}>';
			case IRTDynamic: "dynamic";
		};
	}

	/** Exact structural type key used when a representation plan owns C layout. */
	static function exactTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): 'int:$width:${signed ? "signed" : "unsigned"}';
			case IRTAbiInteger(kind): 'abi-int:$kind';
			case IRTFloat(width): 'float:$width';
			case IRTString: "string-utf8";
			case IRTManagedString: "managed-string-utf8";
			case IRTCString: "cstring";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "non-null"}<${exactTypeKey(pointee)}>';
			case IRTNullable(inner, representation): 'nullable:$representation<${exactTypeKey(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(exactTypeKey).join(",")})->${exactTypeKey(result)}';
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId<${exactTypeKey(element)}>';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}<${exactTypeKey(element)}>';
			case IRTDynamic: "dynamic";
		};
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);
}
#else
class CBodyEmitter {
	public function new() {}
}
#end
