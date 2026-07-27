import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementCandidate;
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
		final program:HxcIRProgram = {
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
		next();
	}
}
