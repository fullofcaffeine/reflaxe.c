package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeReachabilityEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementAnalysis;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementCandidate;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;

/**
 * Names one runtime operation found while walking validated HxcIR.
 *
 * The stable identity is computed once because large programs compare the same
 * feature, operation, and source location while sorting, deduplicating, and
 * reconciling compiler evidence. Rebuilding that source string inside each
 * comparison would turn bookkeeping into a measurable part of compilation.
 */
private class RuntimeIntentObservation {
	public final featureId:String;
	public final operationId:String;
	public final source:HxcSourceSpan;
	public final identity:String;

	public function new(featureId:String, operationId:String, source:HxcSourceSpan) {
		this.featureId = featureId;
		this.operationId = operationId;
		this.source = source;
		this.identity = '$featureId\x00$operationId\x00${source.display()}';
	}

	public inline function key():String
		return identity;
}

/**
 * Pairs one typed source requirement with its two reconciliation keys.
 *
 * This is request-local derived data, not a cache shared between compilations.
 * `observationIdentity` matches the HxcIR operation. `reasonIdentity` also
 * includes the descriptive kind, surface, and alternative, so exact duplicate
 * reasons collapse while distinct reasons at one source point remain visible.
 * Keeping both strings beside the candidate avoids rebuilding source spans in
 * hot comparators.
 */
private class RuntimeCandidateEntry {
	public final candidate:RuntimeRequirementCandidate;
	public final observationIdentity:String;
	public final reasonIdentity:String;

	public function new(candidate:RuntimeRequirementCandidate) {
		this.candidate = candidate;
		this.observationIdentity = '${candidate.featureId.text()}\x00${candidate.operationId}\x00${candidate.source.display()}';
		this.reasonIdentity = '$observationIdentity\x00${candidate.kind}\x00${candidate.surface}\x00${candidate.alternative == null ? "" : candidate.alternative}';
	}
}

/**
 * Proves that source-level runtime reasons describe work that survived in HxcIR.
 *
 * A source span identifies a location in a Haxe file; it does not uniquely
 * identify one semantic operation. For example, constructing one record at one
 * expression can retain a local String and separately retain that String for
 * the record field. Both HxcIR instructions legitimately carry the same span.
 *
 * Reconciliation therefore counts operations by feature, operation, and span.
 * Exact duplicate source reasons collapse, while distinct descriptions remain
 * legal only when the validated HxcIR contains at least that many matching
 * operations. Missing reasons, orphan reasons, and excess distinct reasons are
 * internal compiler errors, so runtime selection can never be guessed from
 * source text alone.
 */
class RuntimeRequirementAnalyzer {
	public static inline final ANALYSIS_SCHEMA_VERSION = 1;

	public function new() {}

