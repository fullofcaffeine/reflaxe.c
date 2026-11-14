import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyControlFlow;

/**
	Checks that backward analysis preserves execution order across block layouts.

	A long jump chain must become the exact authored sequence. The expected plan
	comes from that simple contract, independently of dominance calculations.
	Reversing storage order after the entry exercises graph-based traversal.
**/
class BodyControlFlowOrderProbe {
	/** Run the focused contract without loading the compiler macro bootstrap. */
	public static function main():Void {
		run();
		Sys.println("BODY_CONTROL_FLOW_ORDER_OK");
	}

	/** Require exact block coverage, order, and return completion in both layouts. */
	public static function run():Void {
		final source = new HxcSourceSpan("synthetic-chain.hx", 1, 1, 1, 2);
		final size = 128;
		final blocks:Array<HxcIRBlock> = [
			for (index in 0...size)
				{
					id: 'block-$index',
					parameters: [],
					instructions: [],
					source: source,
					terminator: {
						source: source,
						kind: index + 1 == size ? IRTReturn(null, []) : IRTJump({
							targetBlockId: 'block-${index + 1}',
							arguments: [],
							cleanup: []
						})
					}
				}
		];
		for (reverse in [false, true]) {
			final tail = blocks.slice(1);
			if (reverse)
				tail.reverse();
			final fn:HxcIRFunction = {
				id: "chain",
				displayName: "chain",
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
				entryBlockId: "block-0",
				blocks: [blocks[0]].concat(tail),
				cleanupRegions: [],
				source: source
			};
			final plan = new CBodyControlFlowPlanner().plan(fn);
			new CBodyControlFlowPlanVerifier().requireValid(fn, plan);
			switch plan {
				case CCFStructured(region, deferred):
					if (deferred.length != 0 || region.nodes.length != size - 1)
						throw "jump chain changed block coverage";
					for (index => node in region.nodes)
						switch node {
							case CFNBlock(id) if (id == 'block-$index'):
							case _: throw 'jump chain changed execution order at $index';
						}
					switch region.completion {
						case CFCSharedAbrupt(owner, target) if (owner == 'block-${size - 2}' && target == 'block-${size - 1}'):
						case _: throw 'jump chain lost its final return: ${region.completion}';
					}
				case _:
					throw "jump chain lost structured control flow";
			}
		}
	}
}
