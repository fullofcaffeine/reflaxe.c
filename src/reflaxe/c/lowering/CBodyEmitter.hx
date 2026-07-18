package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRFixedArrayPolicy;
import reflaxe.c.ir.HxcIRFixedArrayPolicy.HxcIRFixedArrayStorageDecision;
import reflaxe.c.interop.CImportRegistry.CImportTypeKind;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyRuntimeNames.CBodyRuntimeName;
import reflaxe.c.lowering.CBodyAggregate.CLoweredBodyAggregate;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyDispatch.CLoweredBodyDispatch;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;

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

/** Lowers the admitted direct-value HxcIR body subset into structural strict C11. */
class CBodyEmitter {
	final aggregateTags:Map<String, CIdentifier> = [];
	final aggregateFieldNames:Map<String, CIdentifier> = [];
	final aggregateFieldTypes:Map<String, HxcIRTypeRef> = [];
	final aggregateFieldOrder:Map<String, Array<String>> = [];
	final aggregateInstanceOrder:Array<String> = [];
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
	final classTags:Map<String, CIdentifier> = [];
	final classBaseInstances:Map<String, String> = [];
	final classBaseMembers:Map<String, CIdentifier> = [];
	final classEmptyAnchors:Map<String, CIdentifier> = [];
	final classFieldNames:Map<String, CIdentifier> = [];
	final classFieldTypes:Map<String, HxcIRTypeRef> = [];
	final classFieldOrder:Map<String, Array<String>> = [];
	final classInstanceOrder:Array<String> = [];
	final classDispatchLayoutIds:Map<String, String> = [];
	final classDispatchHeaders:Map<String, CIdentifier> = [];
	final virtualLayouts:Map<String, CBodyEmitterVirtualLayout> = [];
	final virtualSlots:Map<String, CBodyEmitterVirtualSlot> = [];
	final virtualTables:Map<String, CBodyEmitterVirtualTable> = [];
	final virtualThunks:Array<CBodyEmitterVirtualThunk> = [];
	final imports:CLoweredImports;

