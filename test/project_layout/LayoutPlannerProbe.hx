#if macro
import haxe.macro.Expr;
import reflaxe.c.emit.CProjectLayout;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlanner;
import reflaxe.c.emit.ProjectEmissionError;
#end

/** Synthetic path-policy checks that never create reserved host paths. */
class LayoutPlannerProbe {
	public static function main():Void {}

	#if macro
	public static macro function verify():Expr {
		final planner = new CProjectLayoutPlanner();
		final longComponent = "haxe_utf8_" + StringTools.lpad("", "a", 220);
		final longModule = 'layout.$longComponent.Main';
		final plan = planner.plan(CProjectLayout.Split, [
			"layout.con.Device",
			"layout.haxe_utf8_literal.Main",
			"layout.normal.Main",
			"layout.éclair.Main",
			longModule
		]);
		require(plan.module("layout.con.Device").relativeStem == "layout/haxe_utf8_636f6e/Device", "Windows device component encoding");
		require(plan.module("layout.haxe_utf8_literal.Main").relativeStem == "layout/haxe_utf8_686178655f757466385f6c69746572616c/Main",
			"encoding namespace escaping");
		require(plan.module("layout.éclair.Main").relativeStem == "layout/haxe_utf8_c3a9636c616972/Main", "short UTF-8 lossless encoding");
		final bounded = plan.module(longModule).relativeStem.split("/")[1];
		require(haxe.io.Bytes.ofString(bounded).length <= CProjectLayoutPlanner.MAX_COMPONENT_BYTES && bounded.indexOf("_sha256_") != -1,
			"bounded long-component digest encoding");
		final packages = planner.plan(CProjectLayout.Package, ["Main", "layout.normal.Main", "layout.normal.Other", "root.Main"]);
		require(packages.packageForModule("Main").headerPath == "include/hxc/packages/package.h", "root package header");
		require(packages.packageForModule("layout.normal.Main").headerPath == "include/hxc/packages/layout/normal/package.h", "nested package header");
		require(packages.packageForModule("layout.normal.Other").sourcePath == "src/packages/layout/normal/package.c", "shared package source");
		require(packages.packageForModule("root.Main").headerPath == "include/hxc/packages/root/package.h", "named root package separation");
		var collisionRejected = false;
		try {
			planner.plan(CProjectLayout.Split, ["layout.Alpha", "layout.alpha"]);
		} catch (_:ProjectEmissionError) {
			collisionRejected = true;
		}
		require(collisionRejected, "case-insensitive generated-path collision");
		var packageCollisionRejected = false;
		try {
			planner.plan(CProjectLayout.Package, ["layout.Alpha.One", "layout.alpha.Two"]);
		} catch (_:ProjectEmissionError) {
			packageCollisionRejected = true;
		}
		require(packageCollisionRejected, "case-insensitive generated package-path collision");
		for (layout in [CProjectLayout.Split, CProjectLayout.Unity]) {
			final nonPackagePlan = planner.plan(layout, ["layout.Alpha.One", "layout.alpha.Two"]);
			require(nonPackagePlan.modules.length == 2, "package-only collision does not change split or unity");
		}
		return macro null;
	}

	static function require(condition:Bool, label:String):Void {
		if (!condition)
			throw 'project layout planner probe failed: $label';
	}
	#end
}
