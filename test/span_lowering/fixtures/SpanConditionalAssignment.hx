import c.Span;
import c.UInt8;

/**
	Module-level span operation used to prove bounds across right-side control flow.

	The span and index are evaluated before the conditional value. Their bounds
	proof must still govern the eventual store in the conditional's join block.
**/
function assignSelected(values:Span<UInt8>, index:Int, whenTrue:UInt8, whenFalse:UInt8, selectTrue:Bool):UInt8 {
	values[index] = selectTrue ? whenTrue : whenFalse;
	return values[index];
}

/**
	Inline the same operation into its caller so the destination index becomes a
	caller-block value rather than a function parameter.

	This is the source shape that exposed the compiler bug in Caxecraft: the
	index is evaluated before the conditional value, but the final store happens
	in the conditional's join block. Haxe.c must save that already-computed index
	across the branches instead of re-evaluating it or using a value outside the
	HxcIR block where it was created.
**/
inline function assignSelectedInline(values:Span<UInt8>, index:Int, whenTrue:UInt8, whenFalse:UInt8, selectTrue:Bool):UInt8 {
	values[index] = selectTrue ? whenTrue : whenFalse;
	return values[index];
}
