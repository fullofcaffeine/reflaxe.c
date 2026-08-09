package caxecraft.content;

import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanError;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.EntityId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.ScenarioMessages;
import caxecraft.scenario.ScenarioMessages.resolveScenarioMessage;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioDialogueLine;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioValidator;
import haxe.io.Bytes;
#if caxecraft_runtime_level_testing
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildFault;
#end

/**
 * Loads one complete runtime level from owned CAXEMAP bytes.
 *
 * A package file and an already-owned in-memory byte vector differ only in how
 * bytes enter this module. Both then run the same lexer, parser, validator,
 * resolver, generation builder, and later `ActiveContent` publication. The
 * game selects a package file; the in-memory form keeps focused tests and
 * filesystem-free target adapters on the same semantic path. No live session
 * is accepted here, so every rejection leaves current gameplay unchanged.
 */
/**
 * The two byte sources admitted by the shared runtime loader.
 *
 * `InMemoryBytes` carries source bytes rather than pre-resolved terrain or
 * actor facts. This keeps embedded QA and targets without ambient filesystems
 * on the same parser and resolver as hosted package loading.
 */
enum RuntimeLevelSource {
	/**
	 * Already-owned bytes with an explicit diagnostic origin.
	 *
	 * The caller retains the input `Bytes`, but this synchronous load neither
	 * stores nor exposes them after parsing.
	 */
	InMemoryBytes(bytes:Bytes, rootLabel:String, logicalPath:String);

	/** Read exact owned bytes below an already-open package-root capability. */
	NativePackageFile(store:ContentPackageStore, logicalPath:String);

	/** Reuse one exact package read after an outer coordinator verified it. */
	AdmittedPackageBytes(content:LoadedPackageBytes);
}

/** Which byte source produced one successful candidate. */
enum RuntimeLevelAuthority {
	/** Bytes were already owned by the caller, such as an embedded QA fixture. */
	InMemorySource;

	/** Bytes were read from a staged package after process startup. */
	NativePackage;
}

/**
 * Source receipt reported beside one complete unpublished generation.
 *
 * `inputHash` is a deterministic 32-bit diagnostic fingerprint, not a
 * cryptographic security or cache identity. The package store's confinement,
 * exact-read, and changed-file checks own read safety. A later content-addressed
 * cache must use a separately proved cryptographic digest.
 */
typedef RuntimeLevelReceipt = {
	/** Explicit byte source selected for this load request. */
	final authority:RuntimeLevelAuthority;

	/** Application-chosen safe root name, never an unrestricted host path. */
	final rootLabel:String;

	/** Validated package-relative source spelling. */
	final logicalPath:String;

	/** Exact source byte count consumed by the shared lexer. */
	final byteLength:Int;

	/** Stable target-neutral fingerprint used for logs and parity tests. */
	final inputHash:Int;

	/** One for stable memory/package input, or two after a safe read retry. */
	final readAttempts:Int;
}

/**
 * Target-neutral evidence for story and CaxeFlow facts retained from the map.
 *
 * Construction plans intentionally do not duplicate dialogue or objective
 * bodies. These counts and ordered identity digests prove that the shared
 * validated source—not a hardcoded game branch—supplied those authored facts.
 */
typedef RuntimeLevelAuthoredTrace = {
	/** Number of validated dialogue declarations. */
	final dialogues:Int;

	/** Ordered digest of validated dialogue identities. */
	final dialogueDigest:Int;

	/** Number of validated objective declarations. */
	final objectives:Int;

	/** Ordered digest of validated objective identities. */
	final objectiveDigest:Int;

	/** Number of validated CaxeFlow variables. */
	final flowVariables:Int;

	/** Number of validated CaxeFlow sequences. */
	final flowSequences:Int;

	/** Number of validated CaxeFlow rules. */
	final flowRules:Int;

	/** Ordered digest of all validated CaxeFlow declaration identities. */
	final flowDigest:Int;
}

