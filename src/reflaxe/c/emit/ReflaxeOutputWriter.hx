package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import haxe.Json;
import haxe.io.Bytes;
import haxe.io.Path;
import reflaxe.output.OutputManager;
import sys.FileSystem;
import sys.io.File;

private typedef ReflaxeOwnershipMetadata = {
	final version:Int;
	final id:Int;
	final wasCached:Bool;
	final filesGenerated:Array<String>;
}

/** Validates the ownership boundary before delegating every write to Reflaxe. */
class ReflaxeOutputWriter {
	public function new() {}

	public function write(output:OutputManager, generatedFiles:Array<GeneratedFile>):Void {
		final outputDir = output.outputDir;
		if (outputDir == null) {
			fail("Reflaxe output manager has no output directory");
			return;
		}
		final destination:String = outputDir;
		if (StringTools.trim(destination) == "") {
			fail("Reflaxe output manager has no output directory");
			return;
		}
		final root = canonicalPlannedPath(destination);
		if (FileSystem.exists(destination) && !FileSystem.isDirectory(destination)) {
			fail('Reflaxe output root is not a directory: `$outputDir`');
		}

		final owned = loadAndValidateOwnership(destination, root);
		final files = generatedFiles.copy();
		files.sort(compareFiles);
		var previous:Null<String> = null;
		for (file in files) {
			file.verifyIntegrity();
			if (previous == file.relativePath) {
				fail('duplicate generated artifact path `${file.relativePath}`', [file.relativePath]);
			}
			previous = file.relativePath;
			validateExistingPrefixes(destination, root, file.relativePath);
			final target = Path.join([destination, file.relativePath]);
			if (FileSystem.exists(target) && !owned.exists(file.relativePath)) {
				fail('refusing to overwrite existing file not listed in `${GeneratedFile.OWNERSHIP_MANIFEST}`: `${file.relativePath}`', [file.relativePath]);
			}
			if (FileSystem.exists(target) && FileSystem.isDirectory(target)) {
				fail('generated artifact path is an existing directory: `${file.relativePath}`', [file.relativePath]);
			}
		}

		for (file in files) {
			output.saveFile(file.relativePath, file.contents);
		}
	}

	function loadAndValidateOwnership(outputDir:String, root:String):Map<String, Bool> {
		final owned:Map<String, Bool> = [];
		final metadataPath = Path.join([outputDir, GeneratedFile.OWNERSHIP_MANIFEST]);
		if (!FileSystem.exists(metadataPath)) {
			return owned;
		}
		validateExistingPrefixes(outputDir, root, GeneratedFile.OWNERSHIP_MANIFEST);
		if (FileSystem.isDirectory(metadataPath)) {
			fail('Reflaxe ownership manifest is a directory: `${GeneratedFile.OWNERSHIP_MANIFEST}`');
		}
		final metadata = decodeOwnershipMetadata(metadataPath);
		for (path in metadata.filesGenerated) {
			if (!Std.isOfType(path, String)) {
				fail('Reflaxe ownership manifest contains a non-string generated path');
			}
			if (!GeneratedFile.isNormalizedRelativePath(path) || path == GeneratedFile.OWNERSHIP_MANIFEST) {
				fail('Reflaxe ownership manifest contains an unsafe generated path: `$path`');
			}
			if (owned.exists(path)) {
				fail('Reflaxe ownership manifest contains duplicate path `$path`', [path]);
			}
			validateExistingPrefixes(outputDir, root, path);
			owned.set(path, true);
		}
		return owned;
	}

