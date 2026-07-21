package reflaxe.c.plan;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import reflaxe.c.frontend.TypedProgramInput.TypedAstField;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyInitializerInput;
import reflaxe.c.lowering.CBodyLowering.CBodyInitializerKind;
import reflaxe.c.lowering.HaxeSourceSpan;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationDependency;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationDependencyReason;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationCyclePolicy;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationOncePolicy;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationPhase;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSource;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationStrategy;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializerKind;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializerRecord;

/** Planner output retained as typed inputs plus a stable inspection snapshot. */
class CStaticInitializationPlanningResult {
	public final initializerInputs:Array<CBodyInitializerInput>;
	public final executionFunctionIds:Array<String>;
	public final snapshot:CStaticInitializationSnapshot;

	public function new(initializerInputs:Array<CBodyInitializerInput>, snapshot:CStaticInitializationSnapshot) {
		this.initializerInputs = initializerInputs.copy();
		this.executionFunctionIds = initializerInputs.map(input -> input.id);
		this.snapshot = snapshot;
	}
}

/** Builds the deterministic eager initialization phase from captured typed modules. */
class CStaticInitializationPlanner {
	public static inline final SCHEMA_VERSION = 1;
	public static inline final STRATEGY = CStaticInitializationStrategy.EagerHaxeTypeOrder;

	final nodesById:Map<String, StaticInitializationTypeNode> = [];
	final dependenciesByPair:Map<String, StaticInitializationEdge> = [];

	public function new() {}

	public function plan(program:TypedProgramInput, entryFunctionId:String):CStaticInitializationPlanningResult {
		collectTypeNodes(program);
		collectDependencies();
		final activeTypeIds = activeTypeClosure();
		final typeOrder = topologicalOrder(activeTypeIds);
		final initializerInputs:Array<CBodyInitializerInput> = [];
		final initializerRecords:Array<CStaticInitializerRecord> = [];

		// The pinned Haxe generators run every class-level __init__ body before
		// assigning any ordinary static field, both in finalized type order.
		for (typeId in typeOrder) {
			final node = requireNode(typeId);
			final expression = node.classType.init;
			if (expression != null) {
				addClassInitializer(node, expression, initializerInputs, initializerRecords);
			}
		}
		for (typeId in typeOrder) {
			final node = requireNode(typeId);
			for (field in physicalStaticFields(node.declaration)) {
				if (field.expression != null) {
					addFieldInitializer(node, field, field.expression, initializerInputs, initializerRecords);
				}
			}
		}

		final dependencies = canonicalDependencies(activeTypeIds);
		final snapshot:CStaticInitializationSnapshot = {
			schemaVersion: SCHEMA_VERSION,
			strategy: STRATEGY,
			phaseOrder: [
				CStaticInitializationPhase.ClassInitializers,
				CStaticInitializationPhase.StaticFieldInitializers,
				CStaticInitializationPhase.Entry
			],
			entryFunctionId: entryFunctionId,
			typeOrder: typeOrder,
			dependencies: dependencies,
			initializers: initializerRecords,
			executionOrder: initializerRecords.map(record -> record.id),
			cyclePolicy: CStaticInitializationCyclePolicy.RejectCrossTypeCycles,
			oncePolicy: CStaticInitializationOncePolicy.HostedExecutableEntry,
			runtimeFeatures: []
		};
		return new CStaticInitializationPlanningResult(initializerInputs, snapshot);
	}

	function collectTypeNodes(program:TypedProgramInput):Void {
		for (declaration in program.declarations) {
			if (declaration.isExtern) {
				continue;
			}
			switch declaration.raw {
				case TClassDecl(reference):
					final classType = reference.get();
					final id = declaration.path;
					if (nodesById.exists(id)) {
						throw new haxe.Exception('static initialization received duplicate class `$id`');
					}
					nodesById.set(id, new StaticInitializationTypeNode(id, declaration, classType));
				case TEnumDecl(_) | TTypeDecl(_) | TAbstract(_):
			}
		}
	}

