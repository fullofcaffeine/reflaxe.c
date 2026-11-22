/**
	Defines the enum and record edge used by the cyclic Array fixture.
**/

/**
	One tagged edge used to build cyclic collection graphs without a class object.

	`Linked` owns a record whose `next` field is another shared Array identity.
	The tag therefore decides whether tracing may read the overlapping payload.
**/
enum GraphNode {
	Empty;
	Linked(edge:GraphEdge);
	Marker(value:Int);
}

/** A by-value record that connects one tagged node to another Array. */
typedef GraphEdge = {
	final label:Int;
	final next:Array<GraphNode>;
}