	function decodeOwnershipMetadata(metadataPath:String):ReflaxeOwnershipMetadata {
		try {
			// `Json.parse` is the one irreducibly untyped input operation. Its
			// value is captured and validated here; only this typed record escapes.
			final metadata:ReflaxeOwnershipMetadata = Json.parse(File.getContent(metadataPath));
			if (metadata == null
				|| metadata.version != 1
				|| !Std.isOfType(metadata.id, Int)
				|| metadata.id < 0
				|| !Std.isOfType(metadata.wasCached, Bool)
				|| !Std.isOfType(metadata.filesGenerated, Array)) {
				fail('Reflaxe ownership manifest has an invalid schema: `${GeneratedFile.OWNERSHIP_MANIFEST}`');
			}
			return metadata;
		} catch (error:ProjectEmissionError) {
			throw error;
		} catch (_:haxe.Exception) {
			throw new ProjectEmissionError('Reflaxe ownership manifest is not valid schema-1 JSON: `${GeneratedFile.OWNERSHIP_MANIFEST}`');
		}
	}

	function validateExistingPrefixes(outputDir:String, canonicalRoot:String, relativePath:String):Void {
		var current = outputDir;
		var expected = canonicalRoot;
		for (part in relativePath.split("/")) {
			final parent = current;
			current = Path.join([current, part]);
			expected = joinCanonical(expected, part);
			if (!FileSystem.exists(current)) {
				if (directoryContainsEntry(parent, part)) {
					fail('generated output path contains a dangling link or inaccessible entry: `$relativePath`', [relativePath]);
				}
				return;
			}
			final actual = normalizeCanonical(FileSystem.fullPath(current));
			if (actual != normalizeCanonical(expected)) {
				fail('generated output path crosses a symbolic link or escapes the output root: `$relativePath`', [relativePath]);
			}
		}
	}

	function canonicalPlannedPath(path:String):String {
		if (FileSystem.exists(path)) {
			return normalizeCanonical(FileSystem.fullPath(path));
		}
		var current = FileSystem.absolutePath(path);
		final suffix:Array<String> = [];
		while (!FileSystem.exists(current)) {
			final name = Path.withoutDirectory(current);
			final parent = Path.directory(current);
			if (name == "" || parent == "" || parent == current) {
				fail('cannot establish a canonical parent for output root `$path`');
			}
			if (directoryContainsEntry(parent, name)) {
				fail('output root contains a dangling link or inaccessible entry: `$path`');
			}
			suffix.unshift(name);
			current = parent;
		}
		if (suffix.length > 0 && !FileSystem.isDirectory(current)) {
			fail('output root has a non-directory ancestor: `$path`');
		}
		var result = normalizeCanonical(FileSystem.fullPath(current));
		for (part in suffix) {
			result = joinCanonical(result, part);
		}
		return normalizeCanonical(result);
	}

	static function directoryContainsEntry(directory:String, name:String):Bool {
		if (!FileSystem.exists(directory) || !FileSystem.isDirectory(directory)) {
			return false;
		}
		try {
			final windows = Sys.systemName() == "Windows";
			final expected = windows ? name.toLowerCase() : name;
			for (entry in FileSystem.readDirectory(directory)) {
				if ((windows ? entry.toLowerCase() : entry) == expected) {
					return true;
				}
			}
		} catch (_:haxe.Exception) {
			fail('cannot inspect output directory `$directory` safely');
		}
		return false;
	}

	static function joinCanonical(parent:String, child:String):String
		return normalizeCanonical(parent + "/" + child);

	static function normalizeCanonical(path:String):String {
		var result = StringTools.replace(Path.normalize(path), "\\", "/");
		while (result.length > 1 && StringTools.endsWith(result, "/")) {
			result = result.substr(0, result.length - 1);
		}
		if (Sys.systemName() == "Windows") {
			result = result.toLowerCase();
		}
		return result;
	}

	static function compareFiles(left:GeneratedFile, right:GeneratedFile):Int
		return compareUtf8(left.relativePath, right.relativePath);

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function fail(detail:String, ?paths:Array<String>):Void
		throw new ProjectEmissionError(detail, paths);
}
#else
class ReflaxeOutputWriter {
	public function new() {}
}
#end