	function collectDependencies():Void {
		final typeIds = sortedNodeIds();
		for (typeId in typeIds) {
			final node = requireNode(typeId);
			final classType = node.classType;
			if (classType.superClass != null) {
				addClassDependency(node, classType.superClass.t, CStaticInitializationDependencyReason.Inheritance, classType.pos, node.declaration.sourcePath);
			}
			final interfaces = classType.interfaces.copy();
			interfaces.sort((left, right) -> compareUtf8(classPath(left.t.get()), classPath(right.t.get())));
			for (implemented in interfaces) {
				addClassDependency(node, implemented.t, CStaticInitializationDependencyReason.Interface, classType.pos, node.declaration.sourcePath);
			}
			final visitedStaticFields:Map<String, Bool> = [];
			if (classType.init != null) {
				scanExpression(node, classType.init, node.declaration.sourcePath, visitedStaticFields);
			}
			for (field in physicalStaticFields(node.declaration)) {
				if (field.expression != null) {
					scanExpression(node, field.expression, node.declaration.sourcePath, visitedStaticFields);
				}
			}
		}
	}

	function scanExpression(owner:StaticInitializationTypeNode, expression:TypedExpr, sourcePath:String, visitedStaticFields:Map<String, Bool>):Void {
		switch expression.expr {
			case TTypeExpr(TClassDecl(reference)):
				addClassDependency(owner, reference, CStaticInitializationDependencyReason.TypeReference, expression.pos, sourcePath);
			case TField(_, FStatic(classReference, fieldReference)):
				final field = fieldReference.get();
				final reason = switch field.kind {
					case FMethod(_): CStaticInitializationDependencyReason.StaticFunctionReference;
					case FVar(_, _): CStaticInitializationDependencyReason.StaticFieldReference;
				};
				addClassDependency(owner, classReference, reason, expression.pos, sourcePath);
				scanReferencedStaticField(owner, classReference, fieldReference, visitedStaticFields);
			case TNew(classReference, _, _):
				addClassDependency(owner, classReference, CStaticInitializationDependencyReason.ConstructorReference, expression.pos, sourcePath);
				scanConstructorChain(owner, classReference, visitedStaticFields);
			case _:
		}
		TypedExprTools.iter(expression, child -> scanExpression(owner, child, sourcePath, visitedStaticFields));
	}

	function scanReferencedStaticField(owner:StaticInitializationTypeNode, classReference:Ref<ClassType>, fieldReference:Ref<ClassField>,
			visitedStaticFields:Map<String, Bool>):Void {
		final classType = classReference.get();
		final field = fieldReference.get();
		final key = classPath(classType) + "\x00" + field.name;
		if (visitedStaticFields.exists(key)) {
			return;
		}
		visitedStaticFields.set(key, true);
		final body = field.expr();
		if (body != null) {
			final targetNode = nodesById.get(classPath(classType));
			final sourcePath = targetNode == null ? moduleSourcePath(classType.module) : targetNode.declaration.sourcePath;
			scanExpression(owner, body, sourcePath, visitedStaticFields);
		}
	}

	function scanConstructorChain(owner:StaticInitializationTypeNode, initialReference:Ref<ClassType>, visitedStaticFields:Map<String, Bool>):Void {
		var current:Null<ClassType> = initialReference.get();
		while (current != null) {
			final classType:ClassType = current;
			final key = classPath(classType) + "\x00new";
			if (visitedStaticFields.exists(key)) {
				return;
			}
			visitedStaticFields.set(key, true);
			if (classType.constructor != null) {
				final body = classType.constructor.get().expr();
				if (body != null) {
					final targetNode = nodesById.get(classPath(classType));
					final sourcePath = targetNode == null ? moduleSourcePath(classType.module) : targetNode.declaration.sourcePath;
					scanExpression(owner, body, sourcePath, visitedStaticFields);
				}
			}
			current = classType.superClass == null ? null : classType.superClass.t.get();
		}
	}

	function addClassDependency(owner:StaticInitializationTypeNode, prerequisiteReference:Ref<ClassType>, reason:CStaticInitializationDependencyReason,
			position:Position, sourcePath:String):Void {
		final prerequisiteId = classPath(prerequisiteReference.get());
		if (prerequisiteId == owner.id || !nodesById.exists(prerequisiteId)) {
			return;
		}
		final key = owner.id + "\x00" + prerequisiteId;
		if (dependenciesByPair.exists(key)) {
			return;
		}
		dependenciesByPair.set(key,
			new StaticInitializationEdge(owner.id, prerequisiteId, reason, position, HaxeSourceSpan.fromPosition(position, sourcePath)));
	}

	function activeTypeClosure():Map<String, Bool> {
		final active:Map<String, Bool> = [];
		final pending:Array<String> = [];
		for (typeId in sortedNodeIds()) {
			if (hasInitializer(requireNode(typeId))) {
				active.set(typeId, true);
				pending.push(typeId);
			}
		}
		var index = 0;
		while (index < pending.length) {
			final currentId = pending[index++];
			for (edge in outgoingEdges(currentId)) {
				if (!active.exists(edge.prerequisiteTypeId)) {
					active.set(edge.prerequisiteTypeId, true);
					pending.push(edge.prerequisiteTypeId);
				}
			}
		}
		return active;
	}

