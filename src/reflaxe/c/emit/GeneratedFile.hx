package reflaxe.c.emit;

/** A normalized, output-root-relative compiler artifact. */
typedef GeneratedFile = {
	final relativePath:String;
	final contents:String;
}
