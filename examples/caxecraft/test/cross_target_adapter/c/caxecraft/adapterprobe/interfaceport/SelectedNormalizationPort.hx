package caxecraft.adapterprobe.interfaceport;

import c.IntConvert;
import c.UInt8;
import caxecraft.adapterprobe.AdapterRules.NormalizationPort;

/** Native-C composition-root implementation of the structural adapter. */
final class SelectedNormalizationPort implements NormalizationPort {
	/** Construct the stateless native adapter. */
	public function new() {}

	/** Normalize one positive value through the exact C byte carrier. */
	public inline function normalize(value:Int):Int {
		final normalized:UInt8 = IntConvert.modulo(value);
		return IntConvert.exact(normalized);
	}
}