	#if (macro || reflaxe_runtime)
	public function new(?aggregates:Array<CLoweredBodyAggregate>, ?enums:Array<CLoweredBodyEnum>, ?classes:Array<CLoweredBodyClass>,
			?dispatch:CLoweredBodyDispatch, ?imports:CLoweredImports) {
		this.imports = imports == null ? CLoweredImports.empty() : imports;
		if (aggregates != null) {
			for (aggregate in aggregates) {
				final instanceId = aggregate.prepared.instanceId;
				aggregateInstanceOrder.push(instanceId);
				aggregateTags.set(instanceId, aggregate.cTag);
				final order:Array<String> = [];
				for (field in aggregate.fields) {
					order.push(field.semanticName);
					aggregateFieldNames.set(aggregateFieldKey(instanceId, field.semanticName), field.cName);
					aggregateFieldTypes.set(aggregateFieldKey(instanceId, field.semanticName), field.type.irType);
				}
				aggregateFieldOrder.set(instanceId, order);
			}
		}
		if (enums != null) {
			for (value in enums) {
				final instanceId = value.prepared.instanceId;
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
			}
		}
		if (classes != null) {
			for (value in classes) {
				final instanceId = value.prepared.instanceId;
				classInstanceOrder.push(instanceId);
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
		if (dispatch != null) {
			for (slot in dispatch.slots) {
				virtualSlots.set(slot.prepared.input.id, {
					id: slot.prepared.input.id,
					ownerInstanceId: slot.prepared.owner.instanceId,
					parameterTypes: slot.prepared.parameters.map(value -> value.irType),
					returnType: slot.prepared.returnType.irType,
					cMember: slot.cMember
				});
			}
			for (layout in dispatch.layouts) {
				final slots:Array<CBodyEmitterVirtualSlot> = [];
				for (slot in layout.slots)
					slots.push(requireVirtualSlot(slot.prepared.input.id));
				virtualLayouts.set(layout.prepared.id, {
					id: layout.prepared.id,
					rootInstanceId: layout.prepared.root.instanceId,
					cTag: layout.cTag,
					slots: slots
				});
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
	}
	#end

	public function emitBody(fn:HxcIRFunction, parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			helperNames:Map<String, CIdentifier>, lineDirectives:Bool, tailArgumentNames:Map<String, Array<CIdentifier>>, labelNames:Map<String, CIdentifier>,
			?nonReturningFunctionIds:Map<String, Bool>, ?spanLengthNames:Map<String, CIdentifier>, ?boundsAbortName:CIdentifier):CStmt {
		if (fn.blocks.length == 0 || fn.entryBlockId != fn.blocks[0].id) {
			fail('body lowering requires an entry-first block graph in `${fn.id}`');
		}
		validateConstructionCleanupRegions(fn);
		final values:Map<String, CExpr> = [];
		final spanValueLengths:Map<String, CExpr> = [];
		final declared:Map<String, Bool> = [];
		final referencedValues = referencedValueIds(fn);
		final referencedLocals = referencedLocalIds(fn);
		final referencedSpanLengths = referencedSpanLengthIds(fn);
		final resolvedSpanLengthNames:Map<String, CIdentifier> = spanLengthNames == null ? [] : spanLengthNames;
		final statements:Array<CStmt> = [];
		var terminatedByTailLoop = false;
		for (parameter in fn.parameters) {
			final name = requireParameterName(parameterNames, parameter.id, fn.id);
			values.set(parameter.id, EIdentifier(name));
			switch parameter.type {
				case IRTSpan(_, _):
					spanValueLengths.set(parameter.id, EIdentifier(requireSpanLengthName(resolvedSpanLengthNames, parameter.id, fn.id)));
				case _:
			}
			if (!referencedValues.exists(parameter.id)) {
				addLineDirective(statements, parameter.source, lineDirectives);
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
				final spanLength = spanValueLengths.get(parameter.id);
				if (spanLength != null)
					statements.push(SExpr(ECast(new CType(TVoid), DName(null), spanLength)));
			}
		}
		for (blockIndex in 0...fn.blocks.length) {
			final block = fn.blocks[blockIndex];
			if (blockIndex > 0) {
				statements.push(SLabel(requireLabelName(labelNames, block.id, fn.id), SEmpty));
			}
			var terminatedByNonReturningCall = false;
			for (instruction in block.instructions) {
				switch instruction.kind {
					case IRIOConstant(value):
						final result = requireResult(instruction, fn.id);
						values.set(result.id, constantExpression(value));
					case IRIOLoad(place):
						switch requireResult(instruction, fn.id).type {
							case IRTSpan(_, _):
								emitSpanLoad(statements, values, spanValueLengths, referencedValues, instruction, place, fn, localNames, globalNames,
									resolvedSpanLengthNames, lineDirectives);
							case _:
								emitLoad(statements, values, referencedValues, instruction,
									placeExpression(place, fn, localNames, globalNames, resolvedSpanLengthNames, values), temporaryNames, lineDirectives,
									fn.id);
						}
					case IRIOAddress(place):
						emitAddress(statements, values, referencedValues, instruction, place, fn, localNames, globalNames, resolvedSpanLengthNames,
							temporaryNames, lineDirectives);
					case IRIOConstructAggregate(instanceId, fields):
						emitAggregateConstruction(statements, values, referencedValues, instruction, instanceId, fields, temporaryNames, lineDirectives, fn.id);
					case IRIOProject(valueId, fieldName):
						emitAggregateProjection(statements, values, referencedValues, instruction, valueId, fieldName, fn, temporaryNames, lineDirectives);
					case IRIOConstructTag(instanceId, tagName, payload):
						emitEnumConstruction(statements, values, referencedValues, instruction, instanceId, tagName, payload, temporaryNames, lineDirectives,
							fn.id);
					case IRIOMatchTag(valueId, tagName):
						emitEnumMatch(statements, values, referencedValues, instruction, valueId, tagName, fn, temporaryNames, lineDirectives);
					case IRIOProjectTag(valueId, tagName, payloadIndex, IRTCPCheckedAbort(_, _)):
						emitEnumProjection(statements, values, referencedValues, instruction, valueId, tagName, payloadIndex, fn, temporaryNames,
							boundsAbortName, lineDirectives);
					case IRIODefaultInitialize(IRPLocal(localId), from, to):
						emitDefaultInitialize(statements, declared, referencedLocals, instruction, localId, from, to, fn, localNames, lineDirectives);
					case IRIOBindVirtualTable(IRPLocal(localId), tableId):
						emitBindVirtualTable(statements, instruction, localId, tableId, fn, localNames, lineDirectives);
					case IRIOInitialize(IRPLocal(localId), valueId, IRISUninitialized, IRISInitialized):
						switch requireLocal(fn, localId).type {
							case IRTSpan(_, _):
								emitSpanValueInitialize(statements, values, spanValueLengths, declared, referencedLocals, referencedSpanLengths, instruction,
									localId, valueId, fn, localNames, resolvedSpanLengthNames, lineDirectives);
							case _:
								emitInitialize(statements, values, declared, referencedLocals, instruction, localId, valueId, fn, localNames, lineDirectives);
						}
					case IRIOInitialize(IRPGlobal(globalId), valueId, IRISUninitialized, IRISInitialized):
						if (instruction.result != null) {
							fail('global initializer `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
						}
						addLineDirective(statements, instruction.source, lineDirectives);
						statements.push(SExpr(EBinary(Assign,
							placeExpression(IRPGlobal(globalId), fn, localNames, globalNames, resolvedSpanLengthNames, values),
							requireValue(values, valueId, fn.id))));
					case IRIOInitializeFixedArray(IRPLocal(localId), valueIds, IRISUninitialized, IRISInitialized):
						emitFixedArrayInitialize(statements, values, declared, referencedLocals, instruction, localId, valueIds, fn, localNames,
							lineDirectives);
					case IRIOZeroInitializeFixedArray(IRPLocal(localId), IRISUninitialized, IRISInitialized):
						emitZeroFixedArrayInitialize(statements, declared, referencedLocals, instruction, localId, fn, localNames, lineDirectives);
					case IRIOInitializeSpan(IRPLocal(localId), sourceArray, IRISUninitialized, IRISInitialized):
						emitSpanInitialize(statements, declared, referencedLocals, referencedSpanLengths, instruction, localId, sourceArray, fn, localNames,
							resolvedSpanLengthNames, globalNames, lineDirectives);
					case IRIOBoundsCheck(collection, indexValueId, IRBPCheckedAbort(_, _)):
						emitBoundsCheck(statements, values, instruction, collection, indexValueId, fn, localNames, globalNames, resolvedSpanLengthNames,
							boundsAbortName, lineDirectives);
					case IRIOBoundsCheck(_, _, IRBPStaticProof(_, _) | IRBPLoopGuarded(_, _, _)):
						// The semantic proof remains reviewable in HxcIR; no redundant C check survives.
					case IRIONullCheck(valueId, IRNCPCheckedAbort(_, _)):
						emitNullCheck(statements, values, instruction, valueId, boundsAbortName, lineDirectives, fn.id);
					case IRIOStore(place, valueId):
						if (instruction.result != null) {
							fail('store `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
						}
						addLineDirective(statements, instruction.source, lineDirectives);
						statements.push(SExpr(EBinary(Assign, placeExpression(place, fn, localNames, globalNames, resolvedSpanLengthNames, values),
							requireValue(values, valueId, fn.id))));
					case IRIOUnary(operationId, valueId, implementation):
						final result = requireResult(instruction, fn.id);
						final expression = operationExpression(operationId, implementation, [requireValue(values, valueId, fn.id)], helperNames,
							instruction.id, fn.id);
						recordPureResult(statements, values, referencedValues, instruction, result, expression, lineDirectives, fn.id);
					case IRIOBinary(operationId, leftValueId, rightValueId, implementation):
						final result = requireResult(instruction, fn.id);
						// The admitted operators are total and pure over values that loads/calls
						// have already stabilized. The evaluation-order suite enforces that
						// boundary before allowing this expression-level elision.
						final expression = operationExpression(operationId, implementation, [
							requireValue(values, leftValueId, fn.id),
							requireValue(values, rightValueId, fn.id)
						], helperNames, instruction.id, fn.id);
						recordPureResult(statements, values, referencedValues, instruction, result, expression, lineDirectives, fn.id);
					case IRIOConvert(valueId, IRCRepresentation, targetType, IRIStatic, null):
						final result = requireResult(instruction, fn.id);
						final expression = classUpcastExpression(requireValue(values, valueId, fn.id), valueType(fn, valueId), targetType, instruction.id,
							fn.id);
						recordPureResult(statements, values, referencedValues, instruction, result, expression, lineDirectives, fn.id);
					case IRIOConvert(valueId, IRCNullableInject, _, IRIStatic, null):
						final result = requireResult(instruction, fn.id);
						recordPureResult(statements, values, referencedValues, instruction, result, requireValue(values, valueId, fn.id), lineDirectives,
							fn.id);
					case IRIOConvert(valueId, kind, targetType, IRIStatic, null):
						final result = requireResult(instruction, fn.id);
						final expression = switch kind {
							case IRCNumericExact | IRCNumericWrapping:
								ECast(cType(targetType), DName(null), requireValue(values, valueId, fn.id));
							case _:
								fail('conversion `${instruction.id}` in `${fn.id}` is outside the admitted direct primitive conversion subset');
						};
						values.set(result.id, expression);
						if (!referencedValues.exists(result.id)) {
							addLineDirective(statements, instruction.source, lineDirectives);
							statements.push(SExpr(ECast(new CType(TVoid), DName(null), expression)));
						}
					case IRIOConvert(valueId, kind, _, IRIProgramLocal(helperId), null):
						final result = requireResult(instruction, fn.id);
						switch kind {
							case IRCNumericExact | IRCNumericWrapping | IRCNumericSaturating:
							case _:
								fail('program-local conversion `${instruction.id}` in `${fn.id}` has unsupported kind `$kind`');
						}
						final expression = helperCall(helperId, [requireValue(values, valueId, fn.id)], helperNames, instruction.id, fn.id);
						recordPureResult(statements, values, referencedValues, instruction, result, expression, lineDirectives, fn.id);
					case IRIOCall(call):
						if (isNonReturningSelfCall(fn.id, call, nonReturningFunctionIds)) {
							emitTailLoopCall(statements, values, instruction, call, fn, parameterNames, tailArgumentNames, lineDirectives);
							terminatedByNonReturningCall = true;
							terminatedByTailLoop = true;
						} else {
							terminatedByNonReturningCall = emitCall(statements, values, spanValueLengths, referencedValues, instruction, call, temporaryNames,
								functionNames, lineDirectives, nonReturningFunctionIds, boundsAbortName, fn);
						}
					case IRIOLifetime(_, _, _, _):
						// Direct stack-object lifetime transitions are semantic proof only.
					case _:
						fail('HxcIR instruction `${instruction.id}` in `${fn.id}` is outside the sequenced direct-value function subset');
				}
				if (terminatedByNonReturningCall) {
					break;
				}
			}

			final terminator = requireTerminator(block.terminator, fn.id);
			if (terminatedByNonReturningCall) {
				switch terminator.kind {
					case IRTReturn(_, cleanup) if (cleanup.length == 0):
					case _:
						fail('non-returning call in `${fn.id}` cannot replace its non-return terminator or cleanup');
				}
				continue;
			}
			addLineDirective(statements, terminator.source, lineDirectives);
			emitTerminator(statements, values, terminator, labelNames, fn, boundsAbortName);
		}
		if (terminatedByTailLoop) {
			if (fn.blocks.length != 1) {
				fail('tail-loop lowering in `${fn.id}` requires one HxcIR block');
			}
			return SBlock([SWhile(EInt(CIntegerLiteral.decimal("1")), SBlock(statements))]);
		}
		return SBlock(statements);
	}

	static function referencedValueIds(fn:HxcIRFunction):Map<String, Bool> {
		final referenced:Map<String, Bool> = [];
		for (block in fn.blocks) {
			for (instruction in block.instructions) {
				switch instruction.kind {
					case IRIOStore(place, valueId) | IRIOInitialize(place, valueId, _, _):
						referenced.set(valueId, true);
						markPlaceValues(place, referenced);
					case IRIOConvert(valueId, _, _, _, _):
						referenced.set(valueId, true);
					case IRIOLoad(place) | IRIOAddress(place):
						markPlaceValues(place, referenced);
					case IRIOConstructAggregate(_, fields):
						for (field in fields) {
							referenced.set(field.valueId, true);
						}
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
					case IRIOBoundsCheck(_, indexValueId, _):
						referenced.set(indexValueId, true);
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
					case IRIOLoad(place) | IRIOStore(place, _) | IRIOAddress(place) | IRIOBoundsCheck(place, _, _) | IRIODefaultInitialize(place, _, _) |
						IRIOBindVirtualTable(place, _) | IRIOLifetime(place, _, _, _):
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
			sourceExpression:CExpr, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String):Void {
		final result = requireResult(instruction, functionId);
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
			spanLengthNames:Map<String, CIdentifier>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		final result = requireResult(instruction, fn.id);
		final pointee = switch result.type {
			case IRTPointer(type, false): type;
			case _: return fail('address `${instruction.id}` in `${fn.id}` lost its validated non-null pointer result');
		};
		final expression = EUnary(AddressOf, placeExpression(place, fn, localNames, globalNames, spanLengthNames, values));
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
			instanceId:String, fields:Array<HxcIRNamedValue>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String):Void {
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
					DField(requireAggregateFieldName(instanceId, field.name, instruction.id, functionId))
				],
				value: IExpr(requireValue(values, field.valueId, functionId))
			});
		}
		final result = requireResult(instruction, functionId);
		final expression = ECompoundLiteral(cType(result.type), DName(null), IList(initializers));
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, functionId);
	}

	function emitAggregateProjection(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			valueId:String, fieldName:String, fn:HxcIRFunction, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		final instanceId = switch valueType(fn, valueId) {
			case IRTInstance(id): id;
			case _: return fail('aggregate projection `${instruction.id}` in `${fn.id}` lost its validated instance value');
		};
		final expression = EMember(requireValue(values, valueId, fn.id), requireDirectFieldName(instanceId, fieldName, instruction.id, fn.id), false);
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id);
	}

	function emitEnumConstruction(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			instanceId:String, tagName:String, payload:Array<String>, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool, functionId:String):Void {
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
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, functionId);
	}

	function emitEnumMatch(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction,
			valueId:String, tagName:String, fn:HxcIRFunction, temporaryNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		final instanceId = requireEnumInstanceId(valueType(fn, valueId), instruction.id, fn.id);
		final expression = EBinary(Equal, enumTagExpression(requireValue(values, valueId, fn.id), instanceId),
			EIdentifier(requireEnumCaseDiscriminant(instanceId, tagName)));
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id);
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
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id);
	}

