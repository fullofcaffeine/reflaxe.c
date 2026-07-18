package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.interop.CImportRegistry;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.interop.CImportRegistry.CPreparedImportConstant;
import reflaxe.c.interop.CImportRegistry.CPreparedImportFunction;
import reflaxe.c.interop.CImportRegistry.CPreparedImportType;
import reflaxe.c.lowering.CBodyClass.CBodyClassRegistry;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyClass.CPreparedBodyClass;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRegistry;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Module anchor for the closed anonymous-record lowering model. */
class CBodyAggregate {
	private function new() {}
}

/** A closed body value category; aggregate values never enter primitive semantics. */
enum CBodyValueKind {
	CBVKPrimitive(mapping:CPrimitiveTypeMapping);
	CBVKCString;
	CBVKImport(value:CPreparedImportType);
	CBVKAggregate(aggregate:CPreparedBodyAggregate);
	CBVKEnum(value:CPreparedBodyEnumInstance);
	CBVKClass(value:CPreparedBodyClass, nullable:Bool);
}

/** The exact admitted representation of one Haxe body value. */
class CBodyValueType {
	public final kind:CBodyValueKind;
	public final irType:HxcIRTypeRef;
	public final cSpelling:String;

	public function new(kind:CBodyValueKind) {
		this.kind = kind;
		switch kind {
			case CBVKPrimitive(mapping):
				this.irType = mapping.irType;
				this.cSpelling = mapping.cSpelling;
			case CBVKCString:
				this.irType = IRTCString;
				this.cSpelling = "const-char-pointer:borrowed-literal";
			case CBVKImport(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'c-import:${value.haxePath}';
			case CBVKAggregate(aggregate):
				this.irType = IRTInstance(aggregate.instanceId);
				this.cSpelling = 'closed-record:${aggregate.digest}';
			case CBVKEnum(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-enum:${value.digest}';
			case CBVKClass(value, nullable):
				this.irType = IRTPointer(IRTInstance(value.instanceId), nullable);
				this.cSpelling = 'haxe-class-reference:${nullable ? "nullable" : "nonnull"}:${value.digest}';
		}
	}

	public static function primitive(mapping:CPrimitiveTypeMapping):CBodyValueType
		return new CBodyValueType(CBVKPrimitive(mapping));

	public static function cString():CBodyValueType
		return new CBodyValueType(CBVKCString);

	public static function imported(value:CPreparedImportType):CBodyValueType
		return new CBodyValueType(CBVKImport(value));

	public static function aggregate(value:CPreparedBodyAggregate):CBodyValueType
		return new CBodyValueType(CBVKAggregate(value));

	public static function enumeration(value:CPreparedBodyEnumInstance):CBodyValueType
		return new CBodyValueType(CBVKEnum(value));

	public static function classReference(value:CPreparedBodyClass, nullable:Bool = true):CBodyValueType
		return new CBodyValueType(CBVKClass(value, nullable));

	public function primitiveMapping():Null<CPrimitiveTypeMapping> {
		return switch kind {
			case CBVKPrimitive(mapping): mapping;
			case CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKEnum(_) | CBVKClass(_, _): null;
		};
	}

	public function isCString():Bool
		return kind == CBVKCString;

	public function importedValue():Null<CPreparedImportType> {
		return switch kind {
			case CBVKImport(value): value;
			case _: null;
		};
	}

	public function importedStructValue():Null<CPreparedImportType> {
		return switch kind {
			case CBVKImport(value) if (value.kind == CITStruct): value;
			case _: null;
		};
	}

	public function aggregateValue():Null<CPreparedBodyAggregate> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKCString | CBVKImport(_): null;
			case CBVKAggregate(aggregate): aggregate;
			case CBVKEnum(_) | CBVKClass(_, _): null;
		};
	}

	public function enumValue():Null<CPreparedBodyEnumInstance> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKClass(_, _): null;
			case CBVKEnum(value): value;
		};
	}

	public function classValue():Null<CPreparedBodyClass> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKEnum(_): null;
			case CBVKClass(value, _): value;
		};
	}

	public function classNullable():Null<Bool> {
		return switch kind {
			case CBVKClass(_, nullable): nullable;
			case CBVKPrimitive(_) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) | CBVKEnum(_): null;
		};
	}
}

