class Main {
	static function main():Void {
		try {
			trace("not admitted");
		} catch (error:String) {
			trace(error);
		}
	}

	/**
		Keep a nested unsupported type behind a representation-free `null`.

		The semantic-function replay prepass must not inspect this inferred
		`Null<UnsupportedCacheRequest>` before ordinary lowering reaches the
		source-anchored `try` expression above. A bare null has no storage layout;
		its eventual declaration or use owns that representation decision.
	**/
	static var activeRequest:Null<UnsupportedCacheRequest> = null;
}

/** A deliberately unsupported nested carrier used only by the diagnostic fixture. */
private typedef UnsupportedCacheRequest = {
	final priorByPath:Map<String, UnsupportedPersistedDigest>;
}

/** One ordinary value inside the deliberately unsupported Map carrier. */
private typedef UnsupportedPersistedDigest = {
	final digest:String;
}
