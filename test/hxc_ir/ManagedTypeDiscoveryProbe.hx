#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.lowering.CBodyManagedTypeDiscovery.directManagedTypeFacts;
#end

/** Check declaration-read sharing without changing nested-type evaluation. */
class ManagedTypeDiscoveryProbe {
	#if macro
	/** Run with the existing nominal fixture so this contract needs no extra compiler process. */
	public static function install():Void {
		Context.onAfterInitMacros(() -> {
			var reads = 0;
			final observed = switch Context.getType("NominalCacheFixture") {
				case TInst(reference, parameters):
					final wrapper:Ref<ClassType> = {
						get: () -> {
							reads++;
							return reference.get();
						},
						toString: () -> reference.toString()
					};
					TInst(wrapper, parameters);
				case _: throw "nominal fixture must be a class";
			};
			for (_ in 0...100) {
				final facts = directManagedTypeFacts(observed);
				if (facts == null || facts.array || facts.mapFamily != null || facts.date)
					throw "ordinary class acquired a managed family";
			}
			if (reads != 100)
				throw "discovery must read once per use without caching across uses";
			final date = directManagedTypeFacts(Context.getType("Date"));
			if (date == null || !date.date || date.array || date.mapFamily != null)
				throw "Date classification changed";
			var forced = false;
			final lazy = TLazy(() -> {
				forced = true;
				return observed;
			});
			if (directManagedTypeFacts(lazy) != null || forced || reads != 100)
				throw "direct recognition forced a lazy type";
			final parameterized = switch Context.getType("Array") {
				case TInst(reference, parameters):
					final deferred:Ref<ClassType> = {
						get: () -> {
							throw "parameterized declaration must stay with the original recognizers";
						},
						toString: () -> reference.toString()
					};
					TInst(deferred, parameters);
				case _: throw "Array must retain its type parameter";
			};
			if (directManagedTypeFacts(parameterized) != null || directManagedTypeFacts(Context.getType("Int")) != null)
				throw "direct recognition admitted a parameterized class or abstract";
			final broken:Ref<ClassType> = {
				get: () -> {
					throw "expected first-read failure";
				},
				toString: () -> "UnbuiltClass"
			};
			var failed = false;
			try {
				directManagedTypeFacts(TInst(broken, []));
			} catch (message:String) {
				failed = message == "expected first-read failure";
			}
			if (!failed)
				throw "direct recognition skipped the first declaration read or hid its failure";
		});
	}
	#end
}
