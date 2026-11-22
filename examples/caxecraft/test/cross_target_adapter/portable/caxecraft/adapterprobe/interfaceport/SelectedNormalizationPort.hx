package caxecraft.adapterprobe.interfaceport;

import caxecraft.adapterprobe.AdapterRules.NormalizationPort;

/** Portable composition-root implementation of the structural adapter. */
final class SelectedNormalizationPort implements NormalizationPort {
	/** Construct the stateless portable adapter. */
	public function new() {}

	/** Normalize one positive value with ordinary Haxe arithmetic. */
	public inline function normalize(value:Int):Int
		return value % 256;
}
