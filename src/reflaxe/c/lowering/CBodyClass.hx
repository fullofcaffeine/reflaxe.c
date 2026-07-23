package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/** Module anchor for concrete private Haxe-class instance layouts. */
class CBodyClass {
	private function new() {}
}

typedef CBodyClassValueResolver = (type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
	node:String) -> CBodyValueType;

/** One source-order storage field owned by a single class declaration. */
class CPreparedBodyClassField {
	public final name:String;
	public final type:CBodyValueType;
	public final mutable:Bool;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;

	public function new(name:String, type:CBodyValueType, mutable:Bool, source:HxcSourceSpan, request:CSymbolRequest) {
		this.name = name;
		this.type = type;
		this.mutable = mutable;
		this.source = source;
		this.request = request;
	}
}

/** One non-generic nominal class after recursive layout discovery. */
class CPreparedBodyClass {
	public final semanticKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final haxePath:String;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final typeRequest:CSymbolRequest;
	public final fields:Array<CPreparedBodyClassField> = [];
	public var base:Null<CPreparedBodyClass> = null;
	public var baseMemberRequest:Null<CSymbolRequest> = null;
	public var dispatchLayoutId:Null<String> = null;
	public var dispatchHeaderRequest:Null<CSymbolRequest> = null;
	public var emptyAnchorRequest:Null<CSymbolRequest> = null;

	/** True only when a reachable escaping graph requires stable GC storage. */
	public var managedByCollector:Bool = false;

	public var descriptorRequest:Null<CSymbolRequest> = null;
	public var traceRequest:Null<CSymbolRequest> = null;
	public var finalizerRequest:Null<CSymbolRequest> = null;

	public function new(semanticKey:String, digest:String, haxePath:String, ownerModule:String, source:HxcSourceSpan, typeRequest:CSymbolRequest) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.class.$digest';
		this.instanceId = 'instance.class.$digest';
		this.haxePath = haxePath;
		this.ownerModule = ownerModule;
		this.source = source;
		this.typeRequest = typeRequest;
	}

	public function declaration():HxcIRTypeDeclaration {
		return {
			id: declarationId,
			displayName: haxePath,
			kind: IRTKClass({
				baseInstanceId: base == null ? null : base.instanceId,
				fields: fields.map(field -> {
					name: field.name,
					type: field.type.irType,
					mutable: field.mutable,
					source: field.source
				}),
				header: managedByCollector ? IRCHRuntime("gc") : dispatchLayoutId == null ? IRCHNone : IRCHVirtual(dispatchLayoutId)
			}),
			source: source
		};
	}

	public function instance():HxcIRTypeInstance {
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [],
			representation: managedByCollector ? IRRManaged("gc") : IRRDirect,
			source: source
		};
	}

	public function field(name:String):Null<CPreparedBodyClassField> {
		for (field in fields) {
			if (field.name == name)
				return field;
		}
		return base == null ? null : base.field(name);
	}

	public function ownsField(name:String):Bool {
		for (field in fields) {
			if (field.name == name)
				return true;
		}
		return false;
	}

	public function isDescendantOf(target:CPreparedBodyClass):Bool {
		var current:Null<CPreparedBodyClass> = this;
		while (current != null) {
			if (current.instanceId == target.instanceId)
				return true;
			current = current.base;
		}
		return false;
	}
}

class CLoweredBodyClassField {
	public final prepared:CPreparedBodyClassField;
	public final cName:CIdentifier;

	public function new(prepared:CPreparedBodyClassField, cName:CIdentifier) {
		this.prepared = prepared;
		this.cName = cName;
	}
}

