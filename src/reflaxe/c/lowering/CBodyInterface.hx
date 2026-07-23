package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;

/** Module anchor for nominal Haxe-interface identities used by body lowering. */
class CBodyInterface {
	private function new() {}
}

/**
	One closed, non-generic Haxe interface before C representation is selected.

	An interface is nominal: two declarations with identical methods remain
	different Haxe types. The stable semantic key therefore uses the complete
	package-qualified Haxe path rather than a structural method hash.
**/
class CPreparedBodyInterface {
	public final semanticKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final haxePath:String;
	public final ownerModule:String;
	public final source:HxcSourceSpan;

	public function new(semanticKey:String, digest:String, haxePath:String, ownerModule:String, source:HxcSourceSpan) {
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.interface.$digest';
		this.instanceId = 'instance.interface.$digest';
		this.haxePath = haxePath;
		this.ownerModule = ownerModule;
		this.source = source;
	}

	/** Keep interface identity semantic; the later C layer chooses the fat-value struct. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: haxePath,
			kind: IRTKReference,
			source: source
		};

	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [],
			representation: IRRDirect,
			source: source
		};
}

/** Request-local nominal interface discovery with deterministic output order. */
class CBodyInterfaceRegistry {
	final sourcePathsByModule:Map<String, String> = [];
	final byPath:Map<String, CPreparedBodyInterface> = [];
	final semanticKeysByDigest:Map<String, String> = [];

	public function new(?program:TypedProgramInput) {
		if (program != null)
			for (module in program.modules)
				sourcePathsByModule.set(module.path, module.sourcePath);
	}

	public function require(reference:Ref<ClassType>, parameters:Array<Type>, position:Position, ownerSourcePath:String, fail:(Position, String) -> Void,
			node:String):CPreparedBodyInterface {
		final definition = reference.get();
		final path = classPath(definition);
		if (!definition.isInterface)
			return rejected(fail, position, '$node:not-interface:$path');
		if (definition.isExtern)
			return rejected(fail, position, '$node:extern-interface:$path');
		switch definition.kind {
			case KNormal:
			case KTypeParameter(_):
				return rejected(fail, position, '$node:open-interface-type-parameter:$path');
			case _:
				return rejected(fail, position, '$node:unsupported-interface-kind:${Std.string(definition.kind)}:$path');
		}
		if (definition.params.length != 0 || parameters.length != 0)
			return rejected(fail, position, '$node:generic-interface-requires-specialization:$path');
		final existing = byPath.get(path);
		if (existing != null)
			return existing;

		final semanticKey = 'haxe-interface-v1(${canonicalPart(path)})';
		final digest = Sha256.encode(semanticKey);
		final prior = semanticKeysByDigest.get(digest);
		if (prior != null && prior != semanticKey)
			throw new CBodyEmissionError('interface-identity digest collision `$digest` between `$prior` and `$semanticKey`');
		semanticKeysByDigest.set(digest, semanticKey);
		final sourcePath = sourcePathsByModule.exists(definition.module) ? sourcePathsByModule.get(definition.module) : ownerSourcePath;
		if (sourcePath == null)
			return rejected(fail, position, '$node:missing-source-for-interface:$path');
		final prepared = new CPreparedBodyInterface(semanticKey, digest, path, definition.module, HaxeSourceSpan.fromPosition(definition.pos, sourcePath));
		byPath.set(path, prepared);
		return prepared;
	}

	public function canonicalInterfaces():Array<CPreparedBodyInterface> {
		final values = [for (value in byPath) value];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		return values;
	}

	static function classPath(value:ClassType):String
		return value.pack.concat([value.name]).join(".");

	static function canonicalPart(value:String):String {
		final bytes = Bytes.ofString(value);
		return '${bytes.length}:$value';
	}

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

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw "unreachable";
	}
}
#else
class CBodyInterface {
	private function new() {}
}
#end
