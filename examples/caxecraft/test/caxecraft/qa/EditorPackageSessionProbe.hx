package caxecraft.qa;

import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.RuntimeContentPack;
import caxecraft.editor.EditorPackageSession;
import caxecraft.editor.EditorPackageSession.EditorPackageOpenResult;
import caxecraft.editor.EditorPackageSession.EditorPackageSaveResult;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.scenario.ScenarioText;
import haxe.io.Bytes;
import sys.FileSystem;
import sys.io.File;

/**
 * Proves that native and text editors can save through one package session.
 *
 * The probe copies the checked package before it changes a level. It then uses
 * the active editor attachment used by the application, rejects stale and
 * malformed-package saves, publishes two valid revisions, and reopens the
 * result through the ordinary package path.
 */
final class EditorPackageSessionProbe {
	static inline final MANIFEST_PATH = "caxecraft.package.json";
	static inline final LEVEL_PATH = "scenarios/first-playable/map.caxemap";
	static inline final CONTENT_PATH = "packs/caxecraft/base/content.json";

	/** Run the package save scenarios without changing checked-in content. */
	public static function main():Void {
		final root = temporaryRoot();
		copyPackage(root);
		var failure:Null<haxe.Exception> = null;
		try
			runSaveContract(root)
		catch (error:haxe.Exception)
			failure = error;
		removeTree(root);
		if (failure != null)
			throw failure;
		Sys.println("editor-package-session: one shared session, rejected saves preserve the draft, canonical save/reopen and clean undo/redo passed");
	}

	/** Exercise one active editor session from attachment through final reopen. */
	static function runSaveContract(root:String):Void {
		final registry = switch RuntimeContentPack.decode(File.getBytes('$root/$CONTENT_PATH')) {
			case RuntimeContentPackReady(value): value;
			case RuntimeContentPackRejected(error): throw new haxe.Exception('content pack rejected: ${Std.string(error)}');
		};
		final active = switch EditorSession.openBytes(File.getBytes('$root/$LEVEL_PATH'), registry) {
			case EditorOpened(value): value;
			case EditorOpenRejected(error): throw new haxe.Exception('active editor rejected: ${Std.string(error)}');
		};
		final editorPackage = switch EditorPackageSession.attach(root, root, MANIFEST_PATH, LEVEL_PATH, active) {
			case EditorPackageOpened(value): value;
			case EditorPackageOpenRejected(error): throw new haxe.Exception('package attachment rejected: ${Std.string(error)}');
		};
		require(editorPackage.workspace() == active, "package attachment created a second editor session");
		require(!editorPackage.hasUnsavedChanges(), "new package attachment started dirty");

		applyTitle(editorPackage, "Editor save probe A");
		require(editorPackage.hasUnsavedChanges(), "accepted title edit did not make the package dirty");
		final firstDraft = active.canonicalDraft();
		final firstRevision = active.revision();
		final firstHistory = active.historyEntries();
		switch editorPackage.save(firstRevision - 1) {
			case EditorPackageSaveRejected(RevisionRejected(expected, actual)):
				require(expected == firstRevision && actual == firstRevision - 1, "stale save lost its revision facts");
			case EditorPackageSaved(_, _, _):
				throw new haxe.Exception("stale save changed package files");
			case EditorPackageSaveRejected(error):
				throw new haxe.Exception('stale save returned the wrong error: ${Std.string(error)}');
		}
		require(active.canonicalDraft().compare(firstDraft) == 0
			&& active.revision() == firstRevision
			&& active.historyEntries() == firstHistory
			&& editorPackage.hasUnsavedChanges(),
			"stale save changed the draft, history, revision, or dirty state");
		expectSaved(editorPackage, firstRevision, 4, "first package save");
		require(!editorPackage.hasUnsavedChanges(), "successful save did not update the clean baseline");

		applyTitle(editorPackage, "Editor save probe B");
		final rejectedDraft = active.canonicalDraft();
		final rejectedRevision = active.revision();
		final rejectedHistory = active.historyEntries();
		final validManifest = File.getBytes('$root/$MANIFEST_PATH');
		File.saveContent('$root/$MANIFEST_PATH', "{");
		switch editorPackage.save(rejectedRevision) {
			case EditorPackageSaveRejected(RefreshRejected(_)):
			case EditorPackageSaved(_, _, _):
				throw new haxe.Exception("malformed package accepted a save");
			case EditorPackageSaveRejected(error):
				throw new haxe.Exception('malformed package returned the wrong error: ${Std.string(error)}');
		}
		require(active.canonicalDraft().compare(rejectedDraft) == 0
			&& active.revision() == rejectedRevision
			&& active.historyEntries() == rejectedHistory
			&& editorPackage.hasUnsavedChanges(),
			"rejected package save changed the draft or clean baseline");
		File.saveBytes('$root/$MANIFEST_PATH', validManifest);
		expectSaved(editorPackage, rejectedRevision, 4, "second package save");
		final savedDraft = active.canonicalDraft();
		require(!editorPackage.hasUnsavedChanges(), "second successful save remained dirty");

		switch active.mutate({baseRevision: active.revision(), mutation: Undo}) {
			case MutationApplied(_, _, _, _, _, _):
			case MutationUnchanged(_, _) | MutationRejected(_, _):
				throw new haxe.Exception("undo after save did not restore the prior title");
		}
		require(editorPackage.hasUnsavedChanges(), "undo away from the saved bytes appeared clean");
		switch active.mutate({baseRevision: active.revision(), mutation: Redo}) {
			case MutationApplied(_, _, _, _, _, _):
			case MutationUnchanged(_, _) | MutationRejected(_, _):
				throw new haxe.Exception("redo after save did not restore the saved title");
		}
		require(!editorPackage.hasUnsavedChanges(), "redo to the saved bytes appeared dirty");

		final reopened = switch EditorPackageSession.open(root, MANIFEST_PATH, LEVEL_PATH) {
			case EditorPackageOpened(value): value;
			case EditorPackageOpenRejected(error): throw new haxe.Exception('saved package did not reopen: ${Std.string(error)}');
		};
		require(reopened.workspace().canonicalDraft().compare(savedDraft) == 0, "reopened package changed the saved canonical bytes");
		expectSaved(reopened, reopened.revision(), 0, "clean package save");
	}

