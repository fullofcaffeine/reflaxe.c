package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.naming.CSymbolRequest.CSymbolKind;
import reflaxe.c.naming.CSymbolRequest.CSymbolNamespace;
import reflaxe.c.naming.CSymbolRequest.CSymbolVisibility;
import reflaxe.c.plan.CDeclarationPlanner;

/**
	How compiler-owned C implementation files are arranged on disk.

	This is deliberately a closed setting. Layout changes file ownership and
	build inputs, but it must never change HxcIR, representation, naming, or C
	semantics.
**/
enum abstract CProjectLayout(String) to String {
	/** Source-shaped private headers and sources grouped by Haxe module. */
	var Split = "split";

	/** Private headers and sources coalesced by normalized Haxe package. */
	var Package = "package";

	/** One ordinary implementation unit for convenient embedding and inspection. */
	var Unity = "unity";
}

/** Deterministic paths assigned to one normalized Haxe module. */
class CProjectModuleLayout {
	public final modulePath:String;
	public final packagePath:String;
	public final relativeStem:String;
	public final headerPath:String;
	public final headerInclude:String;
	public final sourcePath:String;

	public function new(modulePath:String, packagePath:String, relativeStem:String) {
		this.modulePath = modulePath;
		this.packagePath = packagePath;
		this.relativeStem = relativeStem;
		headerPath = 'include/hxc/modules/$relativeStem.h';
		headerInclude = 'hxc/modules/$relativeStem.h';
		sourcePath = 'src/modules/$relativeStem.c';
	}

	public function nonReturningSourcePath(index:Int):String {
		if (index < 0 || index > 9999) {
			throw new ProjectEmissionError('split project cannot address non-returning source ordinal `$index` in module `$modulePath`');
		}
		return 'src/modules/$relativeStem.nonreturn_${StringTools.lpad(Std.string(index), "0", 4)}.c';
	}
}

/** Deterministic package artifact shared by every module in one Haxe package. */
class CProjectPackageLayout {
	public final packagePath:String;
	public final relativeStem:String;
	public final headerPath:String;
	public final headerInclude:String;
	public final sourcePath:String;
	public final modules:Array<CProjectModuleLayout>;

	public function new(packagePath:String, relativeStem:String, modules:Array<CProjectModuleLayout>) {
		if (modules.length == 0)
			throw new ProjectEmissionError('generated package `$packagePath` requires at least one Haxe module');
		this.packagePath = packagePath;
		this.relativeStem = relativeStem;
		this.modules = modules.copy();
		final directory = relativeStem == "" ? "" : '$relativeStem/';
		headerPath = 'include/hxc/packages/${directory}package.h';
		headerInclude = 'hxc/packages/${directory}package.h';
		sourcePath = 'src/packages/${directory}package.c';
	}

	public function nonReturningSourcePath(index:Int):String {
		if (index < 0 || index > 9999) {
			throw new ProjectEmissionError('package project cannot address non-returning source ordinal `$index` in package `$packagePath`');
		}
		final directory = relativeStem == "" ? "" : '$relativeStem/';
		return 'src/packages/${directory}package.nonreturn_${StringTools.lpad(Std.string(index), "0", 4)}.c';
	}
}

/**
	Pure, output-root-independent file assignment used before any C is printed.

	The umbrella path remains stable across layouts so native consumers and
	build tooling have one private include. Split mode adds a common detail
	header for program-wide representation declarations and one module header
	per normalized Haxe module; package mode reuses the detail header and assigns
	one header/source pair to every normalized Haxe package.
**/
class CProjectLayoutPlan {
	public static inline final UMBRELLA_HEADER_PATH = "include/hxc/program.h";
	public static inline final UMBRELLA_HEADER_INCLUDE = "hxc/program.h";
	public static inline final UNITY_SOURCE_PATH = "src/program.c";
	public static inline final TYPES_HEADER_PATH = "include/hxc/detail/program_types.h";
	public static inline final TYPES_HEADER_INCLUDE = "hxc/detail/program_types.h";
	public static inline final ENTRY_SOURCE_PATH = "src/hxc/main.c";
	public static inline final SUPPORT_SOURCE_PATH = "src/hxc/support.c";

	public final layout:CProjectLayout;
	public final modules:Array<CProjectModuleLayout>;
	public final packages:Array<CProjectPackageLayout>;
	public final headerPaths:Array<String>;

	final modulesByPath:Map<String, CProjectModuleLayout> = [];
	final packagesByPath:Map<String, CProjectPackageLayout> = [];
	final packagesByModulePath:Map<String, CProjectPackageLayout> = [];
	final headerGuardsByPath:Map<String, String>;

