package reflaxe.c.plan;

/** Stable, inspectable failure raised before any C file can be emitted. */
class CDeclarationPlanningError extends haxe.Exception {
	public static inline final DIAGNOSTIC_ID = "HXC5002";

	public final modulePaths:Array<String>;

	public function new(message:String, ?modulePaths:Array<String>) {
		this.modulePaths = modulePaths == null ? [] : modulePaths.copy();
		super('$DIAGNOSTIC_ID: $message');
	}
}
