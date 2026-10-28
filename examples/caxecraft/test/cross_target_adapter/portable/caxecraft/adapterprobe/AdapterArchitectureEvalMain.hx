package caxecraft.adapterprobe;

/** Portable composition root that prints only the shared probe result. */
final class AdapterArchitectureEvalMain {
	/** Execute the shared rules through the portable adapter source root. */
	static function main():Void
		Sys.println(AdapterArchitectureProbe.selfCheck());
}
