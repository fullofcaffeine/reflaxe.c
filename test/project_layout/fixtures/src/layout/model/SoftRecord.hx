package layout.model;

/** A complete record whose module only refers back through a prototype. */
typedef SoftRecord = {
	final value:Int;
}

/** The prototype's HardRecord value is legal with the shared struct forward. */
class SoftRecordApi {
	public static function read(record:HardRecord):Int {
		return record.soft.value;
	}
}
