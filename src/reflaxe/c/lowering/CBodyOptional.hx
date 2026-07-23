package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.macro.Expr.Position;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/** Module anchor for direct tagged optional-value representation planning. */
class CBodyOptional {
	private function new() {}
}

/**
	One nullable direct value before its private C names are finalized.

	Haxe permits `null` for values whose selected C carrier is a value, including
	a closed record, a closed enum, or the immutable String view. Those C carriers
	have no shared null spelling, so the compiler records the distinction explicitly
	as a presence flag plus the payload. This plan is not a heap object and adds no
	allocation.
**/
class CPreparedBodyOptional {
	public final semanticKey:String;
	public final digest:String;
	public final planId:String;
	public final payload:CBodyValueType;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final position:Position;
	public final typeRequest:CSymbolRequest;
	public final presenceRequest:CSymbolRequest;
	public final payloadRequest:CSymbolRequest;
	public final managedLifetime:Bool;
	public var retainRequest:Null<CSymbolRequest> = null;
	public var destroyRequest:Null<CSymbolRequest> = null;
	public var retainParameterRequest:Null<CSymbolRequest> = null;
	public var destroyParameterRequest:Null<CSymbolRequest> = null;
	public var retainStatusRequest:Null<CSymbolRequest> = null;

	public function new(semanticKey:String, digest:String, payload:CBodyValueType, ownerModule:String, source:HxcSourceSpan, position:Position,
			typeRequest:CSymbolRequest, presenceRequest:CSymbolRequest, payloadRequest:CSymbolRequest, managedLifetime:Bool) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.planId = 'optional.$digest';
		this.payload = payload;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
		this.typeRequest = typeRequest;
		this.presenceRequest = presenceRequest;
		this.payloadRequest = payloadRequest;
		this.managedLifetime = managedLifetime;
	}

	/** Semantic HxcIR retains both the payload type and tagged representation. */
	public inline function irType():HxcIRTypeRef
		return IRTNullable(payload.irType, IRNTagged);

	public function retainImplementationId():Null<String>
		return managedLifetime ? 'optional-lifecycle:$planId:retain' : null;

	public function destroyImplementationId():Null<String>
		return managedLifetime ? 'optional-lifecycle:$planId:destroy' : null;
}

/** Final strict-C names for one direct optional value. */
class CLoweredBodyOptional {
	public final prepared:CPreparedBodyOptional;
	public final cTag:CIdentifier;
	public final presenceName:CIdentifier;
	public final payloadName:CIdentifier;
	public final retainName:Null<CIdentifier>;
	public final destroyName:Null<CIdentifier>;
	public final retainParameterName:Null<CIdentifier>;
	public final destroyParameterName:Null<CIdentifier>;
	public final retainStatusName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyOptional, cTag:CIdentifier, presenceName:CIdentifier, payloadName:CIdentifier, retainName:Null<CIdentifier>,
			destroyName:Null<CIdentifier>, retainParameterName:Null<CIdentifier>, destroyParameterName:Null<CIdentifier>, retainStatusName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.presenceName = presenceName;
		this.payloadName = payloadName;
		this.retainName = retainName;
		this.destroyName = destroyName;
		this.retainParameterName = retainParameterName;
		this.destroyParameterName = destroyParameterName;
		this.retainStatusName = retainStatusName;
	}
}

/** Request-local registry that deduplicates optional values by payload type. */
class CBodyOptionalRegistry {
	final context:CompilationContext;
	final bySemanticKey:Map<String, CPreparedBodyOptional> = [];

	public function new(context:CompilationContext) {
		this.context = context;
	}

