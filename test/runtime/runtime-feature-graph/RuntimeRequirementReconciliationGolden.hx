import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementCandidate;
import reflaxe.c.runtime.RuntimeFeatureError;
import reflaxe.c.runtime.RuntimeRequirementAnalyzer;

/**
	Exercises runtime-requirement reconciliation at a scale that exposes nested scans.

	The analyzer used to search all candidates for every HxcIR observation, then
	search all observations again for every candidate. That quadratic shape made
	Caxecraft spend almost a minute proving that two sets described the same
	runtime work. This fixture is deliberately large enough for that implementation
	to exceed the focused lane's bounded timeout. The indexed implementation does
	one canonical sort plus exact map lookups, while the forward/reversed comparison
	proves that discovery order cannot change reason IDs, source positions, or
	reachability counts.
**/
class RuntimeRequirementReconciliationGolden {
	static inline final FIXTURE_SIZE = 4096;

	/**
		Build and reconcile the same large semantic set in opposite input orders.

		The callback keeps this scale guard in the existing runtime-feature lane
		without adding a line to its source-position golden. It runs only after the
		reconciliation proof succeeds.
	**/
	public static function verify(next:() -> Void):Void {
		final instructions:Array<HxcIRInstruction> = [];
		final candidates:Array<RuntimeRequirementCandidate> = [];
		for (index in 0...FIXTURE_SIZE) {
			final source = new HxcSourceSpan("test/runtime/runtime-feature-graph/ReconciliationFixture.hx", index + 1, 1, index + 1, 2);
			instructions.push({
				id: 'instruction.string.$index',
				result: {id: 'value.string.$index', type: IRTString},
				kind: IRIOConstant(IRCString("x", 1)),
				source: source
			});
			candidates.push(new RuntimeRequirementCandidate(RuntimeFeatureId.parse("string-literal"), "static-value", "compiler-selected",
				"fixture:string-literal", source));
		}
		final source = new HxcSourceSpan("test/runtime/runtime-feature-graph/ReconciliationFixture.hx", 1, 1, FIXTURE_SIZE + 1, 1);
		final program = programWith(instructions, source);
		final analyzer = new RuntimeRequirementAnalyzer();
		final forward = analyzer.analyze(program, candidates);
		candidates.reverse();
		final reversed = analyzer.analyze(program, candidates);
		if (forward.reasons.length != FIXTURE_SIZE
			|| reversed.reasons.length != FIXTURE_SIZE
			|| forward.reachability.runtimeIntentCount != FIXTURE_SIZE
			|| reversed.reachability.runtimeIntentCount != FIXTURE_SIZE) {
			throw "large runtime-requirement reconciliation lost an observation, reason, or reachability count";
		}
		for (index in 0...FIXTURE_SIZE) {
			final left = forward.reasons[index];
			final right = reversed.reasons[index];
			if (left.id != right.id
				|| left.featureId.text() != right.featureId.text()
				|| left.operationId != right.operationId
				|| left.kind != right.kind
				|| left.surface != right.surface
				|| left.source.display() != right.source.display()
				|| left.alternative != right.alternative) {
				throw 'runtime-requirement reconciliation changed canonical reason $index with discovery order';
			}
		}
		verifySameSpanReasons(analyzer);
		verifyDeclarationCarriers(analyzer);
		next();
	}

	/**
		Prove that source spans are locations, not unique semantic identities.

		One Haxe expression can create two ownership operations at the same
		characters—for example, retaining a local String and retaining that String
		inside a new record. Two HxcIR observations may therefore keep two distinct
		diagnostic reasons. One observation may not: that would leave a source
		reason unsupported by reachable semantic work.
	**/
	static function verifySameSpanReasons(analyzer:RuntimeRequirementAnalyzer):Void {
		final source = new HxcSourceSpan("test/runtime/runtime-feature-graph/SameSpanReasons.hx", 4, 7, 4, 15);
		final observations:Array<HxcIRInstruction> = [
			{
				id: "retain.local",
				result: null,
				kind: IRIORetain(IRPLocal("local.alias"), IRIRuntime("array")),
				source: source
			},
			{
				id: "retain.record",
				result: null,
				kind: IRIORetain(IRPLocal("local.record-field"), IRIRuntime("array")),
				source: source
			}
		];
		final first = new RuntimeRequirementCandidate(RuntimeFeatureId.parse("array"), "retain", "runtime-operation", "ordinary Haxe Array local alias",
			source);
		final second = new RuntimeRequirementCandidate(RuntimeFeatureId.parse("array"), "retain", "runtime-operation",
			"managed Array captured by a closed record", source);
		final candidates = [second, first, first];
		final analysis = analyzer.analyze(programWith(observations, source), candidates);
		if (analysis.reasons.length != 2
			|| analysis.reachability.runtimeIntentCount != 2
			|| analysis.reasons[0].surface != "managed Array captured by a closed record"
			|| analysis.reasons[1].surface != "ordinary Haxe Array local alias") {
			throw "same-span runtime reconciliation lost distinct reasons, exact deduplication, or canonical order";
		}

		var rejected = false;
		try {
			analyzer.analyze(programWith([observations[0]], source), candidates);
		} catch (error:RuntimeFeatureError) {
			rejected = error.message.indexOf("2 distinct runtime source reasons describe only 1 reachable `retain` operation") >= 0;
		}
		if (!rejected)
			throw "one runtime observation incorrectly admitted two distinct source reasons";
	}