/**
 * Keeps player-visible story text from one validated map.
 *
 * Runtime loading used to retain only counts and identity digests for story
 * data. That was enough to prove parsing, but the game then had to draw text
 * from the entry map's compile-time catalog after travelling elsewhere. This
 * value instead keeps the validated message catalog, dialogue, journal, and
 * objective references beside the unpublished generation.
 * `ActivePlayableLevel` can therefore replace the world and its matching words
 * in one publication.
 *
 * The loader owns the parsed scenario exclusively and never exposes its
 * mutable Arrays, so retaining that private catalog does not share mutation
 * authority with the editor or caller. A later live objective-state owner can
 * supersede the explicitly bounded “initial active objective” selection.
 */
@:allow(caxecraft.content.RuntimeLevelLoader)
final class RuntimeLevelPresentation {
	final messages:ScenarioMessages;
	final title:ScenarioText;
	final dialogues:Array<ScenarioDialogue>;
	final journal:Array<ScenarioJournalEntry>;
	final objectives:Array<ScenarioObjective>;
	final startingObjective:Null<ScenarioId>;

	/** Retain presentation facts only after the complete scenario validates. */
	private function new(scenario:Scenario) {
		messages = scenario.messages;
		title = scenario.title;
		dialogues = [];
		for (dialogue in scenario.story.dialogues) {
			final lines:Array<ScenarioDialogueLine> = [];
			for (line in dialogue.lines)
				lines.push({speaker: line.speaker, text: line.text});
			dialogues.push({id: dialogue.id, lines: lines});
		}
		journal = [];
		for (entry in scenario.story.journal)
			journal.push({id: entry.id, title: entry.title, body: entry.body});
		objectives = [];
		var selected:Null<ScenarioId> = null;
		for (objective in scenario.story.objectives) {
			objectives.push({
				id: objective.id,
				title: objective.title,
				body: objective.body,
				initialState: objective.initialState
			});
			if (selected == null && objective.initialState == ObjectiveState.Active)
				selected = objective.id;
		}
		startingObjective = selected;
	}

	/** Resolve the map title in the requested locale, using its declared fallback. */
	public inline function scenarioTitle(locale:LocaleId):String
		return resolve(title, locale);

	/**
	 * Resolve one stable message ID from this map's validated runtime catalog.
	 *
	 * The requested locale falls back through the map's declared default. An
	 * unknown ID returns empty text, so no partial catalog or nullable value
	 * escapes into the presentation layer.
	 */
	public function message(id:MessageId, locale:LocaleId):String {
		final translated = resolveScenarioMessage(messages, locale, id);
		return translated == null ? "" : translated;
	}

	/** Resolve the authored Adventure summary used by the title and campaign menus. */
	public inline function adventureTagline(locale:LocaleId):String
		return message(new MessageId("adventure_tagline"), locale);

	/** Resolve the first initially active objective title, or empty text when absent. */
	public inline function initialObjectiveTitle(locale:LocaleId):String
		return objectiveTitle(startingObjective, locale);

	/** Resolve the matching initial objective body, or empty text when absent. */
	public inline function initialObjectiveBody(locale:LocaleId):String
		return objectiveBody(startingObjective, locale);

	/** Return the first objective that starts active, or null when none does. */
	public inline function initialObjectiveId():Null<ScenarioId>
		return startingObjective;

	/**
	 * Resolve one authored dialogue line, or empty text when it is unavailable.
	 *
	 * The active level owns the validated dialogue records and locale catalog as
	 * one immutable presentation value. The HUD can therefore respond to a
	 * `DialogueRequested` event without knowing which NPC, campaign, or source
	 * file supplied the text. This first playable slice presents one line; a
	 * later conversation UI can use the same stable ID with an explicit cursor.
	 */
	public function dialogueLine(id:ScenarioId, lineIndex:Int, locale:LocaleId):String {
		if (lineIndex < 0)
			return "";
		for (dialogue in dialogues)
			if (dialogue.id.text() == id.text())
				return lineIndex < dialogue.lines.length ? resolve(dialogue.lines[lineIndex].text, locale) : "";
		return "";
	}

