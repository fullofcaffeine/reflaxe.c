package caxecraft.content;

import caxecraft.content.ContentPackageModel.ContentPackageReadResult;

/**
 * Supplies package bytes by validated logical name, independent of transport.
 *
 * The confined directory store and bounded ZIP reader implement this same
 * narrow operation, so schema decoding, role checks, receipts, and gameplay
 * never branch on how the package was shared.
 */
interface ContentPackageSource {
	/** Read one package-relative file or return one closed source error. */
	function read(logicalPath:String):ContentPackageReadResult;
}
