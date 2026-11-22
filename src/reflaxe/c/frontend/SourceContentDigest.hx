package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.io.Path;
import reflaxe.c.CContentDigest.sha256Hex;

/**
	Avoid hashing identical source bytes twice within one capture request.

	Function and record provenance keep separate file snapshots and error rules.
	Both still read the file when their own rules require it. This owner shares
	only a digest after comparing those newly read bytes with a private copy.
	An editor save therefore remains visible to the later reader.
**/
class SourceContentDigest {
	final entries:Map<String, SourceDigestEntry> = [];

	/** Start an empty memo whose lifetime ends with one normalization request. */
	public function new() {}

	/**
		Reuse a digest only after comparing the caller's freshly read bytes.

		A lexical absolute path groups likely matches without resolving symlinks or
		changing either caller's read path or snapshot keys. Different files can
		share this hint, so only equal bytes authorize reuse. If the current
		directory cannot be read, a relative hint still preserves that rule.
		Copy on a miss prevents a caller from mutating the retained comparison
		buffer after this method returns. No bytes leave this owner.
	**/
	public function digest(path:String, bytes:Bytes):String {
		var key = Path.normalize(path);
		if (!Path.isAbsolute(key)) {
			final directory:Null<String> = try Sys.getCwd() catch (_:haxe.Exception) null;
			if (directory != null)
				key = Path.join([directory, key]);
		}
		final previous = entries.get(key);
		if (previous != null && previous.bytes.length == bytes.length && previous.bytes.compare(bytes) == 0)
			return previous.digest;
		final digest = sha256Hex(bytes);
		entries.set(key, {bytes: bytes.sub(0, bytes.length), digest: digest});
		return digest;
	}
}

/** A private byte copy keeps the equality check independent of caller mutation. */
private typedef SourceDigestEntry = {
	final bytes:Bytes;
	final digest:String;
}
#end