	/** Resolve one unlocked journal title by stable authored identity. */
	public function journalTitle(id:ScenarioId, locale:LocaleId):String {
		for (entry in journal)
			if (entry.id.text() == id.text())
				return resolve(entry.title, locale);
		return "";
	}

	/** Resolve one unlocked journal body by stable authored identity. */
	public function journalBody(id:ScenarioId, locale:LocaleId):String {
		for (entry in journal)
			if (entry.id.text() == id.text())
				return resolve(entry.body, locale);
		return "";
	}

	/** Resolve one validated objective title by stable authored identity. */
	public function objectiveTitle(id:Null<ScenarioId>, locale:LocaleId):String {
		if (id == null)
			return "";
		for (objective in objectives)
			if (objective.id.text() == id.text())
				return resolve(objective.title, locale);
		return "";
	}

	/** Resolve one validated objective body by stable authored identity. */
	public function objectiveBody(id:Null<ScenarioId>, locale:LocaleId):String {
		if (id == null)
			return "";
		for (objective in objectives)
			if (objective.id.text() == id.text())
				return resolve(objective.body, locale);
		return "";
	}

	/** Turn a literal or validated message reference into runtime-owned text. */
	function resolve(value:ScenarioText, locale:LocaleId):String {
		return switch value {
			case Literal(text): text;
			case Message(id):
				final translated = resolveScenarioMessage(messages, locale, id);
				translated == null ? "" : translated;
		};
	}
}

/**
 * Owns one complete unpublished generation and its source evidence.
 *
 * A class is appropriate because the generation owns mutable session identity
 * for one lifetime. The constructor is private, so callers cannot pair an
 * unrelated receipt or authored trace with a plausible generated session.
 */
@:allow(caxecraft.content.RuntimeLevelLoader)
final class RuntimeLevelCandidate {
	final loadedGeneration:LoadedContentGeneration;
	final sourceReceipt:RuntimeLevelReceipt;
	final authored:RuntimeLevelAuthoredTrace;
	final presentationValue:RuntimeLevelPresentation;

	private function new(generation:LoadedContentGeneration, receipt:RuntimeLevelReceipt, authoredTrace:RuntimeLevelAuthoredTrace,
			presentation:RuntimeLevelPresentation) {
		loadedGeneration = generation;
		sourceReceipt = receipt;
		authored = authoredTrace;
		presentationValue = presentation;
	}

	/** Complete generation that may be passed to `ActiveContent.publish`. */
	public inline function generation():LoadedContentGeneration
		return loadedGeneration;

	/** Copy the immutable scalar/string receipt for diagnostics and logging. */
	public function receipt():RuntimeLevelReceipt
		return {
			authority: sourceReceipt.authority,
			rootLabel: sourceReceipt.rootLabel,
			logicalPath: sourceReceipt.logicalPath,
			byteLength: sourceReceipt.byteLength,
			inputHash: sourceReceipt.inputHash,
			readAttempts: sourceReceipt.readAttempts
		};

	/** Copy the target-neutral story and flow evidence retained from validation. */
	public function authoredTrace():RuntimeLevelAuthoredTrace
		return {
			dialogues: authored.dialogues,
			dialogueDigest: authored.dialogueDigest,
			objectives: authored.objectives,
			objectiveDigest: authored.objectiveDigest,
			flowVariables: authored.flowVariables,
			flowSequences: authored.flowSequences,
			flowRules: authored.flowRules,
			flowDigest: authored.flowDigest
		};

	/** Player-visible facts validated from the same map as `generation()`. */
	public inline function presentation():RuntimeLevelPresentation
		return presentationValue;
}

/** Exact stage that rejected a runtime-level request before publication. */
enum RuntimeLevelLoadError {
	/** In-memory metadata or package byte acquisition was rejected. */
	RuntimeLevelSourceRejected(error:ContentPackageError);

	/** Lexing, parsing, or semantic validation rejected the CAXEMAP bytes. */
	RuntimeLevelScenarioRejected(diagnostics:Array<ScenarioDiagnostic>);

