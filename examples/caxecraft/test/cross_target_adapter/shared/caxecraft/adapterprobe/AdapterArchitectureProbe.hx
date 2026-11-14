package caxecraft.adapterprobe;

import caxecraft.adapterprobe.AdapterRules.conditionalScore;
import caxecraft.adapterprobe.AdapterRules.interfaceScore;
import caxecraft.adapterprobe.AdapterRules.sourceRootScore;
import caxecraft.adapterprobe.interfaceport.SelectedNormalizationPort;

/**
	Executes all three adapter strategies from one conditional-free entry module.

	The C and portable builds select only their composition and adapter roots.
	The shared probe and rule bytes are identical across both compilations.
**/
final class AdapterArchitectureProbe {
	/** Keep the generated entry reachable; the native harness calls `selfCheck`. */
	static function main():Void
		selfCheck();

	/** Return zero only when all strategies preserve the same gameplay result. */
	public static function selfCheck():Int {
		final value = 513;
		final expected = 10;
		if (sourceRootScore(value) != expected)
			return 1;
		if (interfaceScore(new SelectedNormalizationPort(), value) != expected)
			return 2;
		if (conditionalScore(value) != expected)
			return 3;
		final owner = new OwnedCarrierProbe();
		owner.writeFirst(7);
		if (OwnedCarrierProbe.readFirst(owner.view()) != 7)
			return 4;
		return 0;
	}
}
