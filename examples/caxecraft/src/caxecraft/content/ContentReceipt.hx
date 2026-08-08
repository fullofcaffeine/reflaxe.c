package caxecraft.content;

/**
 * Holds the byte count and digest for one package-relative content file.
 *
 * Refresh planning computes these values from final in-memory bytes. Manifest
 * writers then use the same value, so no writer can hash a different version.
 */
final class ContentReceipt {
	/** Package-relative file name that this receipt describes. */
	public final logicalPath:String;

	/** Exact number of bytes in the final file. */
	public final byteLength:Int;

	/** Lowercase SHA-256 digest of the final file. */
	public final sha256:String;

	/** Retain one complete receipt that a caller computed from owned bytes. */
	public function new(logicalPath:String, byteLength:Int, sha256:String) {
		this.logicalPath = logicalPath;
		this.byteLength = byteLength;
		this.sha256 = sha256;
	}
}