/** Finalized private C names for one concrete class layout. */
class CLoweredBodyClass {
	public final prepared:CPreparedBodyClass;
	public final cTag:CIdentifier;
	public final baseMember:Null<CIdentifier>;
	public final dispatchHeader:Null<CIdentifier>;
	public final emptyAnchor:Null<CIdentifier>;
	public final fields:Array<CLoweredBodyClassField>;
	public final descriptorName:Null<CIdentifier>;
	public final traceName:Null<CIdentifier>;
	public final finalizerName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyClass, cTag:CIdentifier, baseMember:Null<CIdentifier>, dispatchHeader:Null<CIdentifier>,
			emptyAnchor:Null<CIdentifier>, fields:Array<CLoweredBodyClassField>, descriptorName:Null<CIdentifier>, traceName:Null<CIdentifier>,
			finalizerName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.baseMember = baseMember;
		this.dispatchHeader = dispatchHeader;
		this.emptyAnchor = emptyAnchor;
		this.fields = fields.copy();
		this.descriptorName = descriptorName;
		this.traceName = traceName;
		this.finalizerName = finalizerName;
	}
}

/** Request-local nominal discovery with base-first deterministic layout order. */
class CBodyClassRegistry {
	final context:CompilationContext;
	final resolveValue:CBodyClassValueResolver;
	final byPath:Map<String, CPreparedBodyClass> = [];
	final semanticKeysByDigest:Map<String, String> = [];
	final preparing:Map<String, Bool> = [];

	public function new(context:CompilationContext, resolveValue:CBodyClassValueResolver) {
		this.context = context;
		this.resolveValue = resolveValue;
	}

	public function valueType(reference:Ref<ClassType>, parameters:Array<Type>, position:Position, ownerModule:String, ownerSourcePath:String,
			fail:(Position, String) -> Void, node:String):CBodyValueType {
		return CBodyValueType.classReference(require(reference, parameters, position, ownerModule, ownerSourcePath, fail, node));
	}

