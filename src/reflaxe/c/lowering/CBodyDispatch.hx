package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyClass.CPreparedBodyClass;
import reflaxe.c.lowering.CBodyInterface.CPreparedBodyInterface;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CGenericSpecialization.CGenericFunctionSpecialization;
import reflaxe.c.lowering.CGenericSpecialization.CGenericSpecializationReason;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeArgument;
import reflaxe.c.lowering.CGenericSpecialization.CGenericCallResolver;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/** Module anchor for reachable program-local virtual dispatch. */
class CBodyDispatch {
	private function new() {}
}

/** One typed instance-method access before call arguments are lowered. */
typedef CBodyInstanceCallAccess = {
	final receiver:TypedExpr;
	final owner:Ref<ClassType>;
	final field:Ref<ClassField>;
	final calleeType:Type;
}

/** Joins direct instance-call discovery with the shared specialization owner. */
typedef CBodyInstanceMethodSpecializer = (CBodyFunctionInput, Array<CGenericTypeArgument>, CGenericSpecializationReason) -> CBodyFunctionInput;

enum CBodyDispatchCallKind {
	CBDDirect(targetFunctionId:String, reason:String);
	CBDVirtual(slotId:String, reason:String);
	CBDInterface(slotId:String, reason:String);
}

/** Source-rooted explanation for one reachable instance call. */
class CBodyDispatchCallInput {
	public final callerFunctionId:String;
	public final methodFunctionId:String;
	public final receiverStaticClass:String;
	public final kind:CBodyDispatchCallKind;
	public final source:HxcSourceSpan;

	public function new(callerFunctionId:String, methodFunctionId:String, receiverStaticClass:String, kind:CBodyDispatchCallKind, source:HxcSourceSpan) {
		this.callerFunctionId = callerFunctionId;
		this.methodFunctionId = methodFunctionId;
		this.receiverStaticClass = receiverStaticClass;
		this.kind = kind;
		this.source = source;
	}
}

/** One source-level virtual slot before target representation validation. */
class CBodyVirtualSlotInput {
	public final id:String;
	public final layoutId:String;
	public final rootClass:Ref<ClassType>;
	public final owner:Ref<ClassType>;
	public final field:ClassField;
	public final sourcePath:String;
	public final implementationIds:Map<String, Bool> = [];

	public function new(id:String, layoutId:String, rootClass:Ref<ClassType>, owner:Ref<ClassType>, field:ClassField, sourcePath:String) {
		this.id = id;
		this.layoutId = layoutId;
		this.rootClass = rootClass;
		this.owner = owner;
		this.field = field;
		this.sourcePath = sourcePath;
	}
}

typedef CBodyVirtualTableEntryInput = {
	final slotId:String;
	final implementation:Null<CBodyFunctionInput>;
	final required:Bool;
}

/** One selected dynamic class table before C names are finalized. */
class CBodyVirtualTableInput {
	public final id:String;
	public final layoutId:String;
	public final classReference:Ref<ClassType>;
	public final rootClass:Ref<ClassType>;
	public final sourcePath:String;
	public final entries:Array<CBodyVirtualTableEntryInput>;

	public function new(id:String, layoutId:String, classReference:Ref<ClassType>, rootClass:Ref<ClassType>, sourcePath:String,
			entries:Array<CBodyVirtualTableEntryInput>) {
		this.id = id;
		this.layoutId = layoutId;
		this.classReference = classReference;
		this.rootClass = rootClass;
		this.sourcePath = sourcePath;
		this.entries = entries.copy();
	}
}

/** Immutable reachable dispatch input consumed by typed body lowering. */
class CBodyDispatchGraph {
	public final slots:Array<CBodyVirtualSlotInput>;
	public final tables:Array<CBodyVirtualTableInput>;
	public final calls:Array<CBodyDispatchCallInput>;

	final slotsByMethodId:Map<String, CBodyVirtualSlotInput>;

	public function new(slots:Array<CBodyVirtualSlotInput>, tables:Array<CBodyVirtualTableInput>, calls:Array<CBodyDispatchCallInput>,
			slotsByMethodId:Map<String, CBodyVirtualSlotInput>) {
		this.slots = slots.copy();
		this.tables = tables.copy();
		this.calls = calls.copy();
		this.slotsByMethodId = slotsByMethodId;
	}

	public function slotForMethodId(methodId:String):Null<CBodyVirtualSlotInput>
		return slotsByMethodId.get(methodId);

	public static function empty():CBodyDispatchGraph
		return new CBodyDispatchGraph([], [], [], []);
}

/**
	Reachable method/table discovery. It deliberately does no C representation
	work; the lowering-side preparer validates every selected override afterward.
**/
class CBodyDispatchCatalog {
	final context:CompilationContext;
	final methodsById:Map<String, CBodyFunctionInput> = [];
	final sourcePathsByClass:Map<String, String> = [];
	final constructedClasses:Map<String, Ref<ClassType>> = [];
	final slotsById:Map<String, CBodyVirtualSlotInput> = [];
	final slotsByMethodId:Map<String, CBodyVirtualSlotInput> = [];
	final calls:Array<CBodyDispatchCallInput> = [];

	public function new(context:CompilationContext, program:TypedProgramInput) {
		this.context = context;
		indexProgram(program);
	}

	public function markConstructed(reference:Ref<ClassType>):Array<CBodyFunctionInput> {
		final path = classPath(reference.get());
		if (!constructedClasses.exists(path))
			constructedClasses.set(path, reference);
		return selectedImplementations();
	}

