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