	public function analyze(program:HxcIRProgram, input:Array<RuntimeRequirementCandidate>):RuntimeRequirementAnalysis {
		if (program.schemaVersion != HxcIRValidator.SCHEMA_VERSION) {
			internal('runtime requirement analysis needs validated schema-${HxcIRValidator.SCHEMA_VERSION} HxcIR; found `${program.schemaVersion}`');
		}
		final observations:Array<RuntimeIntentObservation> = [];
		var typeInstanceCount = 0;
		var functionCount = 0;
		var blockCount = 0;
		var instructionCount = 0;
		var cleanupActionCount = 0;
		for (module in program.modules) {
			for (declaration in module.types) {
				switch declaration.kind {
					case IRTKClass({header: IRCHRuntime(featureId)}):
						observations.push(new RuntimeIntentObservation(featureId, "class-object-header", declaration.source));
					case _:
				}
			}
			typeInstanceCount += module.typeInstances.length;
			for (instance in module.typeInstances) {
				switch instance.representation {
					case IRRManaged(featureId):
						observations.push(new RuntimeIntentObservation(featureId, "managed-type-representation", instance.source));
					case _:
				}
			}
			functionCount += module.functions.length;
			for (fn in module.functions) {
				if (fn.managedRoots != null)
					for (root in fn.managedRoots)
						observations.push(new RuntimeIntentObservation("gc", "root-frame", root.source));
				blockCount += fn.blocks.length;
				for (block in fn.blocks) {
					instructionCount += block.instructions.length;
					for (instruction in block.instructions) {
						collectInstruction(instruction, observations);
					}
				}
				for (region in fn.cleanupRegions) {
					cleanupActionCount += region.actions.length;
					for (action in region.actions) {
						collectCleanup(action, observations);
					}
				}
			}
		}

		final candidates = canonicalCandidates(input);
		final uniqueObservations = canonicalObservations(observations);
		final observationCounts = countObservations(observations);
		final candidatesByObservation:Map<String, Array<RuntimeRequirementCandidate>> = [];
		for (entry in candidates) {
			var matching = candidatesByObservation.get(entry.observationIdentity);
			if (matching == null) {
				matching = [];
				candidatesByObservation.set(entry.observationIdentity, matching);
			}
			matching.push(entry.candidate);
		}
		final observedIdentities:Map<String, Bool> = [];
		final reasons:Array<RuntimeRequirementReason> = [];
		for (observation in uniqueObservations) {
			final matching = candidatesFor(candidatesByObservation, observation);
			final observationCount = requiredCount(observationCounts, observation.identity);
			if (matching.length > observationCount) {
				internal('${matching.length} distinct runtime source reasons describe only $observationCount reachable `${observation.operationId}` operation(s) at `${observation.source.display()}`',
					[observation.featureId]);
			}
			observedIdentities.set(observation.identity, true);
			for (candidate in matching) {
				final index = reasons.length;
				reasons.push(new RuntimeRequirementReason('runtime.${observation.featureId}.${observation.operationId}.$index',
					RuntimeFeatureId.parse(observation.featureId), observation.operationId, candidate.kind, candidate.surface, observation.source,
					candidate.alternative));
			}
		}
		for (entry in candidates) {
			final candidate = entry.candidate;
			if (!observedIdentities.exists(entry.observationIdentity)) {
				internal('runtime source reason for `${candidate.operationId}` at `${candidate.source.display()}` has no reachable HxcIR runtime intent',
					[candidate.featureId.text()]);
			}
		}
		return new RuntimeRequirementAnalysis(reasons,
			new RuntimeReachabilityEvidence(program.modules.length, typeInstanceCount, functionCount, blockCount, instructionCount, cleanupActionCount,
				observations.length));
	}

	static function collectInstruction(instruction:HxcIRInstruction, observations:Array<RuntimeIntentObservation>):Void {
		switch instruction.kind {
			case IRIOConstant(IRCString(_, _)):
				// The value itself is direct data, but generated C needs the selected
				// allocation-free `hxc_string` carrier header. Record that representation
				// intent just as managed type instances record their runtime owner.
				observations.push(new RuntimeIntentObservation("string-literal", "static-value", instruction.source));
			case IRIOUnary(operationId, _, implementation) | IRIOBinary(operationId, _, _, implementation):
				collectImplementation(implementation, operationId, instruction.source, observations);
			case IRIOConvert(_, kind, _, implementation, _):
				collectImplementation(implementation, conversionOperation(kind), instruction.source, observations);
			case IRIOCall(call):
				switch call.dispatch {
					case IRCDRuntime(featureId, operationId):
						observations.push(new RuntimeIntentObservation(featureId, operationId, instruction.source));
					case _:
				}
			case IRIOAllocate(_, _, implementation, _):
				collectImplementation(implementation, "allocation", instruction.source, observations);
			case IRIODeallocate(_, implementation):
				collectImplementation(implementation, "deallocation", instruction.source, observations);
			case IRIORetain(_, implementation):
				collectImplementation(implementation, "retain", instruction.source, observations);
			case IRIOAcquireManagedCarrier(_, _, IRMCARetainBorrowed(implementation)):
				// A borrowed branch result becomes the join's owner by retaining
				// through the same lifecycle operation as an ordinary alias. The
				// carrier node makes the control-flow transfer explicit, so runtime
				// selection must observe it instead of relying on emitter details.
				collectImplementation(implementation, "retain", instruction.source, observations);
			case IRIORelease(_, implementation):
				// This instruction is the in-block form of an owned cleanup action.
				// Keep the same runtime-plan operation ID used by edge cleanup so moving
				// a lexical boundary does not invent a second semantic requirement.
				collectImplementation(implementation, "cleanup-release", instruction.source, observations);
			case IRIOTrace(_, implementation):
				collectImplementation(implementation, "trace", instruction.source, observations);
			case _:
		}
	}