	public function collectCall(expression:TypedExpr, callerFunctionId:String, callerSourcePath:String,
			callerSpecialization:Null<CGenericFunctionSpecialization>, specialize:CBodyInstanceMethodSpecializer):Array<CBodyFunctionInput> {
		final call = switch expression.expr {
			case TCall(callee, arguments):
				final access = instanceAccess(callee);
				access == null ? null : {access: access, arguments: arguments};
			case _: null;
		};
		if (call == null)
			return [];
		final access = call.access;
		final declaration = declaringClass(access.owner, access.field);
		final field = access.field.get();
		final methodId = CBodyLowering.methodId(classPath(declaration.get()), field.name);
		final staticReceiver = receiverClass(access.receiver);
		final receiverPath = staticReceiver == null ? classPath(declaration.get()) : classPath(staticReceiver.get());
		final source = HaxeSourceSpan.fromPosition(expression.pos, callerSourcePath);
		final directReason = directReason(access.receiver, declaration, field);
		if (directReason != null) {
			var target = methodsById.get(methodId);
			if (target == null)
				unsupportedAt(field.pos, sourcePath(declaration), 'TCall(unavailable-instance-target:$methodId)');
			if (target != null && field.params.length != 0) {
				final resolved = CGenericCallResolver.resolve(methodId, field.type, field.params, access.calleeType,
					call.arguments.map(argument -> argument.t), callerSpecialization, context.profile, expression.pos,
					(position, node) -> unsupportedAt(position, callerSourcePath, node));
				target = specialize(target, resolved.arguments, new CGenericSpecializationReason(callerFunctionId, source, expression.pos));
			}
			final targetId = target == null ? methodId : CBodyLowering.functionInputId(target);
			calls.push(new CBodyDispatchCallInput(callerFunctionId, targetId, receiverPath, CBDDirect(targetId, directReason), source));
			return target == null ? [] : [target];
		}
		if (field.params.length != 0)
			unsupportedAt(field.pos, sourcePath(declaration), 'virtual-slot-generic-requires-specialization:$methodId');
		switch field.kind {
			case FMethod(MethDynamic):
				unsupportedAt(field.pos, sourcePath(declaration), 'TCall(dynamic-instance-method-requires-E3.T08:$methodId)');
			case FMethod(MethMacro):
				unsupportedAt(field.pos, sourcePath(declaration), 'TCall(macro-instance-method-has-no-runtime-target:$methodId)');
			case FMethod(MethNormal) | FMethod(MethInline):
			case FVar(_, _):
				unsupportedAt(field.pos, sourcePath(declaration), 'TCall(instance-field-not-method:$methodId)');
		}
		if (declaration.get().isInterface) {
			final receiverInterface = staticReceiver != null && staticReceiver.get().isInterface ? staticReceiver : declaration;
			final slot = requireInterfaceSlot(receiverInterface, field);
			slotsByMethodId.set(methodId, slot);
			calls.push(new CBodyDispatchCallInput(callerFunctionId, methodId, receiverPath, CBDInterface(slot.id, "ordinary-interface-method"), source));
			return selectedImplementations();
		}
		final slot = requireSlot(declaration, access.field);
		slotsByMethodId.set(methodId, slot);
		calls.push(new CBodyDispatchCallInput(callerFunctionId, methodId, receiverPath, CBDVirtual(slot.id, "ordinary-overridable-instance-method"), source));
		return selectedImplementations();
	}

	public function finish():CBodyDispatchGraph {
		selectedImplementations();
		final slots = [for (slot in slotsById) slot];
		slots.sort((left, right) -> compareUtf8(left.id, right.id));
		final tables:Array<CBodyVirtualTableInput> = [];
		final classPaths = [for (path in constructedClasses.keys()) path];
		classPaths.sort(compareUtf8);
		for (path in classPaths) {
			final classReference = constructedClasses.get(path);
			if (classReference == null)
				continue;
			final root = hierarchyRoot(classReference);
			final layoutId = layoutId(root);
			final layoutSlots = slots.filter(slot -> !slot.owner.get().isInterface && slot.layoutId == layoutId);
			if (layoutSlots.length > 0) {
				final entries:Array<CBodyVirtualTableEntryInput> = [];
				for (slot in layoutSlots) {
					final required = isDescendant(classReference, slot.owner);
					final implementation = required ? effectiveMethod(classReference, slot) : null;
					if (implementation != null) {
						slot.implementationIds.set(CBodyLowering.functionInputId(implementation), true);
						slotsByMethodId.set(CBodyLowering.functionInputId(implementation), slot);
					}
					entries.push({slotId: slot.id, implementation: implementation, required: required});
				}
				tables.push(new CBodyVirtualTableInput(tableId(classReference), layoutId, classReference, root, sourcePath(classReference), entries));
			}
			final interfaceLayouts:Map<String, Array<CBodyVirtualSlotInput>> = [];
			for (slot in slots) {
				if (!slot.owner.get().isInterface || !implementsInterface(classReference, slot.owner))
					continue;
				var interfaceSlots = interfaceLayouts.get(slot.layoutId);
				if (interfaceSlots == null) {
					interfaceSlots = [];
					interfaceLayouts.set(slot.layoutId, interfaceSlots);
				}
				interfaceSlots.push(slot);
			}
			final interfaceLayoutIds = [for (interfaceLayoutId in interfaceLayouts.keys()) interfaceLayoutId];
			interfaceLayoutIds.sort(compareUtf8);
			for (interfaceLayoutId in interfaceLayoutIds) {
				final interfaceSlots = interfaceLayouts.get(interfaceLayoutId);
				if (interfaceSlots == null || interfaceSlots.length == 0)
					continue;
				final interfaceReference = interfaceSlots[0].rootClass;
				final interfaceEntries:Array<CBodyVirtualTableEntryInput> = [];
				for (slot in interfaceSlots) {
					final implementation = effectiveMethod(classReference, slot);
					if (implementation != null) {
						slot.implementationIds.set(CBodyLowering.functionInputId(implementation), true);
						slotsByMethodId.set(CBodyLowering.functionInputId(implementation), slot);
					}
					interfaceEntries.push({slotId: slot.id, implementation: implementation, required: true});
				}
				tables.push(new CBodyVirtualTableInput(interfaceTableId(classReference, interfaceReference), interfaceLayoutId, classReference,
					interfaceReference, sourcePath(classReference), interfaceEntries));
			}
		}
		tables.sort((left, right) -> compareUtf8(left.id, right.id));
		calls.sort(compareCalls);
		return new CBodyDispatchGraph(slots, tables, calls, slotsByMethodId);
	}

	function indexProgram(program:TypedProgramInput):Void {
		for (declaration in program.declarations) {
			final classReference = switch declaration.raw {
				case TClassDecl(reference): reference;
				case _: continue;
			};
			sourcePathsByClass.set(declaration.path, declaration.sourcePath);
			for (field in declaration.fields) {
				if (field.role != "member" || field.expression == null || field.rawClassField == null)
					continue;
				switch field.rawClassField.kind {
					case FMethod(MethNormal) | FMethod(MethInline):
						final input:CBodyFunctionInput = {
							modulePath: declaration.ownerModulePath,
							declarationPath: declaration.path,
							sourcePath: declaration.sourcePath,
							fieldName: field.name,
							sourceOrder: field.sourceOrder,
							fieldType: field.rawClassField.type,
							expression: field.expression,
							typeParameters: field.rawClassField.params,
							specialization: null,
							instanceOwner: classReference
						};
						final id = CBodyLowering.methodId(declaration.path, field.name);
						if (methodsById.exists(id))
							throw new CBodyEmissionError('instance-method discovery received duplicate typed input `$id`');
						methodsById.set(id, input);
					case FMethod(MethDynamic) | FMethod(MethMacro) | FVar(_, _):
				}
			}
		}
	}

	function requireSlot(declaration:Ref<ClassType>, fieldReference:Ref<ClassField>):CBodyVirtualSlotInput {
		final rootMethod = slotRoot(declaration, fieldReference);
		final ownerPath = classPath(rootMethod.owner.get());
		final id = slotId(rootMethod.owner, rootMethod.field.name);
		var slot = slotsById.get(id);
		if (slot == null) {
			final hierarchy = hierarchyRoot(rootMethod.owner);
			slot = new CBodyVirtualSlotInput(id, layoutId(hierarchy), hierarchy, rootMethod.owner, rootMethod.field, sourcePath(rootMethod.owner));
			slotsById.set(id, slot);
		}
		slotsByMethodId.set(CBodyLowering.methodId(ownerPath, rootMethod.field.name), slot);
		return slot;
	}

