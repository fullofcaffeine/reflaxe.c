/**
	Defines the neighboring value graph that proves collector selection is narrow.
**/

/** One by-value enum/record shape with no path back to its owning Array. */
enum AcyclicNode {
	Empty;
	Item(value:AcyclicRecord);
}

/** A scalar-only record keeps its containing Array on the selective RC path. */
typedef AcyclicRecord = {
	final label:Int;
}