	/**
		Prepare the optional wrapper for one already-admitted direct value.

		The payload check is recursive. Unmanaged values need no lifecycle work. A
		managed closed record or enum is admitted only after it has one complete
		typed retain/destroy plan; the presence flag then guards every use of that
		plan.
	**/
	public function require(payload:CBodyValueType, ownerModule:String, sourcePath:String, position:Position, fail:(Position, String) -> Void,
			node:String):CPreparedBodyOptional {
		final payloadAggregate = payload.aggregateValue();
		final payloadEnum = payload.enumValue();
		final managedLifetime = (payloadAggregate != null && payloadAggregate.managedLifetime)
			|| (payloadEnum != null && payloadEnum.managedLifetime);
		if (!isDirectUnmanaged(payload) && !managedLifetime) {
			fail(position, '$node:payload-requires-direct-unmanaged-value:${payload.cSpelling}');
			throw new CBodyEmissionError("direct optional rejection callback returned unexpectedly");
		}
		final semanticKey = 'direct-optional-v1(${payload.cSpelling})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final typeRequest = new CSymbolRequest(CSKType, ["compiler", "optional", digest], CNSTag("translation-unit"), CSVInternal, null, [], [], null,
			["optional", readablePayload(payload)]);
		context.symbols.register(typeRequest);
		final presenceRequest = memberRequest(typeRequest, "has-value", 0, ["has", "value"]);
		final payloadRequest = memberRequest(typeRequest, "value", 1, ["value"]);
		context.symbols.register(presenceRequest);
		context.symbols.register(payloadRequest);
		// The wrapper is part of the payload's representation, not of whichever
		// function happened to mention `Null<T>` first. Owning it beside T keeps
		// split/package headers deterministic and prevents accidental module cycles.
		final representationOwner = if (payloadAggregate != null) payloadAggregate.ownerModule else if (payloadEnum != null) payloadEnum.ownerModule else
			staticStringOwner(payload, ownerModule);
		final prepared = new CPreparedBodyOptional(semanticKey, digest, payload, representationOwner, HaxeSourceSpan.fromPosition(position, sourcePath),
			position, typeRequest, presenceRequest, payloadRequest, managedLifetime);
		if (managedLifetime)
			registerLifecycle(prepared);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	public function canonicalOptionals():Array<CPreparedBodyOptional> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> compareUtf8(left.semanticKey, right.semanticKey));
		return values;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyOptional>
		return canonicalOptionals().map(value -> new CLoweredBodyOptional(value, symbols.identifierFor(value.typeRequest),
			symbols.identifierFor(value.presenceRequest), symbols.identifierFor(value.payloadRequest),
			value.retainRequest == null ? null : symbols.identifierFor(value.retainRequest),
			value.destroyRequest == null ? null : symbols.identifierFor(value.destroyRequest),
			value.retainParameterRequest == null ? null : symbols.identifierFor(value.retainParameterRequest),
			value.destroyParameterRequest == null ? null : symbols.identifierFor(value.destroyParameterRequest),
			value.retainStatusRequest == null ? null : symbols.identifierFor(value.retainStatusRequest)));

	function registerLifecycle(value:CPreparedBodyOptional):Void {
		final root = ["compiler", "optional", value.digest, "lifecycle"];
		value.retainRequest = new CSymbolRequest(CSKMethod, root.concat(["retain"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 0,
			["optional", value.digest.substr(0, 8), "retain"]);
		value.destroyRequest = new CSymbolRequest(CSKMethod, root.concat(["destroy"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 1,
			["optional", value.digest.substr(0, 8), "destroy"]);
		context.symbols.register(value.retainRequest);
		context.symbols.register(value.destroyRequest);
		value.retainParameterRequest = lifecycleLocal(value.retainRequest, "value", 0);
		value.destroyParameterRequest = lifecycleLocal(value.destroyRequest, "value", 0);
		value.retainStatusRequest = lifecycleLocal(value.retainRequest, "operation_status", 1);
	}

	function lifecycleLocal(owner:CSymbolRequest, role:String, ordinal:Int):CSymbolRequest {
		final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], ordinal,
			[role]);
		context.symbols.register(request);
		return request;
	}

	static function isDirectUnmanaged(value:CBodyValueType):Bool {
		return switch value.kind {
			case CBVKPrimitive(mapping): mapping.irType != IRTVoid && mapping.irType != IRTString;
			case CBVKStaticString(_): true;
			case CBVKEnum(enumValue): !enumValue.managedLifetime;
			case CBVKAggregate(aggregate):
				var direct = aggregate.fields.length > 0;
				for (field in aggregate.fields)
					if (!isDirectUnmanaged(field.type))
						direct = false;
				direct;
			case _:
				false;
		};
	}

	static function readablePayload(payload:CBodyValueType):String {
		final aggregate = payload.aggregateValue();
		if (aggregate != null)
			return aggregate.displayName == null ? "record" : aggregate.displayName;
		final enumValue = payload.enumValue();
		if (enumValue != null)
			return enumValue.displayName;
		final stringIdentity = payload.staticStringIdentity();
		return stringIdentity == null ? "value" : stringIdentity;
	}

	/** Own an abstract String wrapper beside its declaration when possible. */
	static function staticStringOwner(payload:CBodyValueType, fallback:String):String {
		final identity = payload.staticStringIdentity();
		return identity != null && identity.indexOf(".") != -1 ? identity : fallback;
	}

	static function memberRequest(owner:CSymbolRequest, role:String, ordinal:Int, readable:Array<String>):CSymbolRequest
		return new CSymbolRequest(CSKField, owner.qualifiedName.concat([role]), CNSMember(owner.stableKey()), CSVInternal, null, [], [], ordinal, readable);

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = haxe.io.Bytes.ofString(left);
		final rightBytes = haxe.io.Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}
#else
class CBodyOptional {
	public function new() {}
}
#end
