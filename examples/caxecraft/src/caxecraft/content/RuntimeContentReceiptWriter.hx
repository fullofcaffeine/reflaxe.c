package caxecraft.content;

import caxecraft.content.ContentJsonWriter.appendJsonString;
import caxecraft.content.RuntimeContentDigest.runtimeSha256Hex;
import haxe.io.Bytes;

/**
 * Builds the checked receipt that binds runtime content files together.
 *
 * The game and the authoring command share the same generation-digest rule.
 * A changed map therefore cannot receive a plausible receipt from a second
 * implementation that uses a different field order or hashing algorithm.
 */
/** Build the exact ordered bytes that identify one runtime content generation. */
function runtimeGenerationInput(assetManifestId:String, assetManifestSha256:String, content:ContentReceipt, ui:ContentReceipt, map:ContentReceipt):Bytes {
	final value = "asset-manifest\x00" + assetManifestId + "\x00" + assetManifestSha256 + "\n" + "content-pack\x00" + content.logicalPath + "\x00"
		+ content.sha256 + "\n" + "ui-catalog\x00" + ui.logicalPath + "\x00" + ui.sha256 + "\n" + "level-map\x00" + map.logicalPath + "\x00" + map.sha256 +
		"\n";
	return Bytes.ofString(value);
}

/** Compute the digest for the ordered asset, pack, UI, and map receipts. */
function runtimeGenerationSha256(assetManifestId:String, assetManifestSha256:String, content:ContentReceipt, ui:ContentReceipt, map:ContentReceipt):String {
	return runtimeSha256Hex(runtimeGenerationInput(assetManifestId, assetManifestSha256, content, ui, map));
}

/** Write one canonical runtime-content receipt from final source receipts. */
function writeRuntimeContentReceipt(assetManifestId:String, assetManifest:ContentReceipt, content:ContentReceipt, ui:ContentReceipt, map:ContentReceipt):Bytes {
	final generationSha256 = runtimeGenerationSha256(assetManifestId, assetManifest.sha256, content, ui, map);
	final output = new StringBuf();
	output.add('{\n  "assetManifest": {\n    "packId": ');
	appendJsonString(output, assetManifestId);
	output.add(',\n    "schemaVersion": 1,\n    "sha256": ');
	appendJsonString(output, assetManifest.sha256);
	output.add('\n  },\n  "files": [\n');
	appendFile(output, "content-pack", content, false);
	appendFile(output, "ui-catalog", ui, false);
	appendFile(output, "level-map", map, true);
	output.add('  ],\n  "generationSha256": ');
	appendJsonString(output, generationSha256);
	output.add(',\n  "schemaVersion": 1\n}\n');
	return Bytes.ofString(output.toString());
}

/** Append one fixed-role runtime file record. */
private function appendFile(output:StringBuf, kind:String, receipt:ContentReceipt, last:Bool):Void {
	output.add('    {\n      "byteLength": ${receipt.byteLength},\n      "kind": ');
	appendJsonString(output, kind);
	output.add(',\n      "path": ');
	appendJsonString(output, receipt.logicalPath);
	output.add(',\n      "sha256": ');
	appendJsonString(output, receipt.sha256);
	output.add(last ? '\n    }\n' : '\n    },\n');
}
