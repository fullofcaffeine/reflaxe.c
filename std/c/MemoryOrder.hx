package c;

/** C11 atomic memory order; operations must never silently choose one. */
enum abstract MemoryOrder(String) to String {
	var Relaxed = "relaxed";
	var Consume = "consume";
	var Acquire = "acquire";
	var Release = "release";
	var AcquireRelease = "acq_rel";
	var SequentiallyConsistent = "seq_cst";
}