	function topologicalOrder(active:Map<String, Bool>):Array<String> {
		final states:Map<String, Int> = [];
		final nodeStack:Array<String> = [];
		final edgeStack:Array<StaticInitializationEdge> = [];
		final order:Array<String> = [];
		final roots = [for (typeId in active.keys()) typeId];
		roots.sort(compareUtf8);
		for (root in roots) {
			visit(root, active, states, nodeStack, edgeStack, order);
		}
		return order;
	}

	function visit(typeId:String, active:Map<String, Bool>, states:Map<String, Int>, nodeStack:Array<String>, edgeStack:Array<StaticInitializationEdge>,
			order:Array<String>):Void {
		final state = states.get(typeId);
		if (state == 2) {
			return;
		}
		states.set(typeId, 1);
		nodeStack.push(typeId);
		for (edge in outgoingEdges(typeId)) {
			if (!active.exists(edge.prerequisiteTypeId)) {
				continue;
			}
			final prerequisiteState = states.get(edge.prerequisiteTypeId);
			if (prerequisiteState == 1) {
				throwCycle(edge, nodeStack, edgeStack);
			}
			if (prerequisiteState != 2) {
				edgeStack.push(edge);
				visit(edge.prerequisiteTypeId, active, states, nodeStack, edgeStack, order);
				edgeStack.pop();
			}
		}
		nodeStack.pop();
		states.set(typeId, 2);
		order.push(typeId);
	}

	function throwCycle(closingEdge:StaticInitializationEdge, nodeStack:Array<String>, edgeStack:Array<StaticInitializationEdge>):Void {
		var start = 0;
		for (index in 0...nodeStack.length) {
			if (nodeStack[index] == closingEdge.prerequisiteTypeId) {
				start = index;
				break;
			}
		}
		final cycleEdges:Array<StaticInitializationEdge> = [];
		for (index in start...edgeStack.length) {
			cycleEdges.push(edgeStack[index]);
		}
		cycleEdges.push(closingEdge);
		final segments = cycleEdges.map(edge -> '${edge.dependentTypeId} --[${edge.reason} at ${edge.source.display()}]--> ');
		final detail = "static initialization cycle: " + segments.join("") + closingEdge.prerequisiteTypeId;
		throw new CStaticInitializationError(detail, closingEdge.position);
	}

	function canonicalDependencies(active:Map<String, Bool>):Array<CStaticInitializationDependency> {
		final edges = [for (edge in dependenciesByPair) edge];
		edges.sort(compareEdges);
		final result:Array<CStaticInitializationDependency> = [];
		for (edge in edges) {
			if (active.exists(edge.dependentTypeId) && active.exists(edge.prerequisiteTypeId)) {
				result.push({
					dependentTypeId: edge.dependentTypeId,
					prerequisiteTypeId: edge.prerequisiteTypeId,
					reason: edge.reason,
					source: sourceRecord(edge.source)
				});
			}
		}
		return result;
	}

	function outgoingEdges(typeId:String):Array<StaticInitializationEdge> {
		final result:Array<StaticInitializationEdge> = [];
		for (edge in dependenciesByPair) {
			if (edge.dependentTypeId == typeId) {
				result.push(edge);
			}
		}
		result.sort(compareEdges);
		return result;
	}

	function addClassInitializer(node:StaticInitializationTypeNode, expression:TypedExpr, inputs:Array<CBodyInitializerInput>,
			records:Array<CStaticInitializerRecord>):Void {
		final id = classInitializerId(node.id);
		final source = HaxeSourceSpan.fromPosition(expression.pos, node.declaration.sourcePath);
		inputs.push({
			id: id,
			modulePath: node.declaration.ownerModulePath,
			declarationPath: node.id,
			readableDeclarationPath: readableDeclarationPath(node.declaration),
			sourcePath: node.declaration.sourcePath,
			displayName: "class-init",
			sourceOrder: 0,
			expression: expression,
			kind: CBIClass
		});
		records.push({
			id: id,
			functionId: id,
			modulePath: node.declaration.ownerModulePath,
			typeId: node.id,
			kind: CStaticInitializerKind.ClassInitializer,
			fieldName: null,
			sourceOrder: 0,
			source: sourceRecord(source)
		});
	}

