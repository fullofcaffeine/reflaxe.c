package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type;
import reflaxe.c.ast.CAST.CIdentifier;

/** Stable typed-input facts for one reachable concrete Haxe constructor. */
typedef CBodyConstructorInput = {
	final id:String;
	final modulePath:String;
	final declarationPath:String;
	final sourcePath:String;
	final sourceOrder:Int;
	final fieldType:Type;
	final expression:TypedExpr;
	final classReference:Ref<ClassType>;
	final baseConstructorId:Null<String>;
	final elided:Bool;
	final canFail:Bool;
}

/** Reviewable outcome for one reachable constructor after C names are finalized. */
class CLoweredBodyConstructor {
	public final id:String;
	public final haxePath:String;
	public final instanceId:String;
	public final elided:Bool;
	public final canFail:Bool;
	public final cName:Null<CIdentifier>;

	public function new(id:String, haxePath:String, instanceId:String, elided:Bool, canFail:Bool, cName:Null<CIdentifier>) {
		this.id = id;
		this.haxePath = haxePath;
		this.instanceId = instanceId;
		this.elided = elided;
		this.canFail = canFail;
		this.cName = cName;
	}
}

/** Stable constructor identities shared by discovery, lowering, and reports. */
class CBodyConstructor {
	private function new() {}

	public static function id(haxePath:String):String
		return 'constructor.$haxePath';

	public static function classPath(reference:Ref<ClassType>):String {
		final value = reference.get();
		return value.pack.concat([value.name]).join(".");
	}
}
#else
class CBodyConstructor {
	public function new() {}
}
#end