	/**
		Create a slot for the receiver's interface, including an inherited method.

		A child interface has its own fat-value type and table shape. Repeating an
		inherited method in that shape keeps the receiver identity exact instead of
		pretending that a child-interface value is a parent-interface value.
	**/
	function requireInterfaceSlot(interfaceReference:Ref<ClassType>, field:ClassField):CBodyVirtualSlotInput {
		final interfacePath = classPath(interfaceReference.get());
		final id = interfaceSlotId(interfaceReference, field.name);
		var slot = slotsById.get(id);
		if (slot == null) {
			slot = new CBodyVirtualSlotInput(id, interfaceLayoutId(interfaceReference), interfaceReference, interfaceReference, field,
				sourcePath(interfaceReference));
			slotsById.set(id, slot);
		}
		slotsByMethodId.set(CBodyLowering.methodId(interfacePath, field.name), slot);
		return slot;
	}

	function selectedImplementations():Array<CBodyFunctionInput> {
		final result:Map<String, CBodyFunctionInput> = [];
		for (classReference in constructedClasses) {
			for (slot in slotsById) {
				final applicable = slot.owner.get().isInterface ? implementsInterface(classReference, slot.owner) : isDescendant(classReference, slot.owner);
				if (!applicable)
					continue;
				final implementation = effectiveMethod(classReference, slot);
				if (implementation == null)
					continue;
				final id = CBodyLowering.functionInputId(implementation);
				slot.implementationIds.set(id, true);
				slotsByMethodId.set(id, slot);
				result.set(id, implementation);
			}
		}
		final values = [for (value in result) value];
		values.sort((left, right) -> compareUtf8(CBodyLowering.functionInputId(left), CBodyLowering.functionInputId(right)));
		return values;
	}

	function effectiveMethod(classReference:Ref<ClassType>, slot:CBodyVirtualSlotInput):Null<CBodyFunctionInput> {
		var current:Null<Ref<ClassType>> = classReference;
		while (current != null) {
			final definition = current.get();
			for (field in definition.fields.get()) {
				if (field.name != slot.field.name)
					continue;
				switch field.kind {
					case FMethod(MethNormal) | FMethod(MethInline):
						final id = CBodyLowering.methodId(classPath(definition), field.name);
						final input = methodsById.get(id);
						return input;
					case FMethod(MethDynamic):
						unsupportedAt(field.pos, sourcePath(current), 'virtual-implementation-dynamic-requires-E3.T08:${field.name}');
					case FMethod(MethMacro) | FVar(_, _):
						unsupportedAt(field.pos, sourcePath(current), 'virtual-implementation-not-normal-method:${field.name}');
				}
			}
			if (!slot.owner.get().isInterface && sameClass(current, slot.owner))
				break;
			current = definition.superClass == null ? null : definition.superClass.t;
		}
		return null;
	}

	function slotRoot(owner:Ref<ClassType>, field:Ref<ClassField>):{owner:Ref<ClassType>, field:ClassField} {
		var currentOwner = owner;
		var currentField = field.get();
		while (isOverride(currentOwner, currentField.name)) {
			final inherited = inheritedMethod(currentOwner, currentField.name);
			if (inherited == null)
				break;
			currentOwner = inherited.owner;
			currentField = inherited.field;
		}
		return {owner: currentOwner, field: currentField};
	}

	function inheritedMethod(owner:Ref<ClassType>, name:String):Null<{owner:Ref<ClassType>, field:ClassField}> {
		var current = owner.get().superClass == null ? null : owner.get().superClass.t;
		while (current != null) {
			for (field in current.get().fields.get()) {
				if (field.name == name) {
					return {owner: current, field: field};
				}
			}
			current = current.get().superClass == null ? null : current.get().superClass.t;
		}
		return null;
	}

	static function isOverride(owner:Ref<ClassType>, name:String):Bool {
		for (field in owner.get().overrides) {
			if (field.get().name == name)
				return true;
		}
		return false;
	}

	public static function declaringClass(owner:Ref<ClassType>, fieldReference:Ref<ClassField>):Ref<ClassType> {
		final target = fieldReference.get();
		final interfaceOwner = declaringInterface(owner, target, []);
		if (interfaceOwner != null)
			return interfaceOwner;
		var current:Null<Ref<ClassType>> = owner;
		while (current != null) {
			for (field in current.get().fields.get()) {
				if (field == target)
					return current;
			}
			current = current.get().superClass == null ? null : current.get().superClass.t;
		}
		return owner;
	}

	static function declaringInterface(owner:Ref<ClassType>, target:ClassField, visited:Map<String, Bool>):Null<Ref<ClassType>> {
		final path = classPath(owner.get());
		if (visited.exists(path))
			return null;
		visited.set(path, true);
		if (owner.get().isInterface)
			for (field in owner.get().fields.get())
				if (field == target)
					return owner;
		for (implemented in owner.get().interfaces) {
			final found = declaringInterface(implemented.t, target, visited);
			if (found != null)
				return found;
		}
		return owner.get().superClass == null ? null : declaringInterface(owner.get().superClass.t, target, visited);
	}

	public static function methodIdForAccess(owner:Ref<ClassType>, fieldReference:Ref<ClassField>):String {
		final declaration = declaringClass(owner, fieldReference).get();
		return CBodyLowering.methodId(classPath(declaration), fieldReference.get().name);
	}