	public function require(reference:Ref<ClassType>, parameters:Array<Type>, position:Position, ownerModule:String, ownerSourcePath:String,
			fail:(Position, String) -> Void, node:String):CPreparedBodyClass {
		final definition = reference.get();
		final path = classPath(definition);
		if (definition.isExtern)
			return rejected(fail, position, '$node:extern-class:$path');
		if (definition.isInterface)
			return rejected(fail, position, '$node:interface-reference-requires-E3.T07:$path');
		switch definition.kind {
			case KNormal:
			case KTypeParameter(_):
				return rejected(fail, position, '$node:open-class-type-parameter:$path');
			case _:
				return rejected(fail, position, '$node:unsupported-class-kind:${Std.string(definition.kind)}:$path');
		}
		if (definition.params.length != 0 || parameters.length != 0) {
			return rejected(fail, position, '$node:generic-class-reference-requires-bounded-class-specialization:$path');
		}
		final existing = byPath.get(path);
		if (existing != null)
			return existing;

		final semanticKey = 'haxe-class-v1(${canonicalPart(path)})';
		final digest = Sha256.encode(semanticKey);
		final priorKey = semanticKeysByDigest.get(digest);
		if (priorKey != null && priorKey != semanticKey) {
			throw new CBodyEmissionError('class-layout digest collision `$digest` between `$priorKey` and `$semanticKey`');
		}
		semanticKeysByDigest.set(digest, semanticKey);
		final sourcePath = definition.module == ownerModule ? ownerSourcePath : moduleSourcePath(definition.module);
		final source = HaxeSourceSpan.fromPosition(definition.pos, sourcePath);
		final typeRequest = new CSymbolRequest(CSKType, ["compiler", "haxe-class", path], CNSTag("translation-unit"), CSVInternal, null, [], [], null,
			path.split("."));
		context.symbols.register(typeRequest);
		final prepared = new CPreparedBodyClass(semanticKey, digest, path, definition.module, source, typeRequest);
		byPath.set(path, prepared);
		preparing.set(path, true);

		if (definition.superClass != null) {
			final superClass = definition.superClass;
			if (superClass.params.length != 0) {
				return rejected(fail, definition.pos, '$node:generic-base-class-requires-bounded-class-specialization:$path');
			}
			final base = require(superClass.t, superClass.params, definition.pos, definition.module, sourcePath, fail, '$node.base');
			if (preparing.exists(base.haxePath))
				return rejected(fail, definition.pos, '$node:cyclic-class-inheritance:$path->${base.haxePath}');
			prepared.base = base;
			prepared.baseMemberRequest = new CSymbolRequest(CSKField, ["compiler", "haxe-class", path, "base"], CNSMember(prepared.declarationId),
				CSVInternal, null, [], [], 0, ["base"]);
			context.symbols.register(prepared.baseMemberRequest);
		}

		var storageOrdinal = prepared.base == null ? 0 : 1;
		for (field in definition.fields.get()) {
			switch field.kind {
				case FMethod(MethDynamic):
					return rejected(fail, field.pos, '$node:dynamic-instance-method-requires-E3.T08:${field.name}');
				case FMethod(_):
					continue;
				case FVar(read, write):
					if (!hasDirectStorage(read, write))
						continue;
			}
			if (field.isExtern || field.isAbstract || field.params.length != 0)
				return rejected(fail, field.pos, '$node:unsupported-instance-field:${field.name}');
			if (prepared.base != null && prepared.base.field(field.name) != null)
				return rejected(fail, field.pos, '$node:inherited-storage-field-shadowing:${field.name}');
			final mutable = switch field.kind {
				case FVar(_, write): isDirectWrite(write);
				case FMethod(_): false;
			};
			final fixedArray = CBodyFixedArray.shape(field.type, context.profile, field.pos, fail, '$node.field:${field.name}');
			final fieldType = if (fixedArray == null) {
				final resolved = resolveValue(field.type, field.pos, definition.module, sourcePath, fail, '$node.field:${field.name}');
				final child = resolved.classValue();
				final initializer = child == null ? null : fieldInitializer(definition, field, fail, '$node.field:${field.name}');
				if (child != null && initializer != null && initializedClassPath(initializer) == child.haxePath) {
					if (mutable)
						return rejected(fail, field.pos, '$node.field:${field.name}:owned-class-field-must-be-final');
					if (preparing.exists(child.haxePath))
						return rejected(fail, field.pos, '$node.field:${field.name}:cyclic-owned-class-layout:$path->${child.haxePath}');
					CBodyValueType.ownedClass(child);
				} else {
					resolved;
				}
			} else {
				final initializer = fieldInitializer(definition, field, fail, '$node.field:${field.name}');
				if (initializer == null)
					return rejected(fail, field.pos, '$node.field:${field.name}:fixed-array-requires-initializer');
				final length = CBodyFixedArray.zeroLength(initializer, fixedArray.element.irType, fail, '$node.field:${field.name}:CArray.zero');
				CBodyValueType.fixedArray(fixedArray.element, length, fixedArray.witnessId);
			};
			if (fieldType.spanElement() != null)
				return rejected(fail, field.pos, '$node.field:${field.name}:borrowed-span-field-escape');
			if (fieldType.irType == IRTVoid)
				return rejected(fail, field.pos, '$node.field:${field.name}:Void-not-an-object-field');
			final request = new CSymbolRequest(CSKField, ["compiler", "haxe-class", path, "field", field.name], CNSMember(prepared.declarationId),
				CSVInternal, null, [], [], storageOrdinal++, [field.name]);
			context.symbols.register(request);
			prepared.fields.push(new CPreparedBodyClassField(field.name, fieldType, mutable, HaxeSourceSpan.fromPosition(field.pos, sourcePath), request));
		}
		preparing.remove(path);
		return prepared;
	}