	/** Engine/content resolution could not produce a complete private plan. */
	RuntimeLevelPlanRejected(error:ResolvedLevelPlanError);

	/** Fresh candidate construction failed without exposing partial state. */
	RuntimeLevelGenerationRejected(error:ContentGenerationBuildError);
}

/** One complete unpublished level candidate, or a closed fail-closed error. */
enum RuntimeLevelLoadResult {
	/** The candidate and its source evidence are complete and safe to inspect. */
	RuntimeLevelReady(candidate:RuntimeLevelCandidate);

	/** No generation escaped and no active owner was available to mutate. */
	RuntimeLevelRejected(error:RuntimeLevelLoadError);
}

private typedef RuntimeLevelInput = {
	final authority:RuntimeLevelAuthority;
	final rootLabel:String;
	final logicalPath:String;
	final bytes:Bytes;
	final readAttempts:Int;
}

private enum RuntimeLevelInputResult {
	RuntimeLevelInputReady(input:RuntimeLevelInput);
	RuntimeLevelInputRejected(error:ContentPackageError);
}

/**
 * Read, validate, resolve, and construct one complete unpublished level.
 *
 * A `ScenarioContentRegistry` validates authored names, while a
 * `LevelContentResolver` selects construction facts. The atomic package owner
 * passes one runtime registry through both narrow views so no generated
 * registry can remain hidden in either stage.
 */
