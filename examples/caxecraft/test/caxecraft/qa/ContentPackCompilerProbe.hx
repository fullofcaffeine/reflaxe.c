package caxecraft.qa;

import caxecraft.content.BaseContentPack;

/** Proves that validated generated Haxe facts lower to C without an owned C table. */
final class ContentPackCompilerProbe {
	/**
	 * Keeps the scalar result observable to generated C without selecting I/O.
	 *
	 * The Eval probe checks the exact value. This C-side probe separately proves
	 * that the same generated Haxe functions survive reachability, lower, and
	 * store their result in a build that selects no `hxrt` runtime features.
	 */
	static var observedProof:Int = 0;

	static function main():Void {
		observedProof = BaseContentPack.compilerProof();
	}
}