	public static function instanceAccess(callee:TypedExpr):Null<CBodyInstanceCallAccess> {
		return switch callee.expr {
			case TField(receiver, FInstance(owner, _, field)): {
					receiver: receiver,
					owner: owner,
					field: field,
					calleeType: callee.t
				};
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): instanceAccess(inner);
			case _: null;
		};
	}

	public static function isSuperReceiver(receiver:TypedExpr):Bool {
		return switch receiver.expr {
			case TConst(TSuper): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isSuperReceiver(inner);
			case _: false;
		};
	}

	public static function directReason(receiver:TypedExpr, owner:Ref<ClassType>, field:ClassField):Null<String> {
		if (isSuperReceiver(receiver))
			return "super-call";
		if (!field.isPublic)
			return "private-method";
		if (field.isFinal)
			return "final-method";
		if (field.meta.has(":nonVirtual"))
			return "non-virtual-metadata";
		switch field.kind {
			case FMethod(MethInline):
				return "inline-method";
			case _:
		}
		final staticClass = receiverClass(receiver);
		if (staticClass != null && staticClass.get().isFinal)
			return "final-receiver-class";
		return owner.get().isFinal ? "final-declaring-class" : null;
	}

	static function receiverClass(expression:TypedExpr):Null<Ref<ClassType>> {
		return switch TypeTools.follow(expression.t) {
			case TInst(reference, _): reference;
			case _: null;
		};
	}

	static function hierarchyRoot(reference:Ref<ClassType>):Ref<ClassType> {
		var current = reference;
		while (current.get().superClass != null)
			current = current.get().superClass.t;
		return current;
	}

	static function isDescendant(candidate:Ref<ClassType>, ancestor:Ref<ClassType>):Bool {
		var current:Null<Ref<ClassType>> = candidate;
		while (current != null) {
			if (sameClass(current, ancestor))
				return true;
			current = current.get().superClass == null ? null : current.get().superClass.t;
		}
		return false;
	}

	static function implementsInterface(candidate:Ref<ClassType>, target:Ref<ClassType>):Bool
		return implementsInterfacePath(candidate, classPath(target.get()), []);

	static function implementsInterfacePath(candidate:Ref<ClassType>, targetPath:String, visited:Map<String, Bool>):Bool {
		final candidatePath = classPath(candidate.get());
		if (visited.exists(candidatePath))
			return false;
		visited.set(candidatePath, true);
		if (candidatePath == targetPath)
			return true;
		for (implemented in candidate.get().interfaces)
			if (implementsInterfacePath(implemented.t, targetPath, visited))
				return true;
		return candidate.get().superClass != null && implementsInterfacePath(candidate.get().superClass.t, targetPath, visited);
	}

	static function sameClass(left:Ref<ClassType>, right:Ref<ClassType>):Bool
		return classPath(left.get()) == classPath(right.get());

	static function classPath(value:ClassType):String
		return value.pack.concat([value.name]).join(".");

	function sourcePath(reference:Ref<ClassType>):String {
		final path = classPath(reference.get());
		final source = sourcePathsByClass.get(path);
		return source == null ? reference.get().module.split(".").join("/") + ".hx" : source;
	}

	public static function slotId(owner:Ref<ClassType>, name:String):String
		return 'slot.${classPath(owner.get())}.$name';

	public static function layoutId(root:Ref<ClassType>):String
		return 'vtable.layout.${classPath(root.get())}';

	public static function tableId(classReference:Ref<ClassType>):String
		return 'vtable.${classPath(classReference.get())}';

	public static function interfaceSlotId(interfaceReference:Ref<ClassType>, name:String):String
		return 'interface-slot.${classPath(interfaceReference.get())}.$name';

	public static function interfaceLayoutId(interfaceReference:Ref<ClassType>):String
		return 'itable.layout.${classPath(interfaceReference.get())}';

	public static function interfaceTableId(classReference:Ref<ClassType>, interfaceReference:Ref<ClassType>):String
		return 'itable.${classPath(classReference.get())}.${classPath(interfaceReference.get())}';

	function unsupportedAt<T>(position:Position, sourcePath:String, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, "reachable virtual-dispatch graph discovery",
			source), position);
	}

	static function compareCalls(left:CBodyDispatchCallInput, right:CBodyDispatchCallInput):Int {
		final file = compareUtf8(left.source.file, right.source.file);
		if (file != 0)
			return file;
		if (left.source.startLine != right.source.startLine)
			return left.source.startLine - right.source.startLine;
		if (left.source.startColumn != right.source.startColumn)
			return left.source.startColumn - right.source.startColumn;
		if (left.source.endLine != right.source.endLine)
			return left.source.endLine - right.source.endLine;
		if (left.source.endColumn != right.source.endColumn)
			return left.source.endColumn - right.source.endColumn;
		final caller = compareUtf8(left.callerFunctionId, right.callerFunctionId);
		return caller != 0 ? caller : compareUtf8(left.methodFunctionId, right.methodFunctionId);
	}

	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}

/** One representation-checked virtual slot before C names are finalized. */
class CPreparedVirtualSlot {
	public final input:CBodyVirtualSlotInput;
	public final ownerClass:Null<CPreparedBodyClass>;
	public final ownerInterface:Null<CPreparedBodyInterface>;
	public final parameters:Array<CBodyValueType>;
	public final returnType:CBodyValueType;
	public final memberRequest:CSymbolRequest;

	public function new(input:CBodyVirtualSlotInput, ownerClass:Null<CPreparedBodyClass>, ownerInterface:Null<CPreparedBodyInterface>,
			parameters:Array<CBodyValueType>, returnType:CBodyValueType, memberRequest:CSymbolRequest) {
		this.input = input;
		this.ownerClass = ownerClass;
		this.ownerInterface = ownerInterface;
		this.parameters = parameters.copy();
		this.returnType = returnType;
		this.memberRequest = memberRequest;
	}

	public function ownerInstanceId():String {
		if (ownerClass != null)
			return ownerClass.instanceId;
		if (ownerInterface != null)
			return ownerInterface.instanceId;
		throw new CBodyEmissionError('dispatch slot `${input.id}` lost its owner');
	}

	public function isInterface():Bool
		return ownerInterface != null;

	public function ir():HxcIRVirtualSlot
		return {
			id: input.id,
			ownerInstanceId: ownerInstanceId(),
			parameterTypes: parameters.map(value -> value.irType),
			returnType: returnType.irType,
			source: HaxeSourceSpan.fromPosition(input.field.pos, input.sourcePath)
		};
}

/** One hierarchy-wide table shape before tag finalization. */
class CPreparedVirtualLayout {
	public final id:String;
	public final rootClass:Null<CPreparedBodyClass>;
	public final rootInterface:Null<CPreparedBodyInterface>;
	public final source:HxcSourceSpan;
	public final tagRequest:CSymbolRequest;
	public final valueTagRequest:Null<CSymbolRequest>;
	public final objectMemberRequest:Null<CSymbolRequest>;
	public final tableMemberRequest:Null<CSymbolRequest>;
	public final slots:Array<CPreparedVirtualSlot>;

	public function new(id:String, rootClass:Null<CPreparedBodyClass>, rootInterface:Null<CPreparedBodyInterface>, source:HxcSourceSpan,
			tagRequest:CSymbolRequest, valueTagRequest:Null<CSymbolRequest>, objectMemberRequest:Null<CSymbolRequest>,
			tableMemberRequest:Null<CSymbolRequest>, slots:Array<CPreparedVirtualSlot>) {
		this.id = id;
		this.rootClass = rootClass;
		this.rootInterface = rootInterface;
		this.source = source;
		this.tagRequest = tagRequest;
		this.valueTagRequest = valueTagRequest;
		this.objectMemberRequest = objectMemberRequest;
		this.tableMemberRequest = tableMemberRequest;
		this.slots = slots.copy();
	}

	public function rootInstanceId():String {
		if (rootClass != null)
			return rootClass.instanceId;
		if (rootInterface != null)
			return rootInterface.instanceId;
		throw new CBodyEmissionError('dispatch layout `$id` lost its root');
	}

	public function isInterface():Bool
		return rootInterface != null;

