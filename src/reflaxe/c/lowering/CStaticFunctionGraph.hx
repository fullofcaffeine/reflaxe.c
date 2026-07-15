package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLowering.CBodyGlobalInput;

/** Reachable static-function inputs plus the semantic executable entry ID. */
class CStaticFunctionGraph {
	public final entryFunctionId:String;
	public final functions:Array<CBodyFunctionInput>;
	public final globals:Array<CBodyGlobalInput>;

	public function new(entryFunctionId:String, functions:Array<CBodyFunctionInput>, globals:Array<CBodyGlobalInput>) {
		this.entryFunctionId = entryFunctionId;
		this.functions = functions.copy();
		this.globals = globals.copy();
	}
}

/** Collects only direct static targets reachable from the typed Haxe entry body. */
class CStaticFunctionGraphCollector {
	public function new() {}

	public function collect(entry:CBodyFunctionInput, program:TypedProgramInput):CStaticFunctionGraph {
		final available = staticFunctionInputs(program);
		final byId:Map<String, CBodyFunctionInput> = [];
		final pending:Array<CBodyFunctionInput> = [];
		add(entry, byId, pending);
		available.set(CBodyLowering.functionId(entry.declarationPath, entry.fieldName), entry);
		var index = 0;
		while (index < pending.length) {
			collectExpression(pending[index++].expression, available, byId, pending);
		}
		final functions = [for (fn in byId) fn];
		functions.sort(compareInputs);
		return new CStaticFunctionGraph(CBodyLowering.functionId(entry.declarationPath, entry.fieldName), functions, staticGlobalInputs(program));
	}

	function collectExpression(expression:TypedExpr, available:Map<String, CBodyFunctionInput>, byId:Map<String, CBodyFunctionInput>,
			pending:Array<CBodyFunctionInput>):Void {
		switch expression.expr {
			case TCall(callee, _):
				final targetId = directStaticFunctionId(callee);
				final target = targetId == null ? null : available.get(targetId);
				if (target != null) {
					add(target, byId, pending);
				}
			case _:
		}
		TypedExprTools.iter(expression, child -> collectExpression(child, available, byId, pending));
	}

	static function directStaticFunctionId(callee:TypedExpr):Null<String> {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				CBodyLowering.functionId(owner.pack.concat([owner.name]).join("."), fieldReference.get().name);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStaticFunctionId(inner);
			case _: null;
		};
	}

	static function staticFunctionInputs(program:TypedProgramInput):Map<String, CBodyFunctionInput> {
		final result:Map<String, CBodyFunctionInput> = [];
		for (declaration in program.declarations) {
			for (field in declaration.fields) {
				if (field.role != "static" || field.expression == null || field.rawClassField == null) {
					continue;
				}
				switch field.rawClassField.kind {
					case FMethod(_):
						final input:CBodyFunctionInput = {
							modulePath: declaration.ownerModulePath,
							declarationPath: declaration.path,
							sourcePath: declaration.sourcePath,
							fieldName: field.name,
							sourceOrder: field.sourceOrder,
							fieldType: field.rawClassField.type,
							expression: field.expression
						};
						result.set(CBodyLowering.functionId(input.declarationPath, input.fieldName), input);
					case FVar(_, _):
				}
			}
		}
		return result;
	}

	static function staticGlobalInputs(program:TypedProgramInput):Array<CBodyGlobalInput> {
		final result:Array<CBodyGlobalInput> = [];
		for (declaration in program.declarations) {
			for (field in declaration.fields) {
				if (field.role != "static" || field.rawClassField == null) {
					continue;
				}
				switch field.rawClassField.kind {
					case FVar(_, write):
						result.push({
							modulePath: declaration.ownerModulePath,
							declarationPath: declaration.path,
							sourcePath: declaration.sourcePath,
							fieldName: field.name,
							sourceOrder: field.sourceOrder,
							fieldType: field.rawClassField.type,
							mutable: write != AccNever,
							position: field.rawClassField.pos,
							expression: field.expression
						});
					case FMethod(_):
				}
			}
		}
		result.sort((left, right) -> {
			final identity = compareUtf8('${left.declarationPath}\u0000${left.fieldName}', '${right.declarationPath}\u0000${right.fieldName}');
			return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
		});
		return result;
	}

	static function add(input:CBodyFunctionInput, byId:Map<String, CBodyFunctionInput>, pending:Array<CBodyFunctionInput>):Void {
		final id = CBodyLowering.functionId(input.declarationPath, input.fieldName);
		if (!byId.exists(id)) {
			byId.set(id, input);
			pending.push(input);
		}
	}

	static function compareInputs(left:CBodyFunctionInput, right:CBodyFunctionInput):Int {
		final identity = compareUtf8(CBodyLowering.functionId(left.declarationPath, left.fieldName),
			CBodyLowering.functionId(right.declarationPath, right.fieldName));
		return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
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
#else
class CStaticFunctionGraphCollector {
	public function new() {}
}
#end
