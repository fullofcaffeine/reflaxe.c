/**
 * Keeps one embedded child's identity across two branch arms and their join.
 *
 * Inlining exposes the borrowed receiver without replacing the ordinary class
 * contract. The read after the conditional must refer to the same live child.
 */

/** Provides two distinct values so the independent oracle exercises both arms. */
private final class BranchChild {
	final left:Int;
	final right:Int;

	/** Initialize the complete child before its parent exposes it. */
	public function new(left:Int, right:Int) {
		this.left = left;
		this.right = right;
	}

	/** Read the borrowed receiver in either arm, then once more after the join. */
	public inline function selectAndAddLeft(chooseLeft:Bool):Int {
		final selected = if (chooseLeft) left else right;
		return selected + left;
	}
}

/** Owns a child whose address does not escape this parent's lifetime. */
final class OwnedBranch {
	final child:BranchChild = new BranchChild(19, 23);

	/** Complete embedded initialization before any branch can read the child. */
	public function new() {}

	/** Preserve a method boundary while exposing the inline child's diamond. */
	public function total(chooseLeft:Bool):Int
		return child.selectAndAddLeft(chooseLeft);
}