/** One canonical field before C names are finalized. */
class CPreparedBodyAggregateField {
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

/** One shape-deduplicated anonymous record before symbol finalization. */
class CPreparedBodyAggregate {
	public final shapeKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final typeRequest:CSymbolRequest;
	public final fields:Array<CPreparedBodyAggregateField>;

	public function new(shapeKey:String, digest:String, ownerModule:String, source:HxcSourceSpan, typeRequest:CSymbolRequest) {
		this.shapeKey = shapeKey;
		this.digest = digest;
		this.declarationId = 'type.closed-record.$digest';
		this.instanceId = 'instance.closed-record.$digest';
		this.ownerModule = ownerModule;
		this.source = source;
		this.typeRequest = typeRequest;
		this.fields = [];
	}

	public function declaration():HxcIRTypeDeclaration {
		return {
			id: declarationId,
			displayName: 'anonymous.record.${digest.substr(0, 16)}',
			kind: IRTKAggregate(fields.map(field -> {
				name: field.name,
				type: field.type.irType,
				mutable: field.mutable,
				source: field.source
			})),
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
}

/** One finalized aggregate member used by structural C emission. */
class CLoweredBodyAggregateField {
	public final semanticName:String;
	public final type:CBodyValueType;
	public final mutable:Bool;
	public final cName:CIdentifier;

	public function new(semanticName:String, type:CBodyValueType, mutable:Bool, cName:CIdentifier) {
		this.semanticName = semanticName;
		this.type = type;
		this.mutable = mutable;
		this.cName = cName;
	}
}

/** One finalized private C struct and its matching direct HxcIR instance. */
class CLoweredBodyAggregate {
	public final prepared:CPreparedBodyAggregate;
	public final cTag:CIdentifier;
	public final fields:Array<CLoweredBodyAggregateField>;

	public function new(prepared:CPreparedBodyAggregate, cTag:CIdentifier, fields:Array<CLoweredBodyAggregateField>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.fields = fields.copy();
	}

	public function field(semanticName:String):Null<CLoweredBodyAggregateField> {
		for (field in fields) {
			if (field.semanticName == semanticName) {
				return field;
			}
		}
		return null;
	}
}

/**
	Request-local structural identity registry.

	Field expressions retain source evaluation order in HxcIR, while declaration
	fields are UTF-8 ordered here. Identical closed shapes therefore share one IR
	instance and one C tag even when typedef aliases or source field order differ.
 */
class CBodyAggregateRegistry {
	final context:CompilationContext;
	final byShape:Map<String, CPreparedBodyAggregate> = [];
	final enumRegistry:CBodyEnumRegistry;
	final classRegistry:CBodyClassRegistry;
	final importRegistry:Null<CImportRegistry>;

	public function new(context:CompilationContext, ?program:TypedProgramInput, ?contract:TypedCContractSnapshot) {
		this.context = context;
		this.enumRegistry = new CBodyEnumRegistry(context, valueType);
		this.classRegistry = new CBodyClassRegistry(context, valueType);
		this.importRegistry = program == null || contract == null ? null : new CImportRegistry(context, program, contract, valueType);
	}

	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):CBodyValueType {
		final imported = importRegistry == null ? null : importRegistry.valueType(type, position, ownerModule, sourcePath, fail, node);
		if (imported != null)
			return imported;
		final resolved = unwrapAliases(type, position, fail, node);
		return switch resolved {
			case TInst(reference, parameters) if (!reference.get().isExtern):
				classRegistry.valueType(reference, parameters, position, ownerModule, sourcePath, fail, node);
			case TEnum(reference, parameters):
				enumRegistry.valueType(reference, parameters, position, ownerModule, sourcePath, fail, node);
			case TAbstract(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Null" && parameters.length == 1):
				final nullable = valueType(parameters[0], position, ownerModule, sourcePath, fail, '$node.nullable');
				nullable.classValue() == null ? CBodyValueType.primitive(admittedPrimitive(resolved, position, fail, node)) : nullable;
			case TAnonymous(reference):
				final shape = anonymousShape(reference, [], position, fail, node);
				var aggregate = byShape.get(shape);
				if (aggregate == null) {
					aggregate = prepareAggregate(reference, shape, position, ownerModule, sourcePath, fail, node);
					byShape.set(shape, aggregate);
				}
				CBodyValueType.aggregate(aggregate);
			case _:
				CBodyValueType.primitive(admittedPrimitive(resolved, position, fail, node));
		};
	}

	public function importFunction(callee:TypedExpr, position:Position, sourcePath:String):Null<CPreparedImportFunction>
		return importRegistry == null ? null : importRegistry.functionFor(callee, position, sourcePath);

	public function importStaticConstant(classReference:Ref<ClassType>, fieldReference:Ref<ClassField>, position:Position,
			sourcePath:String):Null<CPreparedImportConstant>
		return importRegistry == null ? null : importRegistry.staticConstantFor(classReference, fieldReference, position, sourcePath);

	public function importEnumConstant(reference:Ref<EnumType>, field:EnumField, position:Position, sourcePath:String):Null<CPreparedImportConstant>
		return importRegistry == null ? null : importRegistry.enumConstantFor(reference, field, position, sourcePath);

	public function canonicalImports():Array<CPreparedImportType>
		return importRegistry == null ? [] : importRegistry.canonicalTypes();

	public function finalizeImports(symbols:CSymbolRegistry):CLoweredImports
		return importRegistry == null ? CLoweredImports.empty() : importRegistry.finalize(symbols);

	public function canonicalEnums():Array<CPreparedBodyEnumInstance>
		return enumRegistry.canonicalEnums();

	public function finalizeEnums(symbols:CSymbolRegistry):Array<CLoweredBodyEnum>
		return enumRegistry.finalize(symbols);

	public function canonicalClasses():Array<CPreparedBodyClass>
		return classRegistry.canonicalClasses();

	public function finalizeClasses(symbols:CSymbolRegistry):Array<CLoweredBodyClass>
		return classRegistry.finalize(symbols);

	public function requireVirtualHeader(root:CPreparedBodyClass, layoutId:String):Void
		classRegistry.requireVirtualHeader(root, layoutId);

	public function completeClassLayouts():Void
		classRegistry.completeLayouts();

	public function canonicalAggregates():Array<CPreparedBodyAggregate> {
		final values = [for (aggregate in byShape) aggregate];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		final result:Array<CPreparedBodyAggregate> = [];
		final emitted:Map<String, Bool> = [];
		for (aggregate in values) {
			appendDependencies(aggregate, result, emitted);
		}
		return result;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyAggregate> {
		final result:Array<CLoweredBodyAggregate> = [];
		for (aggregate in canonicalAggregates()) {
			result.push(new CLoweredBodyAggregate(aggregate, symbols.identifierFor(aggregate.typeRequest),
				aggregate.fields.map(field -> new CLoweredBodyAggregateField(field.name, field.type, field.mutable, symbols.identifierFor(field.request)))));
		}
		return result;
	}

	function prepareAggregate(reference:Ref<AnonType>, shape:String, position:Position, ownerModule:String, sourcePath:String,
			fail:(Position, String) -> Void, node:String):CPreparedBodyAggregate {
		final digest = Sha256.encode(shape);
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		final typeRequest = new CSymbolRequest(CSKType, ["compiler", "closed-record", digest], CNSTag("translation-unit"), CSVInternal);
		context.symbols.register(typeRequest);
		final aggregate = new CPreparedBodyAggregate(shape, digest, ownerModule, source, typeRequest);
		final fields = reference.get().fields.copy();
		fields.sort((left, right) -> compareUtf8(left.name, right.name));
		for (index in 0...fields.length) {
			final field = fields[index];
			final fieldType = valueType(field.type, field.pos, ownerModule, sourcePath, fail, '$node.field:${field.name}');
			if (fieldType.irType == IRTVoid) {
				return rejected(fail, field.pos, '$node.field:${field.name}:Void-not-an-object-type');
			}
			final mutable = switch field.kind {
				case FVar(_, write): isWritable(write);
				case FMethod(_): rejected(fail, field.pos, '$node.field:${field.name}:method');
			};
			final request = new CSymbolRequest(CSKField, ["compiler", "closed-record", digest, field.name], CNSMember(aggregate.declarationId), CSVInternal,
				null, [], [], index);
			context.symbols.register(request);
			aggregate.fields.push(new CPreparedBodyAggregateField(field.name, fieldType, mutable, HaxeSourceSpan.fromPosition(field.pos, sourcePath), request));
		}
		return aggregate;
	}

	function anonymousShape(reference:Ref<AnonType>, stack:Array<Ref<AnonType>>, position:Position, fail:(Position, String) -> Void, node:String):String {
		for (active in stack) {
			if (active == reference) {
				return rejected(fail, position, '$node:recursive-by-value-shape');
			}
		}
		final value = reference.get();
		switch value.status {
			case AClosed | AConst:
			case AOpened:
				return rejected(fail, position, '$node:open-anonymous-shape');
			case AExtend(_):
				return rejected(fail, position, '$node:extended-anonymous-shape');
			case AClassStatics(_) | AEnumStatics(_) | AAbstractStatics(_):
				return rejected(fail, position, '$node:static-container-not-record');
		}
		if (value.fields.length == 0) {
			return rejected(fail, position, '$node:empty-anonymous-shape-not-strict-c11');
		}
		final fields = value.fields.copy();
		fields.sort((left, right) -> compareUtf8(left.name, right.name));
		final nextStack = stack.copy();
		nextStack.push(reference);
		final parts:Array<String> = [];
		for (field in fields) {
			final access = switch field.kind {
				case FVar(read, write): 'var:${accessKey(read)}:${accessKey(write)}:${field.isFinal ? "final" : "nonfinal"}';
				case FMethod(_): return rejected(fail, field.pos, '$node.field:${field.name}:method');
			};
			parts.push(canonicalPart(field.name)
				+ canonicalPart(access)
				+ canonicalPart(typeShape(field.type, nextStack, field.pos, fail, '$node.field:${field.name}')));
		}
		return 'closed-record-v1(${parts.join("")})';
	}

	function typeShape(type:Type, stack:Array<Ref<AnonType>>, position:Position, fail:(Position, String) -> Void, node:String):String {
		final resolved = unwrapAliases(type, position, fail, node);
		return switch resolved {
			case TAnonymous(reference): anonymousShape(reference, stack, position, fail, node);
			case _:
				final mapping = admittedPrimitive(resolved, position, fail, node);
				mapping.irType == IRTVoid ? rejected(fail, position, '$node:Void-not-an-object-type') : primitiveTypeKey(mapping.irType);
		};
	}

	function admittedPrimitive(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(type, context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable;
				if (!admitted) {
					return rejected(fail, position, '$node:${mapping.cSpelling}');
				}
				mapping;
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				rejected(fail, position, '$node:$reason');
		};
	}

	static function unwrapAliases(type:Type, position:Position, fail:(Position, String) -> Void, node:String):Type {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved-monomorph') : unwrapAliases(resolved, position, fail, node);
			case TLazy(resolve): unwrapAliases(resolve(), position, fail, node);
			case TType(reference, parameters):
				final definition = reference.get();
				unwrapAliases(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), position, fail, node);
			case _: type;
		};
	}

	static function isWritable(access:VarAccess):Bool {
		return switch access {
			case AccNo | AccNever: false;
			case AccNormal | AccResolve | AccCall | AccPrivateCall | AccInline | AccRequire(_, _) | AccCtor: true;
		};
	}

	static function accessKey(access:VarAccess):String {
		return switch access {
			case AccNormal: "normal";
			case AccNo: "none";
			case AccNever: "never";
			case AccResolve: "resolve";
			case AccCall: "call";
			case AccPrivateCall: "private-call";
			case AccInline: "inline";
			case AccRequire(requirement, message): 'require:${canonicalPart(requirement)}:${canonicalPart(message == null ? "" : message)}';
			case AccCtor: "constructor";
		};
	}

	function appendDependencies(aggregate:CPreparedBodyAggregate, result:Array<CPreparedBodyAggregate>, emitted:Map<String, Bool>):Void {
		if (emitted.exists(aggregate.declarationId)) {
			return;
		}
		for (field in aggregate.fields) {
			final dependency = field.type.aggregateValue();
			if (dependency != null) {
				appendDependencies(dependency, result, emitted);
			}
		}
		emitted.set(aggregate.declarationId, true);
		result.push(aggregate);
	}

	static function primitiveTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTVoid: "void";
			case _: throw new CBodyEmissionError("anonymous record field resolved to a non-direct primitive type");
		};
	}

	static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';

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

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("anonymous record rejection callback returned unexpectedly");
	}
}
#else
class CBodyAggregate {
	public function new() {}
}
#end