	/**
		Prove that stored String types select their C carriers without fake work.

		A reachable type declaration is emitted even when no function constructs
		one of its values. Its complete C layout therefore needs the direct or
		managed String runtime header. This test keeps that type-only requirement
		separate from literal and concatenation operations.
	**/
	static function verifyDeclarationCarriers(analyzer:RuntimeRequirementAnalyzer):Void {
		final directSource = new HxcSourceSpan("test/runtime/runtime-feature-graph/DeclarationCarriers.hx", 4, 3, 4, 16);
		final managedSource = new HxcSourceSpan("test/runtime/runtime-feature-graph/DeclarationCarriers.hx", 5, 3, 5, 17);
		final program = programWith([], new HxcSourceSpan("test/runtime/runtime-feature-graph/DeclarationCarriers.hx", 1, 1, 7, 1));
		program.modules[0].types.push({
			id: "type.fixture.declaration-carriers",
			displayName: "fixture.DeclarationCarriers",
			kind: IRTKAggregate([
				{
					name: "direct",
					type: IRTString,
					mutable: false,
					source: directSource
				},
				{
					name: "managed",
					type: IRTManagedString,
					mutable: false,
					source: managedSource
				}
			]),
			source: directSource
		});
		final analysis = analyzer.analyze(program, [
			new RuntimeRequirementCandidate(RuntimeFeatureId.parse("string-literal"), "type-carrier", "runtime-representation",
				"closed Haxe record field `direct`", directSource),
			new RuntimeRequirementCandidate(RuntimeFeatureId.parse("string"), "type-carrier", "runtime-representation", "closed Haxe record field `managed`",
				managedSource)
		]);
		if (analysis.reasons.length != 2
			|| analysis.reachability.runtimeIntentCount != 2
			|| analysis.reasons[0].featureId.text() != "string"
			|| analysis.reasons[1].featureId.text() != "string-literal"
			|| analysis.reasons[0].operationId != "type-carrier"
			|| analysis.reasons[1].operationId != "type-carrier") {
			throw "stored String declarations lost their direct or managed runtime carrier";
		}
	}

	/** Build the smallest HxcIR program needed by reconciliation-only tests. */
	static function programWith(instructions:Array<HxcIRInstruction>, source:HxcSourceSpan):HxcIRProgram {
		return {
			schemaVersion: HxcIRValidator.SCHEMA_VERSION,
			dispatch: {layouts: [], slots: [], tables: []},
			modules: [
				{
					id: "fixture.RuntimeRequirementReconciliation",
					types: [],
					typeInstances: [],
					globals: [],
					functions: [
						{
							id: "fixture.RuntimeRequirementReconciliation.main",
							displayName: "fixture.RuntimeRequirementReconciliation.main",
							parameters: [],
							borrowedClassParameterIds: [],
							borrowedInterfaceParameterIds: [],
							borrowedClassLocalIds: [],
							borrowedInterfaceLocalIds: [],
							managedRoots: [],
							locals: [],
							returnType: IRTVoid,
							borrowedSpanReturn: null,
							failureConvention: IRFCInfallible,
							entryBlockId: "entry",
							blocks: [
								{
									id: "entry",
									parameters: [],
									instructions: instructions,
									terminator: {kind: IRTReturn(null, []), source: source},
									source: source
								}
							],
							cleanupRegions: [],
							source: source
						}
					],
					source: source
				}
			]
		};
	}
}
