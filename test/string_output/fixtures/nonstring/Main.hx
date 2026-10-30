/** Keeps record formatting fail-closed until its text contract is implemented. */
class Main {
	/** A typed record must not silently use scalar or String formatting. */
	static function main():Void {
		Sys.println({value: 1});
	}
}
