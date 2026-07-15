package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;

/** Reachable static-function inputs plus the semantic executable entry ID. */
class CStaticFunctionGraph {
	public final entryFunctionId:String;
	public final functions:Array<CBodyFunctionInput>;

	public function new(entryFunctionId:String, functions:Array<CBodyFunctionInput>) {
		this.entryFunctionId = entryFunctionId;
		this.functions = functions.copy();
	}
}

/** Collects only direct static targets reachable from the typed Haxe entry body. */
class CStaticFunctionGraphCollector {
	public function new() {}

	public function collect(entry:CBodyFunctionInput):CStaticFunctionGraph {
		final byId:Map<String, CBodyFunctionInput> = [];
		final pending:Array<CBodyFunctionInput> = [];
		add(entry, byId, pending);
		var index = 0;
		while (index < pending.length) {
			collectExpression(pending[index++].expression, byId, pending);
		}
		final functions = [for (fn in byId) fn];
		functions.sort(compareInputs);
		return new CStaticFunctionGraph(CBodyLowering.functionId(entry.declarationPath, entry.fieldName), functions);
	}

	function collectExpression(expression:TypedExpr, byId:Map<String, CBodyFunctionInput>, pending:Array<CBodyFunctionInput>):Void {
		switch expression.expr {
			case TCall(callee, _):
				final target = directStaticTarget(callee);
				if (target != null) {
					add(target, byId, pending);
				}
			case _:
		}
		TypedExprTools.iter(expression, child -> collectExpression(child, byId, pending));
	}

	static function directStaticTarget(callee:TypedExpr):Null<CBodyFunctionInput> {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				final field = fieldReference.get();
				final expression = field.expr();
				if (expression == null) {
					null;
				} else {
					{
						modulePath: owner.module,
						declarationPath: owner.pack.concat([owner.name]).join("."),
						sourcePath: owner.module.split(".").join("/") + ".hx",
						fieldName: field.name,
						sourceOrder: sourceOrder(owner, field.name),
						fieldType: field.type,
						expression: expression
					};
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStaticTarget(inner);
			case _: null;
		};
	}

	static function sourceOrder(owner:ClassType, fieldName:String):Int {
		final fields = owner.statics.get();
		for (index in 0...fields.length) {
			if (fields[index].name == fieldName) {
				return index;
			}
		}
		return 0;
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
