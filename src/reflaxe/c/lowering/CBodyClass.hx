package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
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
				header: dispatchLayoutId == null ? IRCHNone : IRCHVirtual(dispatchLayoutId)
			}),
			source: source
		};
	}

	public function instance():HxcIRTypeInstance {
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [],
			representation: IRRDirect,
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

	public function new(prepared:CPreparedBodyClass, cTag:CIdentifier, baseMember:Null<CIdentifier>, dispatchHeader:Null<CIdentifier>,
			emptyAnchor:Null<CIdentifier>, fields:Array<CLoweredBodyClassField>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.baseMember = baseMember;
		this.dispatchHeader = dispatchHeader;
		this.emptyAnchor = emptyAnchor;
		this.fields = fields.copy();
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
			final fieldType = resolveValue(field.type, field.pos, definition.module, sourcePath, fail, '$node.field:${field.name}');
			if (fieldType.spanElement() != null)
				return rejected(fail, field.pos, '$node.field:${field.name}:borrowed-span-field-escape');
			if (fieldType.irType == IRTVoid)
				return rejected(fail, field.pos, '$node.field:${field.name}:Void-not-an-object-field');
			final mutable = switch field.kind {
				case FVar(_, write): isDirectWrite(write);
				case FMethod(_): false;
			};
			final request = new CSymbolRequest(CSKField, ["compiler", "haxe-class", path, "field", field.name], CNSMember(prepared.declarationId),
				CSVInternal, null, [], [], storageOrdinal++, [field.name]);
			context.symbols.register(request);
			prepared.fields.push(new CPreparedBodyClassField(field.name, fieldType, mutable, HaxeSourceSpan.fromPosition(field.pos, sourcePath), request));
		}
		preparing.remove(path);
		return prepared;
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

	public function canonicalClasses():Array<CPreparedBodyClass> {
		final values = [for (value in byPath) value];
		values.sort((left, right) -> compareUtf8(left.semanticKey, right.semanticKey));
		final result:Array<CPreparedBodyClass> = [];
		final emitted:Map<String, Bool> = [];
		for (value in values)
			appendBaseFirst(value, result, emitted);
		return result;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyClass> {
		return canonicalClasses().map(prepared -> new CLoweredBodyClass(prepared, symbols.identifierFor(prepared.typeRequest),
			identifierOrNull(symbols, prepared.baseMemberRequest), identifierOrNull(symbols, prepared.dispatchHeaderRequest),
			identifierOrNull(symbols, prepared.emptyAnchorRequest),
			prepared.fields.map(field -> new CLoweredBodyClassField(field, symbols.identifierFor(field.request)))));
	}

	function appendBaseFirst(value:CPreparedBodyClass, result:Array<CPreparedBodyClass>, emitted:Map<String, Bool>):Void {
		if (emitted.exists(value.instanceId))
			return;
		if (value.base != null)
			appendBaseFirst(value.base, result, emitted);
		emitted.set(value.instanceId, true);
		result.push(value);
	}

	static function hasDirectStorage(read:VarAccess, write:VarAccess):Bool
		return isDirectAccess(read) || isDirectAccess(write);

	static function isDirectWrite(write:VarAccess):Bool {
		return switch write {
			case AccNormal | AccCtor: true;
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
