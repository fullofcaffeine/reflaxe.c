#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CompilationContext;
import reflaxe.c.CProfile;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
#end

/** Checks that repeated nominal classifications reuse only the current request's plan. */
class NominalCacheProbe {
	#if macro
	/** Wait for typed fixture declarations before checking cache boundaries. */
	public static function install():Void {
		Context.onAfterInitMacros(() -> {
			final registry = new CBodyAggregateRegistry(new CompilationContext(Portable));
			final type = Context.getType("NominalCacheFixture");
			final position = Context.currentPos();
			final fail = (position, message) -> Context.error(message, position);
			final first = registry.valueType(type, position, "NominalCacheFixture", "NominalCacheFixture.hx", fail, "first");
			// Observe an already classified reference without changing the class it
			// resolves to. A hit must not encode its full description again.
			var repeatedDescriptions = 0;
			final observedType = switch type {
				case TInst(reference, parameters):
					final observedReference:Ref<ClassType> = {
						get: () -> {
							repeatedDescriptions++;
							return reference.get();
						},
						toString: () -> reference.toString()
					};
					TInst(observedReference, parameters);
				case _: throw "nominal fixture is not a class instance type";
			};
			for (_ in 0...100)
				if (registry.valueType(observedType, position, "OtherCaller", "OtherCaller.hx", fail, "repeat") != first)
					throw "nominal cache lost wrapper identity across use sites";
			if (repeatedDescriptions != 0)
				throw "nominal cache rebuilt a class description on a hit";
			if (registry.exactNominalHits() != 100 || registry.exactNominalMisses() != 1)
				throw "nominal cache changed first-use or repeat accounting";
			final string = registry.valueType(Context.getType("String"), position, "NominalCacheFixture", "NominalCacheFixture.hx", fail, "string");
			switch string.kind {
				case CBVKStaticString("String"):
				case _: throw "nominal cache changed String classification";
			}
			if (registry.exactNominalHits() != 100 || registry.exactNominalMisses() != 1)
				throw "String entered the nominal cache";
			final otherRequest = new CBodyAggregateRegistry(new CompilationContext(Portable));
			if (otherRequest.valueType(type, position, "NominalCacheFixture", "NominalCacheFixture.hx", fail, "new-request") == first)
				throw "nominal cache leaked a plan across requests";
			if (otherRequest.exactNominalHits() != 0 || otherRequest.exactNominalMisses() != 1)
				throw "new request reused old cache accounting";
			Sys.println("NOMINAL_CACHE_OK");
		});
	}
	#end
}
