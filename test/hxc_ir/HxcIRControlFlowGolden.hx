import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRControlFlowAnalysis;
import reflaxe.c.ir.HxcSourceSpan;

/**
	Checks HxcIR dominance against a definition that does not share its algorithm.

	The production compiler builds an immediate-dominator tree for speed. This
	test instead asks a simpler question for every pair of blocks: after removing
	the proposed dominator, can graph search still reach the destination from the
	entry? If not, every route required that block. Comparing those independent
	answers catches both an incorrect optimization and a test that merely copies
	the implementation's reasoning.
**/
private enum ControlFlowTestEdgeKind {
	NormalEdge;
	InstructionFailureEdge;
}

private typedef ControlFlowTestEdge = {
	final source:String;
	final target:String;
	final kind:ControlFlowTestEdgeKind;
}

private typedef ControlFlowTestCase = {
	final id:String;
	final entry:String;
	final blockOrder:Array<String>;
	final edges:Array<ControlFlowTestEdge>;
}

/**
	Owns focused, deterministic examples for the shared HxcIR control-flow proof.

	The cases cover straight-line flow, a branch join, a loop, unreachable code,
	reversed declaration order, and an instruction that can jump to a failure
	block before its terminator. Production and test algorithms share only the
	graph input and the public facts API.
**/
class HxcIRControlFlowGolden {
	/** Compare every public fact with the graph-removal oracle. */
	public static function run():Void {
		for (testCase in testCases()) {
			verifyCase(testCase, testCase.blockOrder);
			final reversed = testCase.blockOrder.copy();
			reversed.reverse();
			verifyCase(testCase, reversed);
		}
	}

	static function verifyCase(testCase:ControlFlowTestCase, blockOrder:Array<String>):Void {
		final fn = buildFunction(testCase, blockOrder);
		final facts = new HxcIRControlFlowAnalysis().analyze(fn);
		final graph = graphFor(testCase);
		final reachable = reachableWithout(graph, testCase.entry, null);

		for (blockId in testCase.blockOrder) {
			final expectedStrict:Array<String> = [];
			for (candidateId in testCase.blockOrder) {
				final expected = reachable.exists(blockId)
					&& reachable.exists(candidateId)
					&& !reachableWithout(graph, testCase.entry, candidateId).exists(blockId);
				final actual = facts.dominates(candidateId, blockId);
				if (actual != expected)
					throw '${testCase.id}: dominates($candidateId, $blockId) was $actual; graph-removal oracle expected $expected';
				if (expected && candidateId != blockId)
					expectedStrict.push(candidateId);
			}
			expectedStrict.sort(compareUtf8);
			final actualStrict = facts.strictDominatorsOf(blockId);
			if (!sameStrings(actualStrict, expectedStrict))
				throw '${testCase.id}: strict dominators for $blockId were ${actualStrict.join(",")}; expected ${expectedStrict.join(",")}';

			final expectedFailureJump = hasFailureEdge(testCase, blockId);
			if (facts.hasInstructionFailureJump(blockId) != expectedFailureJump)
				throw '${testCase.id}: instruction failure-jump flag for $blockId did not match $expectedFailureJump';
		}

		if (facts.dominates("missing", testCase.entry)
			|| facts.dominates(testCase.entry, "missing")
			|| facts.strictDominatorsOf("missing").length != 0
			|| facts.hasInstructionFailureJump("missing"))
			throw '${testCase.id}: unknown block IDs must not acquire control-flow facts';
	}

	static function testCases():Array<ControlFlowTestCase> {
		return [
			{
				id: "linear-with-unreachable",
				entry: "z-entry",
				blockOrder: ["m-exit", "ghost", "z-entry", "a-middle"],
				edges: [edge("z-entry", "a-middle"), edge("a-middle", "m-exit")]
			},
			{
				id: "diamond",
				entry: "entry",
				blockOrder: ["join", "right", "entry", "left"],
				edges: [
					edge("entry", "left"),
					edge("entry", "right"),
					edge("left", "join"),
					edge("right", "join")
				]
			},
			{
				id: "loop",
				entry: "entry",
				blockOrder: ["exit", "body", "header", "entry"],
				edges: [
					edge("entry", "header"),
					edge("header", "body"),
					edge("header", "exit"),
					edge("body", "header")
				]
			},
			{
				id: "instruction-failure",
				entry: "entry",
				blockOrder: ["join", "failure", "normal", "entry"],
				edges: [
					edge("entry", "normal"),
					edge("entry", "failure", InstructionFailureEdge),
					edge("normal", "join"),
					edge("failure", "join")
				]
			}
		];
	}