	public function ir():HxcIRVirtualTableLayout
		return {
			id: id,
			rootInstanceId: rootInstanceId(),
			slotIds: slots.map(slot -> slot.input.id),
			source: source
		};
}

/** Adapter from one slot receiver type to one exact overriding implementation. */
class CPreparedVirtualThunk {
	public final id:String;
	public final slot:CPreparedVirtualSlot;
	public final implementationFunctionId:String;
	public final implementationOwner:CPreparedBodyClass;
	public final functionRequest:CSymbolRequest;
	public final receiverRequest:CSymbolRequest;
	public final argumentRequests:Array<CSymbolRequest>;

	public function new(id:String, slot:CPreparedVirtualSlot, implementationFunctionId:String, implementationOwner:CPreparedBodyClass,
			functionRequest:CSymbolRequest, receiverRequest:CSymbolRequest, argumentRequests:Array<CSymbolRequest>) {
		this.id = id;
		this.slot = slot;
		this.implementationFunctionId = implementationFunctionId;
		this.implementationOwner = implementationOwner;
		this.functionRequest = functionRequest;
		this.receiverRequest = receiverRequest;
		this.argumentRequests = argumentRequests.copy();
	}
}

class CPreparedVirtualTableEntry {
	public final slot:CPreparedVirtualSlot;
	public final implementationFunctionId:Null<String>;
	public final implementationOwner:Null<CPreparedBodyClass>;
	public final thunk:Null<CPreparedVirtualThunk>;

	public function new(slot:CPreparedVirtualSlot, implementationFunctionId:Null<String>, implementationOwner:Null<CPreparedBodyClass>,
			thunk:Null<CPreparedVirtualThunk>) {
		this.slot = slot;
		this.implementationFunctionId = implementationFunctionId;
		this.implementationOwner = implementationOwner;
		this.thunk = thunk;
	}

	public function ir():HxcIRVirtualTableEntry
		return {slotId: slot.input.id, implementationFunctionId: implementationFunctionId};
}

class CPreparedVirtualTable {
	public final input:CBodyVirtualTableInput;
	public final layout:CPreparedVirtualLayout;
	public final classValue:CPreparedBodyClass;
	public final source:HxcSourceSpan;
	public final tableRequest:CSymbolRequest;
	public final entries:Array<CPreparedVirtualTableEntry>;

	public function new(input:CBodyVirtualTableInput, layout:CPreparedVirtualLayout, classValue:CPreparedBodyClass, source:HxcSourceSpan,
			tableRequest:CSymbolRequest, entries:Array<CPreparedVirtualTableEntry>) {
		this.input = input;
		this.layout = layout;
		this.classValue = classValue;
		this.source = source;
		this.tableRequest = tableRequest;
		this.entries = entries.copy();
	}

	public function ir():HxcIRVirtualTable
		return {
			id: input.id,
			layoutId: layout.id,
			classInstanceId: classValue.instanceId,
			entries: entries.map(entry -> entry.ir()),
			source: source
		};
}

/** Complete typed dispatch plan before global symbol finalization. */
class CPreparedBodyDispatch {
	public final graph:CBodyDispatchGraph;
	public final layouts:Array<CPreparedVirtualLayout>;
	public final slots:Array<CPreparedVirtualSlot>;
	public final tables:Array<CPreparedVirtualTable>;
	public final thunks:Array<CPreparedVirtualThunk>;

	final slotsByMethodId:Map<String, CPreparedVirtualSlot>;
	final tablesByInstanceAndLayout:Map<String, CPreparedVirtualTable>;

	public function new(graph:CBodyDispatchGraph, layouts:Array<CPreparedVirtualLayout>, slots:Array<CPreparedVirtualSlot>,
			tables:Array<CPreparedVirtualTable>, thunks:Array<CPreparedVirtualThunk>, slotsByMethodId:Map<String, CPreparedVirtualSlot>,
			tablesByInstanceAndLayout:Map<String, CPreparedVirtualTable>) {
		this.graph = graph;
		this.layouts = layouts.copy();
		this.slots = slots.copy();
		this.tables = tables.copy();
		this.thunks = thunks.copy();
		this.slotsByMethodId = slotsByMethodId;
		this.tablesByInstanceAndLayout = tablesByInstanceAndLayout;
	}

	public function slotForMethodId(methodId:String):Null<CPreparedVirtualSlot>
		return slotsByMethodId.get(methodId);

	public function slotForInterface(interfaceInstanceId:String, fieldName:String):Null<CPreparedVirtualSlot> {
		for (slot in slots)
			if (slot.ownerInstanceId() == interfaceInstanceId && slot.input.field.name == fieldName)
				return slot;
		return null;
	}

	public function tableForInstance(instanceId:String):Null<CPreparedVirtualTable>
		return tablesByInstanceAndLayout.get(tableLookupKey(instanceId, null));

	public function tableForInterface(classInstanceId:String, interfaceInstanceId:String):Null<CPreparedVirtualTable>
		return tablesByInstanceAndLayout.get(tableLookupKey(classInstanceId, interfaceInstanceId));

	static function tableLookupKey(classInstanceId:String, interfaceInstanceId:Null<String>):String
		return interfaceInstanceId == null ? 'class:$classInstanceId' : 'interface:$classInstanceId:$interfaceInstanceId';

	public function ir():HxcIRDispatchPlan
		return {
			layouts: layouts.map(layout -> layout.ir()),
			slots: slots.map(slot -> slot.ir()),
			tables: tables.map(table -> table.ir())
		};

	public function finalize(symbols:CSymbolRegistry):CLoweredBodyDispatch {
		final loweredSlots:Map<String, CLoweredVirtualSlot> = [];
		final slotValues:Array<CLoweredVirtualSlot> = [];
		for (slot in slots) {
			final value = new CLoweredVirtualSlot(slot, symbols.identifierFor(slot.memberRequest));
			loweredSlots.set(slot.input.id, value);
			slotValues.push(value);
		}
		final loweredLayouts:Map<String, CLoweredVirtualLayout> = [];
		final layoutValues:Array<CLoweredVirtualLayout> = [];
		for (layout in layouts) {
			final value = new CLoweredVirtualLayout(layout, symbols.identifierFor(layout.tagRequest),
				layout.valueTagRequest == null ? null : symbols.identifierFor(layout.valueTagRequest),
				layout.objectMemberRequest == null ? null : symbols.identifierFor(layout.objectMemberRequest),
				layout.tableMemberRequest == null ? null : symbols.identifierFor(layout.tableMemberRequest),
				layout.slots.map(slot -> requireLoweredSlot(loweredSlots, slot.input.id)));
			loweredLayouts.set(layout.id, value);
			layoutValues.push(value);
		}
		final loweredThunks:Map<String, CLoweredVirtualThunk> = [];
		final thunkValues:Array<CLoweredVirtualThunk> = [];
		for (thunk in thunks) {
			final value = new CLoweredVirtualThunk(thunk, symbols.identifierFor(thunk.functionRequest), symbols.identifierFor(thunk.receiverRequest),
				thunk.argumentRequests.map(request -> symbols.identifierFor(request)));
			loweredThunks.set(thunk.id, value);
			thunkValues.push(value);
		}
		final tableValues:Array<CLoweredVirtualTable> = [];
		for (table in tables) {
			final layout = loweredLayouts.get(table.layout.id);
			if (layout == null)
				throw new CBodyEmissionError('dispatch finalization lost layout `${table.layout.id}`');
			tableValues.push(new CLoweredVirtualTable(table, layout, symbols.identifierFor(table.tableRequest), table.entries.map(entry -> {
				slot: requireLoweredSlot(loweredSlots, entry.slot.input.id),
				implementationFunctionId: entry.implementationFunctionId,
				implementationOwner: entry.implementationOwner,
				thunk: entry.thunk == null ? null : loweredThunks.get(entry.thunk.id)
			})));
		}
		return new CLoweredBodyDispatch(this, layoutValues, slotValues, tableValues, thunkValues);
	}