	/** Replace the literal title through the same revisioned editor mutation. */
	static function applyTitle(editorPackage:EditorPackageSession, title:String):Void {
		switch editorPackage.mutate({baseRevision: editorPackage.revision(), mutation: Apply(SetTitle(ScenarioText.Literal(title)))}) {
			case MutationApplied(_, _, _, _, _, _):
			case MutationUnchanged(_, _) | MutationRejected(_, _):
				throw new haxe.Exception('title edit rejected: $title');
		}
	}

	/** Require one complete package publication with the expected file count. */
	static function expectSaved(editorPackage:EditorPackageSession, revision:Int, changedFiles:Int, label:String):Void {
		switch editorPackage.save(revision) {
			case EditorPackageSaved(actualRevision, actualFiles, warnings):
				require(actualRevision == revision && actualFiles == changedFiles && warnings.length == 0, '$label returned unexpected publication facts');
			case EditorPackageSaveRejected(error):
				throw new haxe.Exception('$label rejected: ${Std.string(error)}');
		}
	}

	/** Copy every file owned by the checked package into one isolated root. */
	static function copyPackage(root:String):Void {
		FileSystem.createDirectory(root);
		final manifestBytes = File.getBytes(MANIFEST_PATH);
		File.saveBytes('$root/$MANIFEST_PATH', manifestBytes);
		final manifest = switch decodeContentPackageManifest(manifestBytes) {
			case ContentPackageManifestReady(value): value;
			case ContentPackageManifestRejected(error): throw new haxe.Exception('checked package manifest rejected: ${Std.string(error)}');
		};
		for (index in 0...manifest.entryCount()) {
			final path = manifest.entryAt(index).logicalPath.text();
			createParentDirectories(root, path);
			File.saveBytes('$root/$path', File.getBytes(path));
		}
	}

	/** Create each package-relative parent without interpreting the file name. */
	static function createParentDirectories(root:String, path:String):Void {
		final parts = path.split("/");
		var current = root;
		for (index in 0...parts.length - 1) {
			current += "/" + parts[index];
			if (!FileSystem.exists(current))
				FileSystem.createDirectory(current);
		}
	}

	/** Select a collision-resistant disposable directory below the current root. */
	static function temporaryRoot():String
		return '.hxc-editor-save-${Std.int(Sys.time() * 1000)}-${Std.random(1000000)}';

	/** Remove only the isolated package copy created by this probe. */
	static function removeTree(path:String):Void {
		if (!FileSystem.exists(path))
			return;
		if (!FileSystem.isDirectory(path)) {
			FileSystem.deleteFile(path);
			return;
		}
		for (name in FileSystem.readDirectory(path))
			removeTree('$path/$name');
		FileSystem.deleteDirectory(path);
	}

	/** Stop at the first broken save invariant. */
	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw new haxe.Exception(message);
	}
}
