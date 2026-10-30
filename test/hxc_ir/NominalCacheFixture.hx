/** Supplies one ordinary class whose representation belongs to each compiler request. */
class NominalCacheFixture {
	/** A scalar field makes the class layout observable to the type classifier. */
	public final value:Int;

	/** Store the value without introducing an unrelated runtime dependency. */
	public function new(value:Int) {
		this.value = value;
	}

	/** The macro owns the assertions; target execution has no work. */
	static function main():Void {}
}