	static function requireLoweredSlot(values:Map<String, CLoweredVirtualSlot>, id:String):CLoweredVirtualSlot {
		final value = values.get(id);
		if (value == null)
			throw new CBodyEmissionError('dispatch finalization lost slot `$id`');
		return value;
	}
}

class CLoweredVirtualSlot {
	public final prepared:CPreparedVirtualSlot;
	public final cMember:CIdentifier;

	public function new(prepared:CPreparedVirtualSlot, cMember:CIdentifier) {
		this.prepared = prepared;
		this.cMember = cMember;
	}
}

class CLoweredVirtualLayout {
	public final prepared:CPreparedVirtualLayout;
	public final cTag:CIdentifier;
	public final cValueTag:Null<CIdentifier>;
	public final cObjectMember:Null<CIdentifier>;
	public final cTableMember:Null<CIdentifier>;
	public final slots:Array<CLoweredVirtualSlot>;

	public function new(prepared:CPreparedVirtualLayout, cTag:CIdentifier, cValueTag:Null<CIdentifier>, cObjectMember:Null<CIdentifier>,
			cTableMember:Null<CIdentifier>, slots:Array<CLoweredVirtualSlot>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.cValueTag = cValueTag;
		this.cObjectMember = cObjectMember;
		this.cTableMember = cTableMember;
		this.slots = slots.copy();
	}
}

class CLoweredVirtualThunk {
	public final prepared:CPreparedVirtualThunk;
	public final cName:CIdentifier;
	public final receiverName:CIdentifier;
	public final argumentNames:Array<CIdentifier>;

	public function new(prepared:CPreparedVirtualThunk, cName:CIdentifier, receiverName:CIdentifier, argumentNames:Array<CIdentifier>) {
		this.prepared = prepared;
		this.cName = cName;
		this.receiverName = receiverName;
		this.argumentNames = argumentNames.copy();
	}
}

typedef CLoweredVirtualTableEntry = {
	final slot:CLoweredVirtualSlot;
	final implementationFunctionId:Null<String>;
	final implementationOwner:Null<CPreparedBodyClass>;
	final thunk:Null<CLoweredVirtualThunk>;
}

class CLoweredVirtualTable {
	public final prepared:CPreparedVirtualTable;
	public final layout:CLoweredVirtualLayout;
	public final cName:CIdentifier;
	public final entries:Array<CLoweredVirtualTableEntry>;

	public function new(prepared:CPreparedVirtualTable, layout:CLoweredVirtualLayout, cName:CIdentifier, entries:Array<CLoweredVirtualTableEntry>) {
		this.prepared = prepared;
		this.layout = layout;
		this.cName = cName;
		this.entries = entries.copy();
	}
}

/** Finalized identifiers plus exact semantic dispatch facts for C emission/reporting. */
class CLoweredBodyDispatch {
	public final prepared:CPreparedBodyDispatch;
	public final layouts:Array<CLoweredVirtualLayout>;
	public final slots:Array<CLoweredVirtualSlot>;
	public final tables:Array<CLoweredVirtualTable>;
	public final thunks:Array<CLoweredVirtualThunk>;

	public function new(prepared:CPreparedBodyDispatch, layouts:Array<CLoweredVirtualLayout>, slots:Array<CLoweredVirtualSlot>,
			tables:Array<CLoweredVirtualTable>, thunks:Array<CLoweredVirtualThunk>) {
		this.prepared = prepared;
		this.layouts = layouts.copy();
		this.slots = slots.copy();
		this.tables = tables.copy();
		this.thunks = thunks.copy();
	}
}

/** Maps selected source signatures into exact HxcIR/C representations. */
class CBodyDispatchPreparer {
	final context:CompilationContext;
	final graph:CBodyDispatchGraph;
	final aggregates:CBodyAggregateRegistry;

	public function new(context:CompilationContext, graph:CBodyDispatchGraph, aggregates:CBodyAggregateRegistry) {
		this.context = context;
		this.graph = graph;
		this.aggregates = aggregates;
	}