	static function buildFunction(testCase:ControlFlowTestCase, blockOrder:Array<String>):HxcIRFunction {
		final blocks:Array<HxcIRBlock> = [];
		for (blockId in blockOrder) {
			final normalTargets = targets(testCase, blockId, NormalEdge);
			final failureTargets = targets(testCase, blockId, InstructionFailureEdge);
			final instructions:Array<HxcIRInstruction> = [];
			for (index => target in failureTargets)
				instructions.push({
					id: '$blockId.failure.$index',
					result: null,
					kind: IRIOConvert("unused.value", IRCNumericExact, IRTInt(32, true), IRIStatic, {
						kind: IRFException,
						target: IRFTBlock(target),
						arguments: [],
						cleanup: []
					}),
					source: source()
				});
			blocks.push({
				id: blockId,
				parameters: [],
				instructions: instructions,
				terminator: {
					kind: switch normalTargets.length {
						case 0: IRTReturn(null, []);
						case 1: IRTJump(blockEdge(normalTargets[0]));
						case 2: IRTBranch("unused.condition", blockEdge(normalTargets[0]), blockEdge(normalTargets[1]));
						case count: throw '${testCase.id}: test block $blockId has unsupported normal successor count $count';
					},
					source: source()
				},
				source: source()
			});
		}
		return {
			id: 'control-flow.${testCase.id}',
			displayName: 'control-flow.${testCase.id}',
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
			entryBlockId: testCase.entry,
			blocks: blocks,
			cleanupRegions: [],
			source: source()
		};
	}

	static function graphFor(testCase:ControlFlowTestCase):Map<String, Array<String>> {
		final graph:Map<String, Array<String>> = [];
		for (blockId in testCase.blockOrder)
			graph.set(blockId, []);
		for (item in testCase.edges) {
			final outgoing = graph.get(item.source);
			if (outgoing != null && outgoing.indexOf(item.target) == -1)
				outgoing.push(item.target);
		}
		return graph;
	}

	/**
		Search the graph while pretending one block does not exist.

		This deliberately favors clarity over speed: the graphs are tiny and the
		repeated search is the independent definition used to check production.
	**/
	static function reachableWithout(graph:Map<String, Array<String>>, entry:String, removed:Null<String>):Map<String, Bool> {
		final reached:Map<String, Bool> = [];
		if (entry == removed || !graph.exists(entry))
			return reached;
		final pending = [entry];
		var next = 0;
		while (next < pending.length) {
			final blockId = pending[next++];
			if (blockId == removed || reached.exists(blockId))
				continue;
			reached.set(blockId, true);
			final outgoing = graph.get(blockId);
			if (outgoing != null)
				for (target in outgoing)
					if (target != removed && graph.exists(target) && !reached.exists(target))
						pending.push(target);
		}
		return reached;
	}

	static function targets(testCase:ControlFlowTestCase, sourceId:String, kind:ControlFlowTestEdgeKind):Array<String> {
		final result:Array<String> = [];
		for (item in testCase.edges)
			if (item.source == sourceId && item.kind == kind && result.indexOf(item.target) == -1)
				result.push(item.target);
		return result;
	}

	static function hasFailureEdge(testCase:ControlFlowTestCase, sourceId:String):Bool {
		for (item in testCase.edges)
			if (item.source == sourceId && item.kind == InstructionFailureEdge)
				return true;
		return false;
	}

	static function edge(source:String, target:String, kind:ControlFlowTestEdgeKind = NormalEdge):ControlFlowTestEdge
		return {source: source, target: target, kind: kind};

	static function blockEdge(targetBlockId:String):HxcIRBlockEdge
		return {targetBlockId: targetBlockId, arguments: [], cleanup: []};

	static function sameStrings(left:Array<String>, right:Array<String>):Bool {
		if (left.length != right.length)
			return false;
		for (index in 0...left.length)
			if (left[index] != right[index])
				return false;
		return true;
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = haxe.io.Bytes.ofString(left);
		final rightBytes = haxe.io.Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	static function source():HxcSourceSpan
		return new HxcSourceSpan("test/hxc_ir/fixtures/ControlFlow.hx", 1, 1, 1, 2);
}
