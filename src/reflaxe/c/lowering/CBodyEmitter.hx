package reflaxe.c.lowering;

import haxe.io.Bytes;
import reflaxe.c.ast.CAST;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyRuntimeNames.CBodyRuntimeName;
#if (macro || reflaxe_runtime)
import reflaxe.c.lowering.CBodyAggregate.CLoweredBodyAggregate;
#end

/** Lowers the admitted direct-value HxcIR body subset into structural strict C11. */
class CBodyEmitter {
	final aggregateTags:Map<String, CIdentifier> = [];
	final aggregateFieldNames:Map<String, CIdentifier> = [];
	final aggregateFieldTypes:Map<String, HxcIRTypeRef> = [];
	final aggregateFieldOrder:Map<String, Array<String>> = [];
	final aggregateInstanceOrder:Array<String> = [];

	#if (macro || reflaxe_runtime)
	public function new(?aggregates:Array<CLoweredBodyAggregate>) {
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
	}
	#else
	public function new() {}
	#end

	public function emitBody(fn:HxcIRFunction, parameterNames:Map<String, CIdentifier>, localNames:Map<String, CIdentifier>,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, globalNames:Map<String, CIdentifier>,
			helperNames:Map<String, CIdentifier>, lineDirectives:Bool, tailArgumentNames:Map<String, Array<CIdentifier>>, labelNames:Map<String, CIdentifier>,
			?nonReturningFunctionIds:Map<String, Bool>, ?spanLengthNames:Map<String, CIdentifier>, ?boundsAbortName:CIdentifier):CStmt {
		if (fn.blocks.length == 0 || fn.entryBlockId != fn.blocks[0].id || fn.cleanupRegions.length != 0) {
			fail('body lowering requires a cleanup-free entry-first block graph in `${fn.id}`');
		}
		final values:Map<String, CExpr> = [];
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
			if (!referencedValues.exists(parameter.id)) {
				addLineDirective(statements, parameter.source, lineDirectives);
				statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(name))));
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
						emitLoad(statements, values, referencedValues, instruction,
							placeExpression(place, fn, localNames, globalNames, resolvedSpanLengthNames, values), temporaryNames, lineDirectives, fn.id);
					case IRIOAddress(place):
						emitAddress(statements, values, referencedValues, instruction, place, fn, localNames, globalNames, resolvedSpanLengthNames,
							temporaryNames, lineDirectives);
					case IRIOConstructAggregate(instanceId, fields):
						emitAggregateConstruction(statements, values, referencedValues, instruction, instanceId, fields, temporaryNames, lineDirectives, fn.id);
					case IRIOProject(valueId, fieldName):
						emitAggregateProjection(statements, values, referencedValues, instruction, valueId, fieldName, fn, temporaryNames, lineDirectives);
					case IRIOInitialize(IRPLocal(localId), valueId, IRISUninitialized, IRISInitialized):
						emitInitialize(statements, values, declared, referencedLocals, instruction, localId, valueId, fn, localNames, lineDirectives);
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
					case IRIOInitializeSpan(IRPLocal(localId), sourceArray, IRISUninitialized, IRISInitialized):
						emitSpanInitialize(statements, declared, referencedLocals, referencedSpanLengths, instruction, localId, sourceArray, fn, localNames,
							resolvedSpanLengthNames, globalNames, lineDirectives);
					case IRIOBoundsCheck(collection, indexValueId, IRBPCheckedAbort(_, _)):
						emitBoundsCheck(statements, values, instruction, collection, indexValueId, fn, localNames, globalNames, resolvedSpanLengthNames,
							boundsAbortName, lineDirectives);
					case IRIOBoundsCheck(_, _, IRBPStaticProof(_, _) | IRBPLoopGuarded(_, _, _)):
						// The semantic proof remains reviewable in HxcIR; no redundant C check survives.
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
							terminatedByNonReturningCall = emitCall(statements, values, referencedValues, instruction, call, temporaryNames, functionNames,
								lineDirectives, nonReturningFunctionIds, fn.id);
						}
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
			emitTerminator(statements, values, terminator, labelNames, fn.id);
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
					case IRIOInitializeFixedArray(_, valueIds, _, _):
						for (valueId in valueIds) {
							referenced.set(valueId, true);
						}
					case IRIOBoundsCheck(_, indexValueId, _):
						referenced.set(indexValueId, true);
					case IRIOUnary(_, valueId, _):
						referenced.set(valueId, true);
					case IRIOBinary(_, leftValueId, rightValueId, _):
						referenced.set(leftValueId, true);
						referenced.set(rightValueId, true);
					case IRIOCall(call):
						for (argument in call.arguments) {
							referenced.set(argument, true);
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
					case IRIOLoad(place) | IRIOStore(place, _) | IRIOAddress(place) | IRIOBoundsCheck(place, _, _):
						markReferencedLocals(place, referenced);
					case IRIOInitializeSpan(place, sourceArray, _, _):
						markReferencedLocals(place, referenced);
						markReferencedLocals(sourceArray, referenced);
					case _:
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
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(result.type),
			declarator: DName(temporaryName),
			initializer: IExpr(sourceExpression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
		if (!referencedValues.exists(result.id)) {
			statements.push(SExpr(ECast(new CType(TVoid), DName(null), EIdentifier(temporaryName))));
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
		final expression = EMember(requireValue(values, valueId, fn.id), requireAggregateFieldName(instanceId, fieldName, instruction.id, fn.id), false);
		emitLoad(statements, values, referencedValues, instruction, expression, temporaryNames, lineDirectives, fn.id);
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
		addLineDirective(statements, instruction.source, lineDirectives);
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(local.type),
			declarator: DName(requireLocalName(localNames, localId, fn.id)),
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

	function emitTerminator(statements:Array<CStmt>, values:Map<String, CExpr>, terminator:HxcIRTerminator, labelNames:Map<String, CIdentifier>,
			functionId:String):Void {
		switch terminator.kind {
			case IRTReturn(valueId, cleanup):
				if (cleanup.length != 0) {
					fail('return in `$functionId` unexpectedly requires cleanup');
				}
				statements.push(SReturn(valueId == null ? null : requireValue(values, valueId, functionId)));
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
			case _:
				fail('function `$functionId` has a terminator outside the sequenced direct-value subset');
		}
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
					case _: return fail('function `${fn.id}` lost the aggregate type of field place `$fieldName`');
				};
				EMember(placeExpression(base, fn, localNames, globalNames, spanLengthNames, values),
					requireAggregateFieldName(instanceId, fieldName, "place", fn.id), false);
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
			case "haxe.bool.not-equal" | "haxe.i32.not-equal" | "haxe.u32.not-equal" | "haxe.f64.not-equal": EBinary(NotEqual, left, right);
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
			case IRTFloat(64): new CType(TDouble);
			case IRTString: new CType(TNamed(CBodyRuntimeNames.identifier(CBRNStringType)));
			case IRTInstance(instanceId): new CType(TStruct(requireAggregateTag(instanceId)));
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` is outside the admitted direct-value C body subset');
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
					case _:
				}
			}
		}
		headers.sort(compareUtf8);
		return headers;
	}

	public function parameters(fn:HxcIRFunction, names:Map<String, CIdentifier>):Array<CParam> {
		final result:Array<CParam> = [];
		for (parameter in fn.parameters) {
			result.push({
				type: cType(parameter.type),
				declarator: DName(requireParameterName(names, parameter.id, fn.id)),
				attributes: []
			});
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

	function addTypeHeaders(headers:Array<String>, type:HxcIRTypeRef):Void {
		switch type {
			case IRTBool:
				addUnique(headers, "stdbool.h");
			case IRTInt(_, _):
				addUnique(headers, "stdint.h");
			case IRTAbiInteger(IRAKSize):
				addUnique(headers, "stddef.h");
			case IRTString:
				addUnique(headers, "hxrt/string_literal.h");
			case IRTInstance(instanceId):
				final order = aggregateFieldOrder.get(instanceId);
				if (order == null) {
					throw new CBodyEmissionError('direct aggregate instance `$instanceId` has no finalized C layout');
				}
				for (fieldName in order) {
					final fieldType = aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
					if (fieldType == null) {
						throw new CBodyEmissionError('direct aggregate instance `$instanceId` lost field type `$fieldName`');
					}
					addTypeHeaders(headers, fieldType);
				}
			case IRTPointer(pointee, _):
				addTypeHeaders(headers, pointee);
			case IRTFixedArray(element, _, _):
				addTypeHeaders(headers, element);
			case IRTSpan(element, _):
				addTypeHeaders(headers, element);
				addUnique(headers, "stddef.h");
			case IRTVoid | IRTFloat(64):
			case _:
				throw new CBodyEmissionError('HxcIR type `${typeKey(type)}` has no admitted strict-C direct-value header mapping');
		}
	}

	static function constantExpression(value:HxcIRConstant):CExpr {
		return switch value {
			case IRCInt(text): integerExpression(text);
			case IRCFloat(text): floatExpression(text);
			case IRCBool(value): EBool(value);
			case IRCString(text, byteLength): stringLiteralExpression(text, byteLength);
			case IRCNull:
				throw new CBodyEmissionError("null constants are outside the admitted body subset");
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

	function emitCall(statements:Array<CStmt>, values:Map<String, CExpr>, referencedValues:Map<String, Bool>, instruction:HxcIRInstruction, call:HxcIRCall,
			temporaryNames:Map<String, CIdentifier>, functionNames:Map<String, CIdentifier>, lineDirectives:Bool,
			nonReturningFunctionIds:Null<Map<String, Bool>>, functionId:String):Bool {
		final targetId = switch call.dispatch {
			case IRCDDirect(value): value;
			case dispatch if (isHostedOutputDispatch(dispatch)):
				emitHostedPrintln(statements, values, instruction, call, lineDirectives, functionId);
				return false;
			case _: return fail('call `${instruction.id}` in `$functionId` has no admitted static or runtime dispatch');
		};
		final doesNotReturn = nonReturningFunctionIds != null && nonReturningFunctionIds.exists(targetId);
		final targetName = requireFunctionName(functionNames, targetId, functionId);
		final arguments = call.arguments.map(argument -> requireValue(values, argument, functionId));
		final callExpression:CExpr = ECall(EIdentifier(targetName), arguments);
		addLineDirective(statements, instruction.source, lineDirectives);
		if (call.returnType == IRTVoid) {
			if (instruction.result != null) {
				fail('Void call `${instruction.id}` in `$functionId` unexpectedly defines a value');
			}
			statements.push(SExpr(callExpression));
			return doesNotReturn;
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
		statements.push(SDecl({
			storage: [],
			alignments: [],
			type: cType(result.type),
			declarator: DName(temporaryName),
			initializer: IExpr(callExpression),
			attributes: []
		}));
		values.set(result.id, EIdentifier(temporaryName));
		return doesNotReturn;
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
					case IRTInstance(instanceId): aggregateFieldTypes.get(aggregateFieldKey(instanceId, fieldName));
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

	static function aggregateFieldKey(instanceId:String, fieldName:String):String
		return instanceId + "\x00" + fieldName;

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
