/** Base interface whose method is called through the child `CounterView`. */
interface ReadableView {
	function read(delta:Int):Int;
}
