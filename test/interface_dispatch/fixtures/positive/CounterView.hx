/**
 * Child interface used to prove that inherited methods keep the child's exact
 * interface identity. A `CounterView` value therefore owns one compact table
 * containing both `read`, inherited from `ReadableView`, and `doubled`.
 */
interface CounterView extends ReadableView {
	function doubled():Int;
}
