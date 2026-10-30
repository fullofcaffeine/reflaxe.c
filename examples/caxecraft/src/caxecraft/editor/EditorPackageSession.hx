package caxecraft.editor;

import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageRefresh.ContentRefreshError;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planLevelContentPackageRefresh;
import caxecraft.content.ContentRefreshPublisher.ContentRefreshPublication;
import caxecraft.content.ContentRefreshPublisher.publishContentRefresh;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.ContentPackageSource;
import caxecraft.content.RuntimeContentPack;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.content.RuntimeContentDigest.runtimeSha256Hex;
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
import haxe.io.Bytes;

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
	LevelReceiptRejected(path:String);
	LevelSessionMismatch(path:String);
	LevelRejected(path:String, detail:String);
	EditorRejected(error:EditorError);
	RevisionRejected(expected:Int, actual:Int);
	RefreshRejected(error:ContentRefreshError);
	PublicationRejected(detail:String);
}

/**
 * Describe the package stage that rejected an editor session.
 *
 * Native startup uses this closed mapping instead of asking target exceptions
 * or compiler-owned enum values for text. Paths and planner details remain
 * visible while low-level host errors stay behind their typed boundaries.
 */
function editorPackageErrorMessage(error:EditorPackageError):String
	return switch error {
		case PackageRootRejected(_): "package root rejected";
		case PackageRejected(detail): 'package rejected: $detail';
		case ContentPackMissing: "content pack missing";
		case ContentPackRejected(detail): 'content pack rejected: $detail';
		case LevelMissing(path): 'level missing: $path';
		case LevelSourceRejected(path, _): 'level source rejected: $path';
		case LevelReceiptRejected(path): 'level receipt rejected: $path';
		case LevelSessionMismatch(path): 'level session differs from package source: $path';
		case LevelRejected(path, detail): 'level rejected: $path: $detail';
		case EditorRejected(_): "editor rejected the level";
		case RevisionRejected(expected, actual): 'revision rejected: expected $expected, received $actual';
		case RefreshRejected(_): "content refresh rejected";
		case PublicationRejected(detail): 'content publication rejected: $detail';
	};

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
 * Reads one logical package across its writable content and read-only assets.
 *
 * Native distribution keeps creator-authored content below `content` while
 * Raylib reads reviewed assets beside the executable. This adapter preserves
 * one logical package view for receipt verification without giving the Save
 * publisher authority to replace assets.
 */
private final class EditorPackageSource implements ContentPackageSource {
	final content:ContentPackageStore;
	final assets:ContentPackageStore;

	/** Borrow two confined stores whose host roots were selected by the app. */
	public function new(content:ContentPackageStore, assets:ContentPackageStore) {
		this.content = content;
		this.assets = assets;
	}

	/** Route only the closed package asset prefix to the presentation root. */
	public function read(logicalPath:String):ContentPackageReadResult
		return StringTools.startsWith(logicalPath, "assets/") ? assets.read(logicalPath) : content.read(logicalPath);
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
	final store:ContentPackageSource;
	final session:EditorSession;
	var savedStateIdentity:Int;

	private function new(rootPath:String, manifestPath:String, levelPath:String, store:ContentPackageSource, session:EditorSession) {
		this.rootPath = rootPath;
		this.manifestPath = manifestPath;
		this.levelPath = levelPath;
		this.store = store;
		this.session = session;
		savedStateIdentity = session.stateIdentity();
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

	/**
	 * Attach package save authority to one already-open editor session.
	 *
	 * The native application already owns validated level bytes and a matching
	 * content registry from one runtime generation. This method reads only the
	 * outer manifest and selected level. It rejects a stale receipt or different
	 * level before it grants save authority, so the application does not decode
	 * the complete package or create a second editor session.
	 */
	public static function attach(rootPath:String, assetRootPath:String, manifestPath:String, levelPath:String, session:EditorSession):EditorPackageOpenResult {
		final contentStore = switch ContentPackageStore.open(rootPath, "editor-package", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreRejected(error): return EditorPackageOpenRejected(PackageRootRejected(error));
			case PackageStoreOpened(value): value;
		};
		final assetStore = switch ContentPackageStore.open(assetRootPath, "editor-assets", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreRejected(error): return EditorPackageOpenRejected(PackageRootRejected(error));
			case PackageStoreOpened(value): value;
		};
		final store = new EditorPackageSource(contentStore, assetStore);
		final manifestBytes = switch store.read(manifestPath) {
			case PackageBytesRejected(error): return EditorPackageOpenRejected(PackageRootRejected(error));
			case PackageBytesRead(value): value.bytes;
		};
		final manifest = switch decodeContentPackageManifest(manifestBytes) {
			case ContentPackageManifestRejected(_): return EditorPackageOpenRejected(PackageRejected("outer package manifest rejected"));
			case ContentPackageManifestReady(value): value;
		};
		var expectedLength = -1;
		var expectedHash:Null<String> = null;
		for (index in 0...manifest.entryCount()) {
			final entry = manifest.entryAt(index);
			if (entry.kind == Level && entry.logicalPath.text() == levelPath) {
				expectedLength = entry.byteLength;
				expectedHash = entry.sha256;
			}
		}
		if (expectedHash == null)
			return EditorPackageOpenRejected(LevelMissing(levelPath));
		final levelBytes = switch store.read(levelPath) {
			case PackageBytesRejected(error): return EditorPackageOpenRejected(LevelSourceRejected(levelPath, error));
			case PackageBytesRead(value): value.bytes;
		};
		if (levelBytes.length != expectedLength || runtimeSha256Hex(levelBytes) != expectedHash)
			return EditorPackageOpenRejected(LevelReceiptRejected(levelPath));
		if (!session.matchesInitialSource(levelBytes))
			return EditorPackageOpenRejected(LevelSessionMismatch(levelPath));
		return EditorPackageOpened(new EditorPackageSession(rootPath, manifestPath, levelPath, store, session));
	}

	/** Current revision for the next mutation or save request. */
	public inline function revision():Int
		return session.revision();

	/**
	 * Borrow the single editor session used by package save and visual edits.
	 *
	 * `EditorPackageSession` retains lifetime and save ownership. The visual
	 * screen uses this reference instead of opening the same level again.
	 */
	public inline function workspace():EditorSession
		return session;

	/** True when the current history state differs from the last successful save. */
	public function hasUnsavedChanges():Bool
		return session.stateIdentity() != savedStateIdentity;

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
		final canonical = session.canonicalDraft();
		final plan = switch planLevelContentPackageRefresh(store, manifestPath, levelPath, canonical) {
			case ContentRefreshRejected(error): return EditorPackageSaveRejected(RefreshRejected(error));
			case ContentRefreshReady(value): value;
		};
		return switch publishContentRefresh(rootPath, plan) {
			case RefreshPublishRejected(detail): EditorPackageSaveRejected(PublicationRejected(detail));
			case RefreshPublished(changed, warnings):
				savedStateIdentity = session.stateIdentity();
				EditorPackageSaved(session.revision(), changed, warnings);
		};
	}
}