function loadRuntimeLevel(source:RuntimeLevelSource, generationId:ContentGenerationId, validationRegistry:ScenarioContentRegistry,
		resolutionRegistry:LevelContentResolver, playerOptions:LevelPlayerOptions):RuntimeLevelLoadResult {
	return loadRuntimeLevelInternal(source, generationId, validationRegistry, resolutionRegistry, playerOptions
		#if caxecraft_runtime_level_testing, NoGenerationFault #end);
}

#if caxecraft_runtime_level_testing
/**
 * Test-only entry that stops candidate construction at one named stage.
 *
 * This function is absent from ordinary game builds. It proves that a package
 * read and successful parse still cannot expose a partial generation when a
 * later construction stage rejects.
 */
function loadRuntimeLevelWithFault(source:RuntimeLevelSource, generationId:ContentGenerationId, validationRegistry:ScenarioContentRegistry,
		resolutionRegistry:LevelContentResolver, playerOptions:LevelPlayerOptions, fault:ContentGenerationBuildFault):RuntimeLevelLoadResult {
	return loadRuntimeLevelInternal(source, generationId, validationRegistry, resolutionRegistry, playerOptions, fault);
}

/**
 * Rebuild a fresh level/session from one real decoded candidate for publication tests.
 *
 * Native complete-package tests use this after one real filesystem load. It
 * avoids rereading and rehashing identical bytes while still constructing a
 * distinct `GameSession`, generation identity, level owner, and copied receipt.
 * The helper is absent from ordinary product builds and cannot combine an
 * unrelated plan, presentation model, registry, or source receipt.
 */
function rebuildRuntimeLevelForPublicationTesting(candidate:RuntimeLevelCandidate, generationId:ContentGenerationId):RuntimeLevelLoadResult {
	return switch LoadedContentGeneration.build(generationId, candidate.generation().plan(), candidate.generation().presentation()) {
		case ContentGenerationReady(generation):
			RuntimeLevelReady(new RuntimeLevelCandidate(generation, candidate.receipt(), candidate.authoredTrace(), candidate.presentation()));
		case ContentGenerationRejected(error):
			RuntimeLevelRejected(RuntimeLevelGenerationRejected(error));
	};
}
#end

/**
 * Share every semantic stage after source admission between both authorities.
 */
private function loadRuntimeLevelInternal(source:RuntimeLevelSource, generationId:ContentGenerationId, validationRegistry:ScenarioContentRegistry,
		resolutionRegistry:LevelContentResolver, playerOptions:LevelPlayerOptions
		#if caxecraft_runtime_level_testing, fault:ContentGenerationBuildFault #end):RuntimeLevelLoadResult {
	final input = switch admitRuntimeLevelInput(source) {
		case RuntimeLevelInputReady(value): value;
		case RuntimeLevelInputRejected(error): return RuntimeLevelRejected(RuntimeLevelSourceRejected(error));
	};
	final scenario = switch ScenarioLexer.read(input.bytes) {
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, validationRegistry) {
						case ReadOk(validated): validated;
						case ReadError(diagnostics): return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
					}
				case ReadError(diagnostics):
					return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
			}
		case ReadError(diagnostics):
			return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
	};
	final resolved = switch ResolvedLevelPlan.resolve(scenario, resolutionRegistry, playerOptions) {
		case LevelPlanResolved(plan, presentation): {plan: plan, presentation: presentation};
		case LevelPlanRejected(error): return RuntimeLevelRejected(RuntimeLevelPlanRejected(error));
	};
	final built =
		#if caxecraft_runtime_level_testing
		fault == NoGenerationFault ? LoadedContentGeneration.build(generationId, resolved.plan,
			resolved.presentation) : LoadedContentGeneration.buildWithFault(generationId, resolved.plan, resolved.presentation, fault);
		#else
		LoadedContentGeneration.build(generationId, resolved.plan, resolved.presentation);
		#end
	return switch built {
		case ContentGenerationReady(generation):
			final actorEntities:Array<EntityId> = [];
			final actorIds:Array<ScenarioId> = [];
			final itemContentIds:Array<ContentId> = [];
			final statefulObjectIds:Array<ScenarioId> = [];
			final statefulObjectPositionsMilli:Array<Int> = [];
			final statefulObjectRadiiMilli:Array<Int> = [];
			final statefulObjectBoundsMilli:Array<Int> = [];
			final statefulObjectStateStarts:Array<Int> = [];
			final statefulObjectStateCounts:Array<Int> = [];
			final statefulObjectCollisionStates:Array<ContentId> = [];
			final statefulObjectCollisionSolid:Array<Int> = [];
			for (binding in generation.actorBindings()) {
				actorEntities.push(binding.entityId);
				actorIds.push(binding.authoredId);
			}
			for (binding in generation.itemBindings())
				itemContentIds.push(binding.contentId);
			for (binding in generation.statefulObjectBindings()) {
				statefulObjectIds.push(binding.authoredId);
				statefulObjectPositionsMilli.push(binding.transform.xMilli);
				statefulObjectPositionsMilli.push(binding.transform.yMilli);
				statefulObjectPositionsMilli.push(binding.transform.zMilli);
				statefulObjectRadiiMilli.push(binding.interactionRadiusMilli);
				statefulObjectBoundsMilli.push(binding.bounds.widthMilli);
				statefulObjectBoundsMilli.push(binding.bounds.heightMilli);
				statefulObjectBoundsMilli.push(binding.bounds.depthMilli);
				statefulObjectBoundsMilli.push(binding.transform.yawDegrees);
				statefulObjectStateStarts.push(statefulObjectCollisionStates.length);
				statefulObjectStateCounts.push(binding.states.length);
				for (state in binding.states) {
					statefulObjectCollisionStates.push(state.state);
					statefulObjectCollisionSolid.push(switch state.collision {
						case StatefulObjectPassable: 0;
						case StatefulObjectSolid: 1;
					});
				}
			}
			generation.session()
				.installValidatedScenarioFlow(scenario, actorEntities, actorIds, itemContentIds, statefulObjectIds, statefulObjectPositionsMilli,
					statefulObjectRadiiMilli, statefulObjectBoundsMilli, statefulObjectStateStarts, statefulObjectStateCounts, statefulObjectCollisionStates,
					statefulObjectCollisionSolid);
			RuntimeLevelReady(new RuntimeLevelCandidate(generation, {
				authority: input.authority,
				rootLabel: input.rootLabel,
				logicalPath: input.logicalPath,
				byteLength: input.bytes.length,
				inputHash: hashBytes(input.bytes),
				readAttempts: input.readAttempts
			}, authoredTrace(scenario), new RuntimeLevelPresentation(scenario)));
		case ContentGenerationRejected(error):
			RuntimeLevelRejected(RuntimeLevelGenerationRejected(error));
	};
}