	/**
		Recover Haxe's typed field-initializer assignment from the constructor.

		After dead-code elimination, Haxe moves instance field initializers into the
		constructor and clears `ClassField.expr()`. The initializer's source range is
		still inside the field declaration, which distinguishes it from an ordinary
		assignment written in the constructor body.
	**/
	static function fieldInitializer(definition:ClassType, field:ClassField, fail:(Position, String) -> Void, node:String):Null<TypedExpr> {
		if (definition.constructor == null)
			return null;
		final constructorExpression = definition.constructor.get().expr();
		if (constructorExpression == null)
			return null;
		final body = switch constructorExpression.expr {
			case TFunction(fn): fn.expr;
			case _: constructorExpression;
		};
		final matches:Array<TypedExpr> = [];
		collectFieldInitializers(body, field, matches);
		if (matches.length > 1)
			return rejected(fail, field.pos, '$node:ambiguous-lowered-initializer:${matches.length}');
		return matches.length == 1 ? matches[0] : null;
	}

	static function collectFieldInitializers(expression:TypedExpr, field:ClassField, matches:Array<TypedExpr>):Void {
		switch expression.expr {
			case TBinop(OpAssign, left, right):
				switch left.expr {
					case TField({expr: TConst(TThis)}, FInstance(_, _, fieldReference))
						if (fieldReference.get().name == field.name && positionInside(right.pos, field.pos)):
						matches.push(right);
					case _:
				}
			case TFunction(_):
				return;
			case _:
		}
		TypedExprTools.iter(expression, child -> collectFieldInitializers(child, field, matches));
	}