	function emitDefaultInitialize(statements:Array<CStmt>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>, instruction:HxcIRInstruction,
			localId:String, from:HxcIRInitializationState, to:HxcIRInitializationState, fn:HxcIRFunction, localNames:Map<String, CIdentifier>,
			lineDirectives:Bool):Void {
		if (instruction.result != null || declared.exists(localId) || from != IRISUninitialized || to != IRISInitializing && to != IRISInitialized) {
			fail('default initializer `${instruction.id}` in `${fn.id}` has invalid declaration or lifetime state');
		}
		final local = requireLocal(fn, localId);
		switch local.type {
			case IRTInstance(instanceId) if (classTags.exists(instanceId)):
			case _:
				return fail('default initializer `${instruction.id}` in `${fn.id}` does not target direct concrete-class storage');
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

	function emitBindVirtualTable(statements:Array<CStmt>, instruction:HxcIRInstruction, localId:String, tableId:String, fn:HxcIRFunction,
			localNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null)
			fail('virtual-table bind `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		final table = requireVirtualTable(tableId);
		final local = requireLocal(fn, localId);
		final instanceId = switch local.type {
			case IRTInstance(value): value;
			case _: return fail('virtual-table bind `${instruction.id}` in `${fn.id}` does not target concrete object storage');
		};
		if (instanceId != table.classInstanceId)
			fail('virtual-table bind `${instruction.id}` in `${fn.id}` selected table `$tableId` for the wrong concrete class');
		final path = classBasePath(instanceId, table.layout.rootInstanceId, instruction.id, fn.id, true);
		var rootObject:CExpr = EIdentifier(requireLocalName(localNames, localId, fn.id));
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
			instruction:HxcIRInstruction, localId:String, valueId:String, fn:HxcIRFunction, localNames:Map<String, CIdentifier>, lineDirectives:Bool):Void {
		if (instruction.result != null) {
			fail('initializer `${instruction.id}` in `${fn.id}` unexpectedly defines a value');
		}
		if (declared.exists(localId)) {
			fail('local `$localId` in `${fn.id}` is initialized more than once');
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

	function emitSpanInitialize(statements:Array<CStmt>, declared:Map<String, Bool>, referencedLocals:Map<String, Bool>,
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
		final sourceExpression = placeExpression(sourceArray, fn, localNames, globalNames, spanLengthNames);
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
			boundsAbortName:Null<CIdentifier>, lineDirectives:Bool, functionId:String):Void {
		if (instruction.result != null)
			fail('null check `${instruction.id}` in `$functionId` unexpectedly defines a value');
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SIf(EBinary(Equal, requireValue(values, valueId, functionId), ENull),
			SExpr(ECall(EIdentifier(requireBoundsAbortName(boundsAbortName, instruction.id, functionId)), [])), null));
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
			fn:HxcIRFunction, boundsAbortName:Null<CIdentifier>):Void {
		final functionId = fn.id;
		switch terminator.kind {
			case IRTReturn(valueId, cleanup):
				emitCleanupSteps(statements, cleanup, fn);
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
				emitCleanupSteps(statements, failure.cleanup, fn);
				emitFailureTarget(statements, failure, fn, boundsAbortName, "throw");
			case _:
				fail('function `$functionId` has a terminator outside the sequenced direct-value subset');
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
					case _:
						fail('construction cleanup `${action.id}` in `${fn.id}` is outside the direct stack-object subset');
				}
			}
		}
	}

	function emitCleanupSteps(statements:Array<CStmt>, steps:Array<HxcIRCleanupStep>, fn:HxcIRFunction):Void {
		for (step in steps) {
			final action = requireCleanupAction(fn, step);
			switch action.kind {
				case IRCADestroy(IRPLocal(_), IRISInitializing | IRISInitialized, IRISDestroyed):
					// Direct class storage currently contains only borrowed/direct fields, so
					// destruction is a proven no-op. The ordered HxcIR edge remains authoritative.
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
				fail('function `${fn.id}` requested a collection length from a non-root place');
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
			case "haxe.bool.equal" | "haxe.i32.equal" | "haxe.u32.equal" | "haxe.f64.equal": EBinary(Equal, left, right);
			case "haxe.class-reference.equal": EBinary(Equal, left, right);
			case "haxe.bool.not-equal" | "haxe.i32.not-equal" | "haxe.u32.not-equal" | "haxe.f64.not-equal": EBinary(NotEqual, left, right);
			case "haxe.class-reference.not-equal": EBinary(NotEqual, left, right);
			case "haxe.i32.less" | "haxe.u32.less" | "haxe.f64.less": EBinary(Less, left, right);
			case "haxe.i32.less-equal" | "haxe.u32.less-equal" | "haxe.f64.less-equal": EBinary(LessEqual, left, right);
			case "haxe.i32.greater" | "haxe.u32.greater" | "haxe.f64.greater": EBinary(Greater, left, right);
			case "haxe.i32.greater-equal" | "haxe.u32.greater-equal" | "haxe.f64.greater-equal": EBinary(GreaterEqual, left, right);
			case _: fail('binary instruction `$instructionId` in `$functionId` has unsupported direct operation `$operationId`');
		};
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
			case IRTString: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringType)));
			case IRTInstance(instanceId):
				final imported = imports.typeByInstance(instanceId);
				if (imported != null) {
					switch imported.prepared.kind {
						case CITStruct: new CType(TStruct(imported.cName));
						case CITEnum: new CType(TNamed(imported.cName));
						case CITTypedef: new CType(TNamed(imported.cName));
					}
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
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` is outside the admitted direct-value C body subset');
		};
	}

	public function typedDeclarator(type:HxcIRTypeRef, inner:CDeclarator):CTypedDeclarator {
		return switch type {
			case IRTCString:
				{type: new CType(TChar(null), [QConst]), declarator: DPointer(inner, [])};
			case IRTPointer(pointee, _):
				final nested = typedDeclarator(pointee, DPointer(inner, []));
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
					case _:
				}
			}
			if (block.terminator != null) {
				switch block.terminator.kind {
					case IRTThrow(_, {target: IRFTAbort}):
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
		for (instanceId in aggregateInstanceOrder) {
			final order = requireAggregateFieldOrder(instanceId);
			final fields:Array<CField> = [];
			for (fieldName in order) {
				fields.push({
					type: requireAggregateFieldType(instanceId, fieldName),
					declarator: DName(requireAggregateFieldName(instanceId, fieldName, "definition", instanceId)),
					bitWidth: null,
					alignments: [],
					attributes: []
				});
			}
			result.push(DStruct(requireAggregateTag(instanceId), fields, []));
		}
		return result;
	}

	public function virtualTableForwardDeclarations():Array<CDecl> {
		final result:Array<CDecl> = [];
		final ids = [for (id in virtualLayouts.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids)
			result.push(DForwardStruct(requireVirtualLayout(id).cTag, []));
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
				final receiver = typedDeclarator(IRTPointer(IRTInstance(slot.ownerInstanceId), true), DName(null));
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
				storage: [SStatic],
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
		final receiver = typedDeclarator(IRTPointer(IRTInstance(thunk.slot.ownerInstanceId), true), DName(thunk.receiverName));
		result.push({type: receiver.type, declarator: receiver.declarator, attributes: []});
		for (index in 0...thunk.slot.parameterTypes.length) {
			final parameter = typedDeclarator(thunk.slot.parameterTypes[index], DName(thunk.argumentNames[index]));
			result.push({type: parameter.type, declarator: parameter.declarator, attributes: []});
		}
		return result;
	}

	function virtualThunkDeclarator(thunk:CBodyEmitterVirtualThunk, inner:CDeclarator):CTypedDeclarator
		return typedDeclarator(thunk.slot.returnType, inner);

	public function classDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in classInstanceOrder)
			result.push(DForwardStruct(requireClassTag(instanceId), []));
		for (instanceId in classInstanceOrder) {
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
			result.push(DStruct(requireClassTag(instanceId), fields, []));
		}
		return result;
	}

	public function enumDefinitions():Array<CDecl> {
		final result:Array<CDecl> = [];
		for (instanceId in enumInstanceOrder) {
			if (requireEnumRepresentation(instanceId) == CBECTagged) {
				result.push(DForwardStruct(requireEnumValueTag(instanceId), []));
			}
		}
		for (instanceId in enumInstanceOrder) {
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
				final fieldType = requireAggregateFieldType(instanceId, fieldName);
				final offset = EOffsetOf(structType, DName(null), member);
				if (index == 0) {
					result.push(DStaticAssert(EBinary(Equal, offset, EInt(CIntegerLiteral.decimal("0"))),
						'closed record ${tag.value} first field begins at offset zero'));
				} else {
					final previousName = order[index - 1];
					final previousMember = requireAggregateFieldName(instanceId, previousName, "layout", instanceId);
					final previousType = requireAggregateFieldType(instanceId, previousName);
					result.push(DStaticAssert(EBinary(GreaterEqual, offset,
						EBinary(Add, EOffsetOf(structType, DName(null), previousMember), ESizeOfType(previousType, DName(null)))),
						'closed record ${tag.value} field $index follows the prior field without overlap'));
				}
				result.push(DStaticAssert(EBinary(GreaterEqual, EAlignOfType(structType, DName(null)), EAlignOfType(fieldType, DName(null))),
					'closed record ${tag.value} alignment admits field $index'));
			}
			final lastIndex = order.length - 1;
			final lastName = order[lastIndex];
			final lastMember = requireAggregateFieldName(instanceId, lastName, "layout", instanceId);
			final lastType = requireAggregateFieldType(instanceId, lastName);
			result.push(DStaticAssert(EBinary(GreaterEqual, ESizeOfType(structType, DName(null)),
				EBinary(Add, EOffsetOf(structType, DName(null), lastMember), ESizeOfType(lastType, DName(null)))),
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
			case IRTCString:
			case IRTInstance(instanceId):
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
			{designators: [], value: IExpr(EBool(true))}
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
			lineDirectives:Bool, nonReturningFunctionIds:Null<Map<String, Bool>>, boundsAbortName:Null<CIdentifier>, fn:HxcIRFunction):Bool {
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
			case _: return fail('call `${instruction.id}` in `$functionId` has no admitted static or runtime dispatch');
		};
		addLineDirective(statements, instruction.source, lineDirectives);
		if (call.failure != null) {
			final failure = call.failure;
			if (failure.kind != IRFException || call.returnType != IRTVoid || instruction.result != null || doesNotReturn) {
				return fail('failable direct call `${instruction.id}` in `$functionId` is outside the constructor-status subset');
			}
			final failedStatements:Array<CStmt> = [];
			emitCleanupSteps(failedStatements, failure.cleanup, fn);
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

	function requireAggregateFieldType(instanceId:String, fieldName:String):CType {
		final type = aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
		if (type == null) {
			throw new CBodyEmissionError('direct aggregate instance `$instanceId` has no finalized type for member `$fieldName`');
		}
		return cType(type);
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