	public function new(layout:CProjectLayout, modules:Array<CProjectModuleLayout>, packages:Array<CProjectPackageLayout>) {
		this.layout = layout;
		this.modules = modules.copy();
		this.packages = packages.copy();
		for (module in modules) {
			if (modulesByPath.exists(module.modulePath)) {
				throw new ProjectEmissionError('project layout repeats Haxe module `${module.modulePath}`');
			}
			modulesByPath.set(module.modulePath, module);
		}
		for (pack in packages) {
			if (packagesByPath.exists(pack.packagePath))
				throw new ProjectEmissionError('project layout repeats Haxe package `${pack.packagePath}`');
			packagesByPath.set(pack.packagePath, pack);
			for (module in pack.modules) {
				if (!modulesByPath.exists(module.modulePath))
					throw new ProjectEmissionError('project layout package `${pack.packagePath}` references unknown Haxe module `${module.modulePath}`');
				if (packagesByModulePath.exists(module.modulePath))
					throw new ProjectEmissionError('project layout assigns Haxe module `${module.modulePath}` to more than one package');
				if (module.packagePath != pack.packagePath) {
					throw new ProjectEmissionError('project layout assigns Haxe module `${module.modulePath}` to package `${pack.packagePath}` instead of `${module.packagePath}`');
				}
				packagesByModulePath.set(module.modulePath, pack);
			}
		}
		for (module in modules) {
			if (!packagesByModulePath.exists(module.modulePath))
				throw new ProjectEmissionError('project layout omits package ownership for Haxe module `${module.modulePath}`');
		}
		headerPaths = switch layout {
			case Unity: [UMBRELLA_HEADER_PATH];
			case Split:
				final paths = [TYPES_HEADER_PATH, UMBRELLA_HEADER_PATH];
				for (module in modules)
					paths.push(module.headerPath);
				paths.sort(compareUtf8);
				paths;
			case Package:
				final paths = [TYPES_HEADER_PATH, UMBRELLA_HEADER_PATH];
				for (pack in packages)
					paths.push(pack.headerPath);
				paths.sort(compareUtf8);
				paths;
		};
		headerGuardsByPath = CDeclarationPlanner.headerGuardsFor(headerPaths);
	}

	public function module(modulePath:String):CProjectModuleLayout {
		final result = modulesByPath.get(modulePath);
		if (result == null)
			throw new ProjectEmissionError('project layout cannot resolve Haxe module `$modulePath`');
		return result;
	}

	public function packageLayout(packagePath:String):CProjectPackageLayout {
		final result = packagesByPath.get(packagePath);
		if (result == null)
			throw new ProjectEmissionError('project layout cannot resolve Haxe package `$packagePath`');
		return result;
	}

	public function packageForModule(modulePath:String):CProjectPackageLayout {
		final result = packagesByModulePath.get(modulePath);
		if (result == null)
			throw new ProjectEmissionError('project layout cannot resolve package ownership for Haxe module `$modulePath`');
		return result;
	}

	public function guardRequest(path:String):CSymbolRequest {
		if (headerPaths.indexOf(path) == -1)
			throw new ProjectEmissionError('project layout does not own header `$path`');
		final guard = headerGuardsByPath.get(path);
		if (guard == null)
			throw new ProjectEmissionError('project layout cannot resolve a guard for header `$path`');
		if (path == UMBRELLA_HEADER_PATH) {
			// This request identity predates selectable layouts. Keep it stable so
			// adding split-only headers does not churn existing symbol manifests.
			return new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal, guard);
		}
		return new CSymbolRequest(CSKModule, ["compiler", "generated-header", guard, "guard"], CNSPreprocessor, CSVInternal, guard);
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final common = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...common) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}

/** Builds source-shaped paths without consulting the filesystem or printer. */
class CProjectLayoutPlanner {
	/** Leaves room for `.nonreturn_0000.c` below common 255-byte limits. */
	public static inline final MAX_COMPONENT_BYTES = 200;

	static inline final ENCODED_PREFIX_HEX_BYTES = 32;

	public function new() {}

