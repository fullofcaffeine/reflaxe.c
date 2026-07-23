package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;

/** Module anchor for exact `haxe.io.Bytes` recognition and representation. */
class CBodyBytes {
	private function new() {}
}

/**
	One closed Bytes identity before its C spelling is selected.

	Bytes is a mutable reference value: assigning it to another local shares the
	same storage. The managed instance records that source fact before CAST picks
	the private `hxc_bytes_ref *` representation.
**/
class CPreparedBodyBytes {
	public static inline final DECLARATION_ID = "type.haxe-io-bytes";
	public static inline final INSTANCE_ID = "instance.haxe-io-bytes";

	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final position:Position;

	public function new(ownerModule:String, source:HxcSourceSpan, position:Position) {
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
	}

	public function declaration():HxcIRTypeDeclaration
		return {
			id: DECLARATION_ID,
			displayName: "haxe.io.Bytes",
			kind: IRTKReference,
			source: source
		};

	public function instance():HxcIRTypeInstance
		return {
			id: INSTANCE_ID,
			declarationId: DECLARATION_ID,
			arguments: [],
			representation: IRRManaged("bytes"),
			source: source
		};
}

/** Request-local registry for the one nominal standard-library Bytes type. */
class CBodyBytesRegistry {
	var prepared:Null<CPreparedBodyBytes>;

	public function new() {}

	/** Return null for every type except the exact standard `haxe.io.Bytes`. */
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String):Null<CPreparedBodyBytes> {
		if (!CBodyBytesRecognition.isCoreBytesType(type))
			return null;
		if (prepared == null)
			prepared = new CPreparedBodyBytes(ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath), position);
		return prepared;
	}

	public function canonicalBytes():Array<CPreparedBodyBytes>
		return prepared == null ? [] : [prepared];
}

/** Exact declaration identity keeps Bytes operations out of class dispatch. */
class CBodyBytesRecognition {
	public static function isCoreBytesType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, parameters): parameters.length == 0 && isCoreBytes(reference);
			case _: false;
		};

	public static function isCoreBytes(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.length == 2 && value.pack[0] == "haxe" && value.pack[1] == "io" && value.name == "Bytes";
	}
}
#else
class CBodyBytes {
	private function new() {}
}
#end
