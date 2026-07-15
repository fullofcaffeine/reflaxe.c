class BootstrapProbe {
	#if reflaxe_c_bootstrap_ready
	static final bootstrap = "1";
	#else
	static final bootstrap = "0";
	#end

	#if reflaxe_c_init_ready
	static final initialized = "1";
	#else
	static final initialized = "0";
	#end

	#if c
	static final publicTarget = "1";
	#else
	static final publicTarget = "0";
	#end

	#if reflaxe_c
	static final implementation = "1";
	#else
	static final implementation = "0";
	#end

	#if target.unicode
	static final unicode = "1";
	#else
	static final unicode = "0";
	#end

	#if target.utf16
	static final utf16 = "1";
	#else
	static final utf16 = "0";
	#end

	static function main():Void {
		Sys.println([
			"bootstrap=" + bootstrap,
			"init=" + initialized,
			"c=" + publicTarget,
			"reflaxe_c=" + implementation,
			"unicode=" + unicode,
			"utf16=" + utf16
		].join(" "));
	}
}