	function addFieldInitializer(node:StaticInitializationTypeNode, field:TypedAstField, expression:TypedExpr, inputs:Array<CBodyInitializerInput>,
			records:Array<CStaticInitializerRecord>):Void {
		final id = fieldInitializerId(node.id, field.name);
		final source = HaxeSourceSpan.fromPosition(expression.pos, node.declaration.sourcePath);
		inputs.push({
			id: id,
			modulePath: node.declaration.ownerModulePath,
			declarationPath: node.id,
			readableDeclarationPath: readableDeclarationPath(node.declaration),
			sourcePath: node.declaration.sourcePath,
			displayName: 'static-field-${field.name}',
			sourceOrder: field.sourceOrder + 1,
			expression: expression,
			kind: CBIStaticField(CBodyLowering.globalId(node.id, field.name))
		});
		records.push({
			id: id,
			functionId: id,
			modulePath: node.declaration.ownerModulePath,
			typeId: node.id,
			kind: CStaticInitializerKind.StaticFieldInitializer,
			fieldName: field.name,
			sourceOrder: field.sourceOrder,
			source: sourceRecord(source)
		});
	}

	function hasInitializer(node:StaticInitializationTypeNode):Bool {
		if (node.classType.init != null) {
			return true;
		}
		for (field in physicalStaticFields(node.declaration)) {
			if (field.expression != null) {
				return true;
			}
		}
		return false;
	}

	static function physicalStaticFields(declaration:TypedAstDeclaration):Array<TypedAstField> {
		final result:Array<TypedAstField> = [];
		for (field in declaration.fields) {
			if (field.role != "static" || field.isExtern || field.rawClassField == null) {
				continue;
			}
			switch field.rawClassField.kind {
				case FVar(AccInline, _):
				case FVar(_, _):
					result.push(field);
				case FMethod(_):
			}
		}
		result.sort((left, right) -> {
			final order = left.sourceOrder - right.sourceOrder;
			return order != 0 ? order : compareUtf8(left.name, right.name);
		});
		return result;
	}

	static function readableDeclarationPath(declaration:TypedAstDeclaration):Null<String>
		return declaration.classKind == "module-fields" ? declaration.ownerModulePath : null;

	function sortedNodeIds():Array<String> {
		final result = [for (id in nodesById.keys()) id];
		result.sort(compareUtf8);
		return result;
	}

	function requireNode(id:String):StaticInitializationTypeNode {
		final node = nodesById.get(id);
		if (node == null) {
			throw new haxe.Exception('static initialization lost type node `$id`');
		}
		return node;
	}

	public static function classInitializerId(typeId:String):String
		return 'initializer.$typeId.class';

	public static function fieldInitializerId(typeId:String, fieldName:String):String
		return 'initializer.$typeId.field.$fieldName';

	static function classPath(classType:ClassType):String
		return classType.pack.concat([classType.name]).join(".");

	static function moduleSourcePath(modulePath:String):String
		return modulePath.split(".").join("/") + ".hx";

	static function sourceRecord(source:HxcSourceSpan):CStaticInitializationSource
		return {
			file: source.file,
			startLine: source.startLine,
			startColumn: source.startColumn,
			endLine: source.endLine,
			endColumn: source.endColumn
		};

	static function compareEdges(left:StaticInitializationEdge, right:StaticInitializationEdge):Int {
		final identity = compareUtf8(left.dependentTypeId + "\x00" + left.prerequisiteTypeId, right.dependentTypeId + "\x00" + right.prerequisiteTypeId);
		if (identity != 0) {
			return identity;
		}
		final reason = compareUtf8(left.reason, right.reason);
		return reason != 0 ? reason : compareUtf8(left.source.display(), right.source.display());
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
}

private class StaticInitializationTypeNode {
	public final id:String;
	public final declaration:TypedAstDeclaration;
	public final classType:ClassType;

	public function new(id:String, declaration:TypedAstDeclaration, classType:ClassType) {
		this.id = id;
		this.declaration = declaration;
		this.classType = classType;
	}
}

private class StaticInitializationEdge {
	public final dependentTypeId:String;
	public final prerequisiteTypeId:String;
	public final reason:CStaticInitializationDependencyReason;
	public final position:Position;
	public final source:HxcSourceSpan;

	public function new(dependentTypeId:String, prerequisiteTypeId:String, reason:CStaticInitializationDependencyReason, position:Position,
			source:HxcSourceSpan) {
		this.dependentTypeId = dependentTypeId;
		this.prerequisiteTypeId = prerequisiteTypeId;
		this.reason = reason;
		this.position = position;
		this.source = source;
	}
}
#else
class CStaticInitializationPlanner {
	public function new() {}
}
#end
