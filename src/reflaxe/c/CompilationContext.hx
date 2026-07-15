package reflaxe.c;

/** Mutable compiler state has one instance per Haxe compilation request. */
class CompilationContext {
	public final profile:CProfile;

	public function new(profile:CProfile) {
		this.profile = profile;
	}
}