/** Acquire one immutable byte vector and normalized receipt from either source. */
private function admitRuntimeLevelInput(source:RuntimeLevelSource):RuntimeLevelInputResult {
	return switch source {
		case InMemoryBytes(bytes, rootLabel, logicalPath):
			if (rootLabel.length == 0 || rootLabel.indexOf("\x00") >= 0) RuntimeLevelInputRejected(RootUnavailable); else
				switch ContentPackagePath.parse(logicalPath) {
				case PathRejected(error):
					RuntimeLevelInputRejected(InvalidLogicalPath(error));
				case PathAccepted(path):
					if (bytes.length > ContentPackageStore.MAXIMUM_PACKAGE_BYTES)
							RuntimeLevelInputRejected(EntryTooLarge(ContentPackageStore.MAXIMUM_PACKAGE_BYTES)); else RuntimeLevelInputReady({
						authority: InMemorySource,
						rootLabel: rootLabel,
						logicalPath: path.text(),
						bytes: bytes,
						readAttempts: 1
					});
			}
		case NativePackageFile(store, logicalPath):
			switch store.read(logicalPath) {
				case PackageBytesRejected(error):
					RuntimeLevelInputRejected(error);
				case PackageBytesRead(content):
					RuntimeLevelInputReady({
						authority: NativePackage,
						rootLabel: content.provenance.rootLabel,
						logicalPath: content.provenance.logicalPath.text(),
						bytes: content.bytes,
						readAttempts: content.provenance.readAttempts
					});
			}
		case AdmittedPackageBytes(content):
			RuntimeLevelInputReady({
				authority: NativePackage,
				rootLabel: content.provenance.rootLabel,
				logicalPath: content.provenance.logicalPath.text(),
				bytes: content.bytes,
				readAttempts: content.provenance.readAttempts
			});
	};
}

/** Build deterministic story/flow evidence without retaining mutable source Arrays. */
private function authoredTrace(scenario:Scenario):RuntimeLevelAuthoredTrace {
	var dialogueDigest = hashStart();
	for (dialogue in scenario.story.dialogues)
		dialogueDigest = hashText(dialogueDigest, dialogue.id.text());
	var objectiveDigest = hashStart();
	for (objective in scenario.story.objectives)
		objectiveDigest = hashText(objectiveDigest, objective.id.text());
	var flowDigest = hashStart();
	for (variable in scenario.flow.variables)
		flowDigest = hashText(flowDigest, variable.id.text());
	for (sequence in scenario.flow.sequences)
		flowDigest = hashText(flowDigest, sequence.id.text());
	for (rule in scenario.flow.rules)
		flowDigest = hashText(flowDigest, rule.id.text());
	return {
		dialogues: scenario.story.dialogues.length,
		dialogueDigest: dialogueDigest,
		objectives: scenario.story.objectives.length,
		objectiveDigest: objectiveDigest,
		flowVariables: scenario.flow.variables.length,
		flowSequences: scenario.flow.sequences.length,
		flowRules: scenario.flow.rules.length,
		flowDigest: flowDigest
	};
}

/** Fold exact source bytes into a cheap log/parity fingerprint. */
private function hashBytes(bytes:Bytes):Int {
	var hash = hashStart();
	for (index in 0...bytes.length)
		hash = CaxecraftTrace.mix(hash, bytes.get(index));
	return hash;
}

/** Fold one UTF-8 identity into a deterministic authored-fact digest. */
private function hashText(hash:Int, value:String):Int {
	final bytes = Bytes.ofString(value);
	for (index in 0...bytes.length)
		hash = CaxecraftTrace.mix(hash, bytes.get(index));
	return hash;
}

/** Shared nonzero start value for deterministic trace fingerprints. */
private inline function hashStart():Int
	return 0x811C9DC5;
