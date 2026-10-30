#if macro
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import reflaxe.c.CompilationContext;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
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
		checkValueClassification();
		checkPreparationBoundaries();
		Sys.println("ENUM_PROVENANCE_DELTA_OK");
	}

	/**
		Reuse completed and recursively discovered enums without skipping validation.

		The same source type has one instance per registry. Invalid arguments must
		still fail after a valid use, and a new registry must own fresh preparation.
	**/
	static function checkPreparationBoundaries():Void {
		final source = "test/enum_lowering/fixtures/provenance/ProvenanceFixture.hx";
		final position = Context.makePosition({file: source, min: 0, max: 1});
		final registry = new CBodyAggregateRegistry(new CompilationContext(reflaxe.c.CProfile.Portable));
		final ordinary = Context.getType("ProvenanceFixture.FirstReasonEnum");
		final recursive = Context.getType("ProvenanceFixture.RecursiveReasonEnum");
		final fail = (position:Position, message:String) -> throw new haxe.Exception(message);
		for (_ in 0...4) {
			registry.valueType(ordinary, position, "ProvenanceFixture", source, fail, "boundary");
			registry.valueType(recursive, position, "ProvenanceFixture", source, fail, "boundary");
		}
		final prepared = registry.canonicalEnums();
		if (prepared.length != 2)
			throw new haxe.Exception("repeated or recursive enum preparation created another instance");
		final recursiveInstances = [for (value in prepared) if (value.haxePath == "RecursiveReasonEnum") value];
		if (recursiveInstances.length != 1 || recursiveInstances[0].cases.length != 2)
			throw new haxe.Exception("recursive enum preparation lost a constructor");
		final malformed = switch ordinary {
			case TEnum(reference, _): TEnum(reference, [Context.getType("Int")]);
			case _: throw new haxe.Exception("ordinary enum fixture is not an enum");
		};
		for (entry in [
			{type: malformed, diagnostic: "boundary:enum-argument-count:FirstReasonEnum:1-for-0"},
			{type: Context.getType("ProvenanceFixture.ExternalReasonEnum"), diagnostic: "boundary:extern-enum:ExternalReasonEnum"}
		]) {
			var diagnostic:Null<String> = null;
			try {
				registry.valueType(entry.type, position, "ProvenanceFixture", source, fail, "boundary");
			} catch (error:haxe.Exception) {
				diagnostic = error.message;
			}
			if (diagnostic != entry.diagnostic)
				throw new haxe.Exception('enum preparation diagnostic differed: $diagnostic');
		}
		final other = new CBodyAggregateRegistry(new CompilationContext(reflaxe.c.CProfile.Portable));
		other.valueType(recursive, position, "ProvenanceFixture", source, fail, "boundary");
		if (other.canonicalEnums().length != 1 || other.canonicalEnums()[0] == recursiveInstances[0])
			throw new haxe.Exception("enum preparation escaped its owning registry");
	}

	/**
		Keep source evidence and generic identity through the ordinary value classifier.

		A repeated enum use may skip unrelated type families, but it must still
		record each distinct source range. Aliases share their enum; different
		payload arguments keep separate layouts. These checks use synthetic source
		ranges and do not depend on the classifier's dispatch order.
	**/
	static function checkValueClassification():Void {
		final registry = new CBodyAggregateRegistry(new CompilationContext(reflaxe.c.CProfile.Portable));
		final source = "test/enum_lowering/fixtures/provenance/ProvenanceFixture.hx";
		final first = Context.makePosition({file: source, min: 0, max: 1});
		final second = Context.makePosition({file: source, min: 2, max: 3});
		final generic = switch Context.getType("ProvenanceFixture.GenericReasonEnum") {
			case TEnum(reference, _): reference;
			case _: throw new haxe.Exception("generic provenance fixture is not an enum");
		};
		final types = [
			Context.getType("ProvenanceFixture.FirstReasonEnum"),
			Context.getType("ProvenanceFixture.FirstReasonAlias"),
			TEnum(generic, [Context.getType("Int")]),
			TEnum(generic, [Context.getType("Bool")]),
			Context.getType("other.FirstReasonEnum")
		];
		final values = [
			for (type in types)
				registry.valueType(type, first, "ProvenanceFixture", source, (position, message) -> Context.error(message, position), "value-provenance")
		];
		if (values[0].cSpelling != values[1].cSpelling
			|| values[2].cSpelling == values[3].cSpelling
			|| values[0].cSpelling == values[4].cSpelling)
			throw new haxe.Exception("enum classification changed alias or generic identity");
		final checkpoint = registry.enumReasonCheckpoint();
		for (_ in 0...2)
			for (type in types)
				registry.valueType(type, second, "ProvenanceFixture", source, (position, message) -> Context.error(message, position), "value-provenance");
		final delta = registry.enumReasonsSince(checkpoint);
		final enums = registry.canonicalEnums();
		if (enums.length != 4)
			throw new haxe.Exception("enum classification lost a concrete specialization");
		for (value in enums) {
			final added = delta.get(value.instanceId);
			if (added == null || added.length != 1 || value.reasonCount() != 2)
				throw new haxe.Exception("enum classification lost or duplicated a source range");
		}
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