	public function plan(layout:CProjectLayout, modulePaths:Array<String>):CProjectLayoutPlan {
		final unique:Map<String, Bool> = [];
		final ordered = modulePaths.copy();
		ordered.sort(compareUtf8);
		final modules:Array<CProjectModuleLayout> = [];
		final caseFoldedPaths:Map<String, String> = [];
		for (modulePath in ordered) {
			if (unique.exists(modulePath))
				continue;
			unique.set(modulePath, true);
			final packagePath = packagePathForModule(modulePath);
			final module = new CProjectModuleLayout(modulePath, packagePath, moduleStem(modulePath));
			final folded = module.headerPath.toLowerCase();
			final previous = caseFoldedPaths.get(folded);
			if (previous != null) {
				throw new ProjectEmissionError('Haxe modules `$previous` and `$modulePath` map to the same case-insensitive generated path `${module.headerPath}`');
			}
			caseFoldedPaths.set(folded, modulePath);
			modules.push(module);
		}
		if (modules.length == 0)
			throw new ProjectEmissionError("project layout requires at least one normalized Haxe module");
		final packageModules:Map<String, Array<CProjectModuleLayout>> = [];
		final packagePaths:Array<String> = [];
		for (module in modules) {
			var grouped = packageModules.get(module.packagePath);
			if (grouped == null) {
				grouped = [];
				packageModules.set(module.packagePath, grouped);
				packagePaths.push(module.packagePath);
			}
			grouped.push(module);
		}
		packagePaths.sort(compareUtf8);
		final packages:Array<CProjectPackageLayout> = [];
		final caseFoldedPackagePaths:Map<String, String> = [];
		for (packagePath in packagePaths) {
			final grouped = packageModules.get(packagePath);
			if (grouped == null)
				throw new ProjectEmissionError('project layout lost modules for Haxe package `$packagePath`');
			final pack = new CProjectPackageLayout(packagePath, packageStem(packagePath), grouped);
			// Package paths become artifact paths only in package mode. Split uses
			// complete module paths, while unity has no package-shaped files, so a
			// package-only collision must not change those established layouts.
			if (layout == CProjectLayout.Package) {
				final folded = pack.headerPath.toLowerCase();
				final previous = caseFoldedPackagePaths.get(folded);
				if (previous != null) {
					throw new ProjectEmissionError('Haxe packages `$previous` and `$packagePath` map to the same case-insensitive generated path `${pack.headerPath}`');
				}
				caseFoldedPackagePaths.set(folded, packagePath);
			}
			packages.push(pack);
		}
		return new CProjectLayoutPlan(layout, modules, packages);
	}

	static function packagePathForModule(modulePath:String):String {
		final split = modulePath.lastIndexOf(".");
		return split == -1 ? "" : modulePath.substr(0, split);
	}

	static function moduleStem(modulePath:String):String {
		if (modulePath == "" || modulePath.indexOf("/") != -1 || modulePath.indexOf("\\") != -1) {
			throw new ProjectEmissionError('invalid normalized Haxe module path `$modulePath`');
		}
		final output:Array<String> = [];
		for (component in modulePath.split(".")) {
			if (component == "" || component == "." || component == "..")
				throw new ProjectEmissionError('invalid normalized Haxe module path `$modulePath`');
			output.push(safeComponent(component));
		}
		return output.join("/");
	}

	static function packageStem(packagePath:String):String {
		if (packagePath == "")
			return "";
		if (packagePath.indexOf("/") != -1 || packagePath.indexOf("\\") != -1)
			throw new ProjectEmissionError('invalid normalized Haxe package path `$packagePath`');
		final output:Array<String> = [];
		for (component in packagePath.split(".")) {
			if (component == "" || component == "." || component == "..")
				throw new ProjectEmissionError('invalid normalized Haxe package path `$packagePath`');
			output.push(safeComponent(component));
		}
		return output.join("/");
	}

	static function safeComponent(component:String):String {
		final bytes = Bytes.ofString(component);
		if (isPortableComponent(component) && bytes.length <= MAX_COMPONENT_BYTES)
			return component;
		final encoded = new StringBuf();
		encoded.add("haxe_utf8_");
		for (index in 0...bytes.length)
			encoded.add(StringTools.hex(bytes.get(index), 2).toLowerCase());
		final complete = encoded.toString();
		if (Bytes.ofString(complete).length <= MAX_COMPONENT_BYTES)
			return complete;
		final prefixLength = ENCODED_PREFIX_HEX_BYTES * 2;
		return complete.substr(0, "haxe_utf8_".length + prefixLength) + "_sha256_" + Sha256.encode(component);
	}

	static function isPortableComponent(value:String):Bool {
		if (value.length == 0)
			return false;
		final folded = value.toLowerCase();
		// Keep the byte-encoding namespace unambiguous, and avoid names that the
		// Windows filesystem reserves even when they carry a normal extension.
		if (StringTools.startsWith(folded, "haxe_utf8_") || isWindowsDeviceName(folded))
			return false;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (!((code >= "a".code && code <= "z".code)
				|| (code >= "A".code && code <= "Z".code)
				|| (code >= "0".code && code <= "9".code)
				|| code == "_".code))
				return false;
		}
		return true;
	}

	static function isWindowsDeviceName(folded:String):Bool {
		if (folded == "con" || folded == "prn" || folded == "aux" || folded == "nul")
			return true;
		if (folded.length == 4) {
			final prefix = folded.substr(0, 3);
			final digit = folded.charCodeAt(3);
			return (prefix == "com" || prefix == "lpt") && digit >= "1".code && digit <= "9".code;
		}
		return false;
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final common = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...common) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}
#else
enum abstract CProjectLayout(String) to String {
	var Split = "split";
	var Package = "package";
	var Unity = "unity";
}
#end