	static function collectCleanup(action:HxcIRCleanupAction, observations:Array<RuntimeIntentObservation>):Void {
		switch action.kind {
			case IRCARelease(_, implementation):
				collectImplementation(implementation, "cleanup-release", action.source, observations);
			case IRCADeallocate(_, implementation):
				collectImplementation(implementation, "cleanup-deallocate", action.source, observations);
			case _:
		}
	}

	static function collectImplementation(implementation:HxcIRImplementation, operationId:String, source:HxcSourceSpan,
			observations:Array<RuntimeIntentObservation>):Void {
		switch implementation {
			case IRIRuntime(featureId):
				observations.push(new RuntimeIntentObservation(featureId, operationId, source));
			case _:
		}
	}

	static function conversionOperation(kind:HxcIRConversionKind):String {
		return switch kind {
			case IRCNumericExact: "conversion-numeric-exact";
			case IRCNumericRoundBinary32: "conversion-numeric-round-binary32";
			case IRCNumericWidenBinary64: "conversion-numeric-widen-binary64";
			case IRCNumericWrapping: "conversion-numeric-wrapping";
			case IRCNumericSaturating: "conversion-numeric-saturating";
			case IRCNumericChecked: "conversion-numeric-checked";
			case IRCNullableInject: "conversion-nullable-inject";
			case IRCNullableUnwrap: "conversion-nullable-unwrap";
			case IRCPointer: "conversion-pointer";
			case IRCBox: "conversion-box";
			case IRCUnbox: "conversion-unbox";
			case IRCRepresentation: "conversion-representation";
		};
	}

	static function canonicalCandidates(input:Array<RuntimeRequirementCandidate>):Array<RuntimeCandidateEntry> {
		final candidates = input.map(candidate -> new RuntimeCandidateEntry(candidate));
		candidates.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.reasonIdentity, right.reasonIdentity));
		final result:Array<RuntimeCandidateEntry> = [];
		for (entry in candidates) {
			if (result.length == 0 || result[result.length - 1].reasonIdentity != entry.reasonIdentity) {
				result.push(entry);
			}
		}
		return result;
	}

	static function countObservations(input:Array<RuntimeIntentObservation>):Map<String, Int> {
		final counts:Map<String, Int> = [];
		for (observation in input) {
			final previous = counts.get(observation.identity);
			counts.set(observation.identity, previous == null ? 1 : previous + 1);
		}
		return counts;
	}

	static function canonicalObservations(input:Array<RuntimeIntentObservation>):Array<RuntimeIntentObservation> {
		final observations = input.copy();
		observations.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.key(), right.key()));
		final result:Array<RuntimeIntentObservation> = [];
		for (observation in observations) {
			if (result.length == 0 || result[result.length - 1].key() != observation.key()) {
				result.push(observation);
			}
		}
		return result;
	}

	static function candidatesFor(candidates:Map<String, Array<RuntimeRequirementCandidate>>,
			observation:RuntimeIntentObservation):Array<RuntimeRequirementCandidate> {
		final matching = candidates.get(observation.identity);
		if (matching != null)
			return matching;
		return
			internal('reachable HxcIR runtime intent `${observation.featureId}/${observation.operationId}` at `${observation.source.display()}` has no typed source reason',
			[observation.featureId]);
	}

	static function requiredCount(counts:Map<String, Int>, identity:String):Int {
		final count = counts.get(identity);
		if (count != null)
			return count;
		return internal('canonical runtime observation `$identity` lost its raw occurrence count');
	}

	static function internal<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail, featureIds);
}
