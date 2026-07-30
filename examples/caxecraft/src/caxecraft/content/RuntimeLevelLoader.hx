package caxecraft.content;

import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanError;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
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

	private function new(generation:LoadedContentGeneration, receipt:RuntimeLevelReceipt, authoredTrace:RuntimeLevelAuthoredTrace) {
		loadedGeneration = generation;
		sourceReceipt = receipt;
		authored = authoredTrace;
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
 * The currently compiled `BaseContentRegistry` supplies both validation and
 * engine-resolution facts in this first vertical. Runtime pack loading will
 * replace that concrete registry under `haxe_c-xge.20.4.3.7` without changing
 * the byte, plan, generation, or publication pipeline.
 */
function loadRuntimeLevel(source:RuntimeLevelSource, generationId:ContentGenerationId, registry:BaseContentRegistry,
		playerOptions:LevelPlayerOptions):RuntimeLevelLoadResult {
	return loadRuntimeLevelInternal(source, generationId, registry, playerOptions #if caxecraft_runtime_level_testing, NoGenerationFault #end);
}

#if caxecraft_runtime_level_testing
/**
 * Test-only entry that stops candidate construction at one named stage.
 *
 * This function is absent from ordinary game builds. It proves that a package
 * read and successful parse still cannot expose a partial generation when a
 * later construction stage rejects.
 */
function loadRuntimeLevelWithFault(source:RuntimeLevelSource, generationId:ContentGenerationId, registry:BaseContentRegistry,
		playerOptions:LevelPlayerOptions, fault:ContentGenerationBuildFault):RuntimeLevelLoadResult {
	return loadRuntimeLevelInternal(source, generationId, registry, playerOptions, fault);
}
#end

/**
 * Share every semantic stage after source admission between both authorities.
 */
private function loadRuntimeLevelInternal(source:RuntimeLevelSource, generationId:ContentGenerationId, registry:BaseContentRegistry,
		playerOptions:LevelPlayerOptions #if caxecraft_runtime_level_testing, fault:ContentGenerationBuildFault #end):RuntimeLevelLoadResult {
	final input = switch admitRuntimeLevelInput(source) {
		case RuntimeLevelInputReady(value): value;
		case RuntimeLevelInputRejected(error): return RuntimeLevelRejected(RuntimeLevelSourceRejected(error));
	};
	final scenario = switch ScenarioLexer.read(input.bytes) {
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, registry) {
						case ReadOk(validated): validated;
						case ReadError(diagnostics): return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
					}
				case ReadError(diagnostics):
					return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
			}
		case ReadError(diagnostics):
			return RuntimeLevelRejected(RuntimeLevelScenarioRejected(diagnostics));
	};
	final resolved = switch ResolvedLevelPlan.resolve(scenario, registry, playerOptions) {
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
			RuntimeLevelReady(new RuntimeLevelCandidate(generation, {
				authority: input.authority,
				rootLabel: input.rootLabel,
				logicalPath: input.logicalPath,
				byteLength: input.bytes.length,
				inputHash: hashBytes(input.bytes),
				readAttempts: input.readAttempts
			}, authoredTrace(scenario)));
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