	static function initializedClassPath(expression:TypedExpr):Null<String> {
		return switch expression.expr {
			case TNew(reference, _, _):
				final value = reference.get();
				value.pack.concat([value.name]).join(".");
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): initializedClassPath(inner);
			case _: null;
		};
	}

	static function positionInside(inner:Position, outer:Position):Bool {
		final innerInfo = Context.getPosInfos(inner);
		final outerInfo = Context.getPosInfos(outer);
		return innerInfo.file == outerInfo.file && innerInfo.min >= outerInfo.min && innerInfo.max <= outerInfo.max;
	}

	/** Select one program-local vtable pointer on the hierarchy root only. */
	public function requireVirtualHeader(root:CPreparedBodyClass, layoutId:String):Void {
		if (root.base != null)
			throw new CBodyEmissionError('virtual layout `$layoutId` selected non-root class `${root.haxePath}`');
		if (root.dispatchLayoutId != null && root.dispatchLayoutId != layoutId) {
			throw new CBodyEmissionError('class `${root.haxePath}` received conflicting virtual layouts `${root.dispatchLayoutId}` and `$layoutId`');
		}
		root.dispatchLayoutId = layoutId;
		if (root.dispatchHeaderRequest == null) {
			root.dispatchHeaderRequest = new CSymbolRequest(CSKField, ["compiler", "haxe-class", root.haxePath, "virtual-table"],
				CNSMember(root.declarationId), CSVInternal, null, [], [], 0, ["vtable"]);
			context.symbols.register(root.dispatchHeaderRequest);
		}
	}

	/** Register strict-C empty anchors only after selective headers are known. */
	public function completeLayouts():Void {
		for (prepared in canonicalClasses()) {
			if (prepared.base == null
				&& prepared.fields.length == 0
				&& prepared.dispatchHeaderRequest == null
				&& prepared.emptyAnchorRequest == null) {
				prepared.emptyAnchorRequest = new CSymbolRequest(CSKField, ["compiler", "haxe-class", prepared.haxePath, "empty-anchor"],
					CNSMember(prepared.declarationId), CSVInternal, null, [], [], 0, ["storage"]);
				context.symbols.register(prepared.emptyAnchorRequest);
			}
		}
	}

	/**
		Close the collector representation after every reachable type is known.

		An `Array<Class>` or enum case carrying a class is an escaping graph: its
		pointer must remain stable and may point back through another managed field.
		Marking is a fixed point because a managed class can expose a second class
		through one of its fields or inheritance prefix. Direct classes that never
		enter such a graph remain unchanged and descriptor-free.
	**/
	public function completeManagedRepresentations(arrays:Array<reflaxe.c.lowering.CBodyArray.CPreparedBodyArray>,
			enums:Array<reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance>):Void {
		var changed = true;
		while (changed) {
			changed = false;
			for (array in arrays)
				if (markCollectorClasses(array.element, []))
					changed = true;
			for (enumValue in enums)
				for (tagCase in enumValue.cases)
					for (payload in tagCase.payload)
						if (markCollectorClasses(payload.valueType, []))
							changed = true;
			for (value in canonicalClasses()) {
				if (!value.managedByCollector)
					continue;
				if (value.dispatchLayoutId != null)
					throw new CBodyEmissionError('managed class `${value.haxePath}` cannot yet combine collector and virtual headers');
				if (value.base != null && !value.base.managedByCollector) {
					value.base.managedByCollector = true;
					changed = true;
				}
				for (field in value.fields) {
					if (field.type.ownedClassValue() != null)
						throw new CBodyEmissionError('managed class `${value.haxePath}` cannot yet contain an inline owned class `${field.name}`');
					final target = field.type.classValue();
					if (target != null && !target.managedByCollector) {
						target.managedByCollector = true;
						changed = true;
					}
				}
			}
		}
		for (value in canonicalClasses())
			if (value.managedByCollector)
				registerManagedNames(value);
	}

	/** Mark every class reference reachable through one finite direct value. */
	function markCollectorClasses(type:CBodyValueType, visitedEnums:Map<String, Bool>):Bool {
		final classValue = type.classValue();
		if (classValue != null) {
			if (classValue.managedByCollector)
				return false;
			classValue.managedByCollector = true;
			return true;
		}
		var changed = false;
		final array = type.arrayValue();
		if (array != null)
			return markCollectorClasses(array.element, visitedEnums);
		final aggregate = type.aggregateValue();
		if (aggregate != null) {
			for (field in aggregate.fields)
				if (markCollectorClasses(field.type, visitedEnums))
					changed = true;
			return changed;
		}
		final optional = type.optionalValue();
		if (optional != null)
			return markCollectorClasses(optional.payload, visitedEnums);
		final enumValue = type.enumValue();
		if (enumValue == null || visitedEnums.exists(enumValue.instanceId))
			return false;
		visitedEnums.set(enumValue.instanceId, true);
		for (tagCase in enumValue.cases)
			for (payload in tagCase.payload)
				if (!payload.indirect && markCollectorClasses(payload.valueType, visitedEnums))
					changed = true;
		return changed;
	}

	function registerManagedNames(value:CPreparedBodyClass):Void {
		if (value.descriptorRequest != null)
			return;
		value.descriptorRequest = new CSymbolRequest(CSKTypeDescriptor, ["compiler", "gc", "class", value.haxePath, "descriptor"],
			CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 0, [value.haxePath, "descriptor"]);
		context.symbols.register(value.descriptorRequest);
		if (classNeedsTrace(value)) {
			value.traceRequest = new CSymbolRequest(CSKMethod, ["compiler", "gc", "class", value.haxePath, "trace"], CNSOrdinary("translation-unit"),
				CSVInternal, null, [], [], 1, [value.haxePath, "trace"]);
			context.symbols.register(value.traceRequest);
		}
		if (classNeedsFinalizer(value)) {
			value.finalizerRequest = new CSymbolRequest(CSKMethod, ["compiler", "gc", "class", value.haxePath, "finalize"], CNSOrdinary("translation-unit"),
				CSVInternal, null, [], [], 2, [value.haxePath, "finalize"]);
			context.symbols.register(value.finalizerRequest);
		}
	}

	static function classNeedsTrace(value:CPreparedBodyClass):Bool {
		if (value.base != null && classNeedsTrace(value.base))
			return true;
		for (field in value.fields)
			if (containsManagedReference(field.type, []))
				return true;
		return false;
	}

	static function classNeedsFinalizer(value:CPreparedBodyClass):Bool {
		if (value.base != null && classNeedsFinalizer(value.base))
			return true;
		for (field in value.fields) {
			final array = field.type.arrayValue();
			if (array != null && !array.managedByCollector)
				return true;
			if (field.type.bytesValue() != null)
				return true;
			final aggregate = field.type.aggregateValue();
			if (aggregate != null && aggregate.managedLifetime)
				return true;
			final enumeration = field.type.enumValue();
			if (enumeration != null && enumeration.managedLifetime)
				return true;
			final optional = field.type.optionalValue();
			if (optional != null && optional.managedLifetime)
				return true;
		}
		return false;
	}

	static function containsManagedReference(value:CBodyValueType, visitedEnums:Map<String, Bool>):Bool {
		final classValue = value.classValue();
		if (classValue != null)
			return classValue.managedByCollector;
		final arrayValue = value.arrayValue();
		if (arrayValue != null)
			return arrayValue.managedByCollector;
		final aggregate = value.aggregateValue();
		if (aggregate != null) {
			for (field in aggregate.fields)
				if (containsManagedReference(field.type, visitedEnums))
					return true;
			return false;
		}
		final optional = value.optionalValue();
		if (optional != null)
			return containsManagedReference(optional.payload, visitedEnums);
		final enumValue = value.enumValue();
		if (enumValue == null || visitedEnums.exists(enumValue.instanceId))
			return false;
		visitedEnums.set(enumValue.instanceId, true);
		return enumValue.collectorPayload;
	}

	public function canonicalClasses():Array<CPreparedBodyClass> {
		final values = [for (value in byPath) value];
		values.sort((left, right) -> compareUtf8(left.semanticKey, right.semanticKey));
		final result:Array<CPreparedBodyClass> = [];
		final emitted:Map<String, Bool> = [];
		for (value in values)
			appendLayoutDependenciesFirst(value, result, emitted);
		return result;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyClass> {
		return canonicalClasses().map(prepared -> new CLoweredBodyClass(prepared, symbols.identifierFor(prepared.typeRequest),
			identifierOrNull(symbols, prepared.baseMemberRequest), identifierOrNull(symbols, prepared.dispatchHeaderRequest),
			identifierOrNull(symbols, prepared.emptyAnchorRequest),
			prepared.fields.map(field -> new CLoweredBodyClassField(field, symbols.identifierFor(field.request))),
			identifierOrNull(symbols, prepared.descriptorRequest), identifierOrNull(symbols, prepared.traceRequest),
			identifierOrNull(symbols, prepared.finalizerRequest)));
	}

	/** Order every by-value class dependency before the layout that contains it. */
	function appendLayoutDependenciesFirst(value:CPreparedBodyClass, result:Array<CPreparedBodyClass>, emitted:Map<String, Bool>):Void {
		if (emitted.exists(value.instanceId))
			return;
		if (value.base != null)
			appendLayoutDependenciesFirst(value.base, result, emitted);
		for (field in value.fields) {
			final child = field.type.ownedClassValue();
			if (child != null)
				appendLayoutDependenciesFirst(child, result, emitted);
		}
		emitted.set(value.instanceId, true);
		result.push(value);
	}

	static function hasDirectStorage(read:VarAccess, write:VarAccess):Bool
		return isDirectAccess(read) || isDirectAccess(write);

	static function isDirectWrite(write:VarAccess):Bool {
		return switch write {
			// `AccCtor` is Haxe's constructor-only write permission for a final
			// field. It permits initialization, but it does not make the completed
			// object's field mutable.
			case AccNormal: true;
			case _: false;
		};
	}

	static function isDirectAccess(access:VarAccess):Bool {
		return switch access {
			case AccNormal | AccNo | AccNever | AccCtor: true;
			case AccResolve | AccCall | AccPrivateCall | AccInline | AccRequire(_, _): false;
		};
	}

	static function classPath(definition:ClassType):String
		return definition.pack.concat([definition.name]).join(".");

	static function moduleSourcePath(modulePath:String):String
		return modulePath.split(".").join("/") + ".hx";

	static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';

	static function compareUtf8(left:String, right:String):Int {
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

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("class-layout rejection callback returned unexpectedly");
	}
}
#else
class CBodyClass {
	public function new() {}
}
#end
