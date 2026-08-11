package caxecraft.editor;

import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageRefresh.ContentRefreshError;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planContentPackageRefresh;
import caxecraft.content.ContentRefreshPublisher.ContentRefreshPublication;
import caxecraft.content.ContentRefreshPublisher.publishContentRefresh;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.RuntimeContentPack;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorMutationRequest;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;

/**
 * Opens and saves one real package level through the shared editor model.
 *
 * `EditorSession` owns each draft change. This class adds the package root,
 * content registry, level path, and grouped save operation. A visual editor,
 * local command process, or later MCP adapter can use the same instance.
 */
enum EditorPackageError {
	PackageRootRejected(error:ContentPackageError);
	PackageRejected(detail:String);
	ContentPackMissing;
	ContentPackRejected(detail:String);
	LevelMissing(path:String);
	LevelSourceRejected(path:String, error:ContentPackageError);
	LevelRejected(path:String, detail:String);
	EditorRejected(error:EditorError);
	RevisionRejected(expected:Int, actual:Int);
	RefreshRejected(error:ContentRefreshError);
	PublicationRejected(detail:String);
}

/** A package level opened for revisioned editing, or one closed error. */
enum EditorPackageOpenResult {
	EditorPackageOpened(value:EditorPackageSession);
	EditorPackageOpenRejected(error:EditorPackageError);
}

/** Result of one package save request. */
enum EditorPackageSaveResult {
	EditorPackageSaved(revision:Int, changedFiles:Int, cleanupWarnings:Array<String>);
	EditorPackageSaveRejected(error:EditorPackageError);
}

/**
 * Owns one package-confined editor session and its save authority.
 *
 * This class has identity and mutable draft state for one open level. A module
 * function cannot represent that lifetime or prevent two levels from sharing
 * one undo history by mistake.
 */
final class EditorPackageSession {
	final rootPath:String;
	final manifestPath:String;
	final levelPath:String;
	final store:ContentPackageStore;
	final session:EditorSession;

	private function new(rootPath:String, manifestPath:String, levelPath:String, store:ContentPackageStore, session:EditorSession) {
		this.rootPath = rootPath;
		this.manifestPath = manifestPath;
		this.levelPath = levelPath;
		this.store = store;
		this.session = session;
	}

	/** Open one verified package level and create its isolated editor history. */
	public static function open(rootPath:String, manifestPath:String, levelPath:String):EditorPackageOpenResult {
		final store = switch ContentPackageStore.open(rootPath, "editor-package", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreRejected(error): return EditorPackageOpenRejected(PackageRootRejected(error));
			case PackageStoreOpened(value): value;
		};
		final packageValue = switch loadContentPackage(store, manifestPath) {
			case ContentPackageRejected(error): return EditorPackageOpenRejected(PackageRejected(Std.string(error)));
			case ContentPackageReady(value): value;
		};
		var contentPath:Null<String> = null;
		var levelOwned = false;
		for (index in 0...packageValue.manifest.entryCount()) {
			final entry = packageValue.manifest.entryAt(index);
			if (entry.kind == ContentPack)
				contentPath = entry.logicalPath.text();
			if (entry.kind == Level && entry.logicalPath.text() == levelPath)
				levelOwned = true;
		}
		if (contentPath == null)
			return EditorPackageOpenRejected(ContentPackMissing);
		if (!levelOwned)
			return EditorPackageOpenRejected(LevelMissing(levelPath));
		final contentBytes = switch store.read(contentPath) {
			case PackageBytesRejected(error): return EditorPackageOpenRejected(PackageRootRejected(error));
			case PackageBytesRead(value): value.bytes;
		};
		final registry = switch RuntimeContentPack.decode(contentBytes) {
			case RuntimeContentPackRejected(diagnostic): return EditorPackageOpenRejected(ContentPackRejected(Std.string(diagnostic)));
			case RuntimeContentPackReady(value): value;
		};
		final levelBytes = switch store.read(levelPath) {
			case PackageBytesRejected(error): return EditorPackageOpenRejected(LevelSourceRejected(levelPath, error));
			case PackageBytesRead(value): value.bytes;
		};
		final scenario = switch ScenarioLexer.read(levelBytes) {
			case ReadError(diagnostics): return EditorPackageOpenRejected(LevelRejected(levelPath, Std.string(diagnostics[0])));
			case ReadOk(records):
				switch ScenarioParser.parse(records) {
					case ReadError(diagnostics): return EditorPackageOpenRejected(LevelRejected(levelPath, Std.string(diagnostics[0])));
					case ReadOk(parsed):
						switch ScenarioValidator.validate(parsed, registry) {
							case ReadError(diagnostics): return EditorPackageOpenRejected(LevelRejected(levelPath, Std.string(diagnostics[0])));
							case ReadOk(value): value;
						}
				}
		};
		return switch EditorSession.open(scenario, registry) {
			case EditorOpenRejected(error): EditorPackageOpenRejected(EditorRejected(error));
			case EditorOpened(value): EditorPackageOpened(new EditorPackageSession(rootPath, manifestPath, levelPath, store, value));
		};
	}

	/** Current revision for the next mutation or save request. */
	public inline function revision():Int
		return session.revision();

	/** Ask one copy-owned question about the current draft. */
	public inline function query(request:EditorQuery):EditorObservation
		return session.query(request);

	/** Apply one revision-checked edit through the shared history boundary. */
	public inline function mutate(request:EditorMutationRequest):EditorMutationResult
		return session.mutate(request);

	/**
	 * Validate and save the current draft with its package receipts.
	 *
	 * The request fails if its revision is stale. The refresh planner validates
	 * all final bytes before the publisher replaces any file.
	 */
	public function save(baseRevision:Int):EditorPackageSaveResult {
		if (baseRevision != session.revision())
			return EditorPackageSaveRejected(RevisionRejected(session.revision(), baseRevision));
		final plan = switch planContentPackageRefresh(store, manifestPath, levelPath, session.canonicalDraft()) {
			case ContentRefreshRejected(error): return EditorPackageSaveRejected(RefreshRejected(error));
			case ContentRefreshReady(value): value;
		};
		return switch publishContentRefresh(rootPath, plan) {
			case RefreshPublishRejected(detail): EditorPackageSaveRejected(PublicationRejected(detail));
			case RefreshPublished(changed, warnings): EditorPackageSaved(session.revision(), changed, warnings);
		};
	}
}