	public function prepare():CPreparedBodyDispatch {
		final slots:Array<CPreparedVirtualSlot> = [];
		final slotsById:Map<String, CPreparedVirtualSlot> = [];
		final slotsByMethodId:Map<String, CPreparedVirtualSlot> = [];
		final layoutInputs:Map<String, Array<CPreparedVirtualSlot>> = [];
		for (input in graph.slots) {
			if (input.owner.get().params.length != 0 || input.field.params.length != 0)
				unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot-generic-requires-specialization:${input.id}');
			if (input.field.overloads.get().length != 0)
				unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot-overloads-not-admitted:${input.id}');
			final signature = functionSignature(input.field.type, input.field.pos, input.sourcePath, 'virtual-slot:${input.id}');
			final ownerClass = input.owner.get()
				.isInterface ? null : requireClass(input.owner, input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:owner');
			final ownerInterface = input.owner.get()
				.isInterface ? requireInterface(input.owner, input.field.pos, input.sourcePath, 'interface-slot:${input.id}:owner') : null;
			final parameterTypes:Array<CBodyValueType> = [];
			for (index => argument in signature.arguments) {
				if (argument.opt)
					unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:optional-argument:$index');
				final mapping = valueType(argument.t, input.owner, input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:argument:$index');
				if (mapping.irType == IRTVoid)
					unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:Void-argument:$index');
				if (mapping.spanElement() != null)
					unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:borrowed-span-argument:$index');
				parameterTypes.push(mapping);
			}
			final returnType = valueType(signature.result, input.owner, input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:return');
			if (returnType.spanElement() != null)
				unsupportedAt(input.field.pos, input.sourcePath, 'virtual-slot:${input.id}:borrowed-span-return');
			var layoutSlots = layoutInputs.get(input.layoutId);
			if (layoutSlots == null) {
				layoutSlots = [];
				layoutInputs.set(input.layoutId, layoutSlots);
			}
			final memberRequest = new CSymbolRequest(CSKField, ["compiler", "virtual-dispatch", input.layoutId, input.id], CNSMember(input.layoutId),
				CSVInternal, null, [], [], layoutSlots.length);
			context.symbols.register(memberRequest);
			final slot = new CPreparedVirtualSlot(input, ownerClass, ownerInterface, parameterTypes, returnType, memberRequest);
			layoutSlots.push(slot);
			slots.push(slot);
			slotsById.set(input.id, slot);
			final ownerMethodId = CBodyLowering.methodId(input.owner.get().pack.concat([input.owner.get().name]).join("."), input.field.name);
			slotsByMethodId.set(ownerMethodId, slot);
			for (implementationId in input.implementationIds.keys())
				slotsByMethodId.set(implementationId, slot);
		}
		final layouts:Array<CPreparedVirtualLayout> = [];
		final layoutsById:Map<String, CPreparedVirtualLayout> = [];
		final layoutIds = [for (id in layoutInputs.keys()) id];
		layoutIds.sort(CBodyDispatchCatalog.compareUtf8);
		for (id in layoutIds) {
			final rawSlot = firstRawSlot(id);
			final rootClass = rawSlot.rootClass.get()
				.isInterface ? null : requireClass(rawSlot.rootClass, rawSlot.rootClass.get().pos, rawSlot.sourcePath, 'virtual-layout:$id:root');
			final rootInterface = rawSlot.rootClass.get()
				.isInterface ? requireInterface(rawSlot.rootClass, rawSlot.rootClass.get().pos, rawSlot.sourcePath, 'interface-layout:$id:root') : null;
			final source = HaxeSourceSpan.fromPosition(rawSlot.rootClass.get().pos, rawSlot.sourcePath);
			final rootPath = rootClass == null ?rootInterface == null ? throw new CBodyEmissionError('dispatch layout `$id` lost its root'):rootInterface.haxePath : rootClass.haxePath;
			final tagRequest = new CSymbolRequest(CSKType, [
				"compiler", rawSlot.rootClass.get().isInterface ? "interface-dispatch" : "virtual-dispatch",
				  rootPath,                                                                  "table-layout"
			], CNSTag("translation-unit"), CSVInternal);
			context.symbols.register(tagRequest);
			var valueTagRequest:Null<CSymbolRequest> = null;
			var objectMemberRequest:Null<CSymbolRequest> = null;
			var tableMemberRequest:Null<CSymbolRequest> = null;
			if (rootInterface != null) {
				valueTagRequest = new CSymbolRequest(CSKType, ["compiler", "interface-dispatch", rootPath, "value"], CNSTag("translation-unit"), CSVInternal);
				objectMemberRequest = new CSymbolRequest(CSKField, ["compiler", "interface-dispatch", rootPath, "value", "object"],
					CNSMember('interface-value:$id'), CSVInternal, "object", [], [], 0);
				tableMemberRequest = new CSymbolRequest(CSKField, ["compiler", "interface-dispatch", rootPath, "value", "table"],
					CNSMember('interface-value:$id'), CSVInternal, "table", [], [], 1);
				context.symbols.register(valueTagRequest);
				context.symbols.register(objectMemberRequest);
				context.symbols.register(tableMemberRequest);
			}
			final layoutSlots = layoutInputs.get(id);
			if (layoutSlots == null)
				throw new CBodyEmissionError('dispatch preparation lost slots for layout `$id`');
			final layout = new CPreparedVirtualLayout(id, rootClass, rootInterface, source, tagRequest, valueTagRequest, objectMemberRequest,
				tableMemberRequest, layoutSlots);
			if (rootClass != null)
				aggregates.requireVirtualHeader(rootClass, id);
			layouts.push(layout);
			layoutsById.set(id, layout);
		}

		final thunks:Array<CPreparedVirtualThunk> = [];
		final thunksById:Map<String, CPreparedVirtualThunk> = [];
		final tables:Array<CPreparedVirtualTable> = [];
		final tablesByInstanceAndLayout:Map<String, CPreparedVirtualTable> = [];
		for (input in graph.tables) {
			final layout = layoutsById.get(input.layoutId);
			if (layout == null)
				throw new CBodyEmissionError('dispatch table `${input.id}` refers to unknown layout `${input.layoutId}`');
			final classValue = requireClass(input.classReference, input.classReference.get().pos, input.sourcePath, 'virtual-table:${input.id}:class');
			final tablePath = layout.isInterface() ? ["compiler", "interface-dispatch", classValue.haxePath, layout.id] : ["compiler", "virtual-dispatch", classValue.haxePath];
			final tableRequest = new CSymbolRequest(layout.isInterface() ? CSKInterfaceTable : CSKVTable, tablePath, CNSOrdinary("translation-unit"),
				CSVInternal);
			context.symbols.register(tableRequest);
			final entries:Array<CPreparedVirtualTableEntry> = [];
			for (entryInput in input.entries) {
				final slot = slotsById.get(entryInput.slotId);
				if (slot == null)
					throw new CBodyEmissionError('dispatch table `${input.id}` refers to unknown slot `${entryInput.slotId}`');
				if (entryInput.implementation == null) {
					if (entryInput.required)
						unsupportedAt(input.classReference.get().pos, input.sourcePath,
							'virtual-implementation-without-body:slot=${entryInput.slotId}:class=${input.classReference.get().pack.concat([input.classReference.get().name]).join(".")}');
					entries.push(new CPreparedVirtualTableEntry(slot, null, null, null));
					continue;
				}
				final implementation = entryInput.implementation;
				final implementationOwnerReference = implementation.instanceOwner;
				if (implementationOwnerReference == null)
					throw new CBodyEmissionError('virtual implementation `${CBodyLowering.functionInputId(implementation)}` lost its instance owner');
				final implementationOwner = requireClass(implementationOwnerReference, implementation.expression.pos, implementation.sourcePath,
					'virtual-implementation:${CBodyLowering.functionInputId(implementation)}:owner');
				validateImplementation(slot, implementation, implementationOwnerReference);
				final implementationId = CBodyLowering.functionInputId(implementation);
				var thunk:Null<CPreparedVirtualThunk> = null;
				if (slot.isInterface() || slot.ownerClass == null || implementationOwner.instanceId != slot.ownerClass.instanceId) {
					final thunkId = 'thunk.${slot.input.id}.$implementationId';
					thunk = thunksById.get(thunkId);
					if (thunk == null) {
						thunk = prepareThunk(thunkId, slot, implementationId, implementationOwner);
						thunksById.set(thunkId, thunk);
						thunks.push(thunk);
					}
				}
				entries.push(new CPreparedVirtualTableEntry(slot, implementationId, implementationOwner, thunk));
			}
			final table = new CPreparedVirtualTable(input, layout, classValue, HaxeSourceSpan.fromPosition(input.classReference.get().pos, input.sourcePath),
				tableRequest, entries);
			tables.push(table);
			final lookupKey = layout.isInterface() ? 'interface:${classValue.instanceId}:${layout.rootInstanceId()}' : 'class:${classValue.instanceId}';
			tablesByInstanceAndLayout.set(lookupKey, table);
		}
		thunks.sort((left, right) -> CBodyDispatchCatalog.compareUtf8(left.id, right.id));
		return new CPreparedBodyDispatch(graph, layouts, slots, tables, thunks, slotsByMethodId, tablesByInstanceAndLayout);
	}

	function validateImplementation(slot:CPreparedVirtualSlot, input:CBodyFunctionInput, owner:Ref<ClassType>):Void {
		final signature = functionSignature(input.fieldType, input.expression.pos, input.sourcePath,
			'virtual-implementation:${CBodyLowering.functionInputId(input)}');
		if (signature.arguments.length != slot.parameters.length) {
			unsupportedAt(input.expression.pos, input.sourcePath,
				'virtual-override-representation-mismatch:slot=${slot.input.id}:implementation=${CBodyLowering.functionInputId(input)}:argument-count=${signature.arguments.length}:expected=${slot.parameters.length}');
		}
		for (index in 0...signature.arguments.length) {
			final argument = signature.arguments[index];
			if (argument.opt)
				unsupportedAt(input.expression.pos, input.sourcePath,
					'virtual-override-representation-mismatch:slot=${slot.input.id}:implementation=${CBodyLowering.functionInputId(input)}:optional-argument:$index');
			final actual = valueType(argument.t, owner, input.expression.pos, input.sourcePath,
				'virtual-implementation:${CBodyLowering.functionInputId(input)}:argument:$index');
			final expected = slot.parameters[index];
			if (typeKey(actual.irType) != typeKey(expected.irType)) {
				unsupportedAt(input.expression.pos, input.sourcePath,
					'virtual-override-representation-mismatch:slot=${slot.input.id}:implementation=${CBodyLowering.functionInputId(input)}:argument=$index:expected=${typeKey(expected.irType)}:actual=${typeKey(actual.irType)}');
			}
		}
		final actualReturn = valueType(signature.result, owner, input.expression.pos, input.sourcePath,
			'virtual-implementation:${CBodyLowering.functionInputId(input)}:return');
		if (typeKey(actualReturn.irType) != typeKey(slot.returnType.irType)) {
			unsupportedAt(input.expression.pos, input.sourcePath,
				'virtual-override-representation-mismatch:slot=${slot.input.id}:implementation=${CBodyLowering.functionInputId(input)}:return:expected=${typeKey(slot.returnType.irType)}:actual=${typeKey(actualReturn.irType)}');
		}
	}

	function prepareThunk(id:String, slot:CPreparedVirtualSlot, implementationId:String, implementationOwner:CPreparedBodyClass):CPreparedVirtualThunk {
		final signature = [
			slot.isInterface() ? 'interface-object:${slot.ownerInstanceId()}' : typeKey(IRTPointer(IRTInstance(slot.ownerInstanceId()), true))
		].concat(slot.parameters.map(value -> typeKey(value.irType)));
		final functionRequest = new CSymbolRequest(CSKMethod, ["compiler", "virtual-dispatch", "thunk", slot.input.id, implementationId],
			CNSOrdinary("translation-unit"), CSVInternal, null, signature);
		context.symbols.register(functionRequest);
		final receiverRequest = new CSymbolRequest(CSKLocal, [
			"compiler",
			"virtual-dispatch",
			"thunk",
			slot.input.id,
			implementationId,
			"receiver"
		], CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], 0);
		context.symbols.register(receiverRequest);
		final argumentRequests:Array<CSymbolRequest> = [];
		for (index in 0...slot.parameters.length) {
			final request = new CSymbolRequest(CSKLocal, [
				"compiler",
				"virtual-dispatch",
				"thunk",
				slot.input.id,
				implementationId,
				'argument-$index'
			], CNSOrdinary(functionRequest.stableKey()), CSVInternal, null, [], [], index + 1);
			context.symbols.register(request);
			argumentRequests.push(request);
		}
		return new CPreparedVirtualThunk(id, slot, implementationId, implementationOwner, functionRequest, receiverRequest, argumentRequests);
	}

	function firstRawSlot(layoutId:String):CBodyVirtualSlotInput {
		for (slot in graph.slots) {
			if (slot.layoutId == layoutId)
				return slot;
		}
		throw new CBodyEmissionError('dispatch preparation lost raw layout `$layoutId`');
	}

	function requireClass(reference:Ref<ClassType>, position:Position, sourcePath:String, node:String):CPreparedBodyClass {
		final mapping = valueType(TInst(reference, []), reference, position, sourcePath, node);
		final value = mapping.classValue();
		if (value == null)
			return unsupportedAt(position, sourcePath, '$node:not-concrete-class');
		return value;
	}

	function requireInterface(reference:Ref<ClassType>, position:Position, sourcePath:String, node:String):CPreparedBodyInterface
		return aggregates.requireInterface(reference, [], position, sourcePath,
			(failurePosition, failureNode) -> unsupportedAt(failurePosition, sourcePath, failureNode), node);

	function valueType(type:Type, owner:Ref<ClassType>, position:Position, sourcePath:String, node:String):CBodyValueType
		return aggregates.valueType(type, position, owner.get().module, sourcePath,
			(failurePosition, failureNode) -> unsupportedAt(failurePosition, sourcePath, failureNode), node);

	function functionSignature(type:Type, position:Position, sourcePath:String, node:String):{arguments:Array<{name:String, opt:Bool, t:Type}>, result:Type} {
		return switch TypeTools.follow(type) {
			case TFun(arguments, result): {arguments: arguments, result: result};
			case _: unsupportedAt(position, sourcePath, '$node:not-function-type');
		};
	}

	function unsupportedAt<T>(position:Position, sourcePath:String, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, "virtual slot representation validation",
			source), position);
	}

	public static function typeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTAbiInteger(kind): 'abi:${Std.string(kind)}';
			case IRTFloat(width): 'f$width';
			case IRTString: "string";
			case IRTManagedString: "managed-string";
			case IRTCString: "cstring-borrowed-literal";
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "nonnull"}:${typeKey(pointee)}';
			case IRTNullable(value, representation): 'nullable:${Std.string(representation)}:${typeKey(value)}';
			case IRTFunction(parameters, result): 'function:(${parameters.map(typeKey).join(",")})->${typeKey(result)}';
			case IRTFixedArray(element, length, witnessId): 'fixed-array:$length:$witnessId:${typeKey(element)}';
			case IRTSpan(element, mutable): 'span:${mutable ? "mutable" : "const"}:${typeKey(element)}';
			case IRTDynamic: "dynamic";
		};
	}
}
#else
class CBodyDispatch {
	public function new() {}
}
#end
