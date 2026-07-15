package c;

#if macro
import haxe.macro.Context;
import haxe.macro.Expr;
import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;
#end

/** Source-positioned Haxe compile-time assertion; native layout assertions are emitted later. */
class StaticAssert {
	public static macro function require(condition:ExprOf<Bool>, message:ExprOf<String>):Expr {
		final detail = switch message.expr {
			case EConst(CString(value, _)): value;
			case _:
				CDiagnostic.error(CDiagnosticId.InvalidTypedCContract, "c.StaticAssert.require message must be a string literal", message.pos);
				"invalid static assertion";
		}

		switch condition.expr {
			case EConst(CIdent("true")):
			case EConst(CIdent("false")):
				CDiagnostic.error(CDiagnosticId.InvalidTypedCContract, 'static assertion failed: $detail', condition.pos);
			case _:
				CDiagnostic.error(CDiagnosticId.InvalidTypedCContract, "c.StaticAssert.require condition must be a compile-time Bool literal", condition.pos);
		}

		return macro {};
	}
}
