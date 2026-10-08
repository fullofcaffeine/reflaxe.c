#if macro
import haxe.macro.Context;
import reflaxe.c.CompilationContext;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRegistry;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance;
import reflaxe.c.lowering.CBodyEmissionError;
#end

/**
	Checks that function replay receives new enum source ranges without old history.
	The fixture includes duplicates, a later enum, and mutations of returned arrays.
**/
class EnumProvenanceProbe {
	#if macro
	/** Run against real typed enums without invoking the full C emission pipeline. */
	public static function run():Void {
		Context.onAfterInitMacros(check);
	}

	/** Type discovery must wait until Haxe has completed macro initialization. */
	static function check():Void {
		final registry = new CBodyEnumRegistry(new CompilationContext(reflaxe.c.CProfile.Portable),
			(type, position, owner, path, fail, node) -> throw new haxe.Exception("payload-free fixture requested a payload type"));
		final first = prepare(registry, "FirstReasonEnum");
		for (line in 1...2001)
			first.addReason(new HxcSourceSpan("history.hx", line, 1, line, 2));
		final checkpoint = registry.reasonCheckpoint();
		final added = new HxcSourceSpan("added.hx", 8, 1, 8, 2);
		first.addReason(added);
		first.addReason(added);
		first.addReason(new HxcSourceSpan("history.hx", 1, 1, 1, 2));
		final second = prepare(registry, "SecondReasonEnum");
		second.addReason(new HxcSourceSpan("second.hx", 3, 1, 3, 2));
		final delta = registry.reasonsSince(checkpoint);
		final firstDelta = delta.get(first.instanceId);
		if (firstDelta == null || firstDelta.length != 1 || firstDelta[0].display() != added.display())
			throw new haxe.Exception("enum delta included history or lost the new range");
		final secondDelta = delta.get(second.instanceId);
		if (secondDelta == null || secondDelta.length != 2)
			throw new haxe.Exception("new enum delta lost its initial or added range");
		firstDelta.resize(0);
		final repeated = registry.reasonsSince(checkpoint).get(first.instanceId);
		if (repeated == null || repeated.length != 1)
			throw new haxe.Exception("caller mutated enum provenance history");
		if (registry.reasonsSince(registry.reasonCheckpoint()).iterator().hasNext())
			throw new haxe.Exception("unchanged enum history produced a delta");
		final canonical = first.canonicalReasons();
		if (canonical[0].display() != added.display())
			throw new haxe.Exception("canonical provenance exposed insertion order");
		var rejected = false;
		try {
			first.reasonsSince(first.reasonCount() + 1);
		} catch (_:CBodyEmissionError) {
			rejected = true;
		}
		if (!rejected)
			throw new haxe.Exception("invalid provenance position was accepted");
		Sys.println("ENUM_PROVENANCE_DELTA_OK");
	}

	/** Discover a real payload-free enum through the production registry. */
	static function prepare(registry:CBodyEnumRegistry, name:String):CPreparedBodyEnumInstance {
		final position = Context.makePosition({file: "test/enum_lowering/fixtures/provenance/ProvenanceFixture.hx", min: 0, max: 1});
		return switch Context.getType("ProvenanceFixture." + name) {
			case TEnum(reference, parameters):
				registry.require(reference, parameters, position, "ProvenanceFixture", "test/enum_lowering/fixtures/provenance/ProvenanceFixture.hx",
					(position, message) -> Context.fatalError(message, position), "provenance-test");
			case _: throw new haxe.Exception("provenance fixture is not an enum");
		};
	}
	#end
}
