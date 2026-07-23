import haxe.io.Bytes;

/** Small value-only bounds record reduced from Caxecraft's editor history. */
typedef HistoryBounds = {
	final minimum:Int;
	final maximum:Int;
}

/**
	A reduced editor-history record with two shared binary buffers.

	The Array owns a logical copy of this record. Copying the record must retain
	each `Bytes` reference, while removing the record or destroying the Array must
	release each reference exactly once. Scalar fields remain direct unboxed C.
**/
typedef HistoryEntry = {
	final revision:Int;
	final before:Bytes;
	final after:Bytes;
	final bounds:Null<HistoryBounds>;
}
