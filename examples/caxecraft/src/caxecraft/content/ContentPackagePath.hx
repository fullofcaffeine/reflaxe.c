package caxecraft.content;

import haxe.io.Bytes;

/**
	Defines the only path spelling that Caxecraft content packages may accept.

	A package path is a UTF-8, forward-slash-separated name relative to a
	separately selected content root. Parsing rejects host-specific shortcuts
	before any filesystem adapter sees the value, so Eval and native builds agree
	on which names are safe to request.
**/
/** Exact reason a raw path could not become a package-relative path. */
enum ContentPackagePathError {
	EmptyPath;
	AbsolutePath;
	DriveOrDevicePath;
	AlternateSeparator;
	ContainsNul;
	EmptyComponent(index:Int);
	DotComponent(index:Int);
	ParentComponent(index:Int);
	ReservedDeviceComponent(index:Int);
	TrailingDotOrSpace(index:Int);
	PathTooLong(maximumBytes:Int);
	ComponentTooLong(index:Int, maximumBytes:Int);
	TooManyComponents(maximumComponents:Int);
}

/** A validated package path or the first deterministic spelling error. */
enum ContentPackagePathResult {
	PathAccepted(path:ContentPackagePath);
	PathRejected(error:ContentPackagePathError);
}

/**
	One normalized logical path below an application-selected content root.

	The constructor is private because a plain `String` cannot prove confinement.
	`parse` validates the complete value and stores its already-separated
	components. The object owns that Array and exposes indexed reads only, so a
	caller cannot mutate the validation evidence after construction.
**/
final class ContentPackagePath {
	/** Maximum UTF-8 byte length accepted for one logical package path. */
	public static inline final MAXIMUM_PATH_BYTES:Int = 4096;

	/** Maximum UTF-8 byte length accepted for one path component. */
	public static inline final MAXIMUM_COMPONENT_BYTES:Int = 255;

	/** Maximum component count accepted before filesystem traversal begins. */
	public static inline final MAXIMUM_COMPONENTS:Int = 128;

	final value:String;
	final components:Array<String>;

	private function new(value:String, components:Array<String>) {
		this.value = value;
		this.components = components;
	}

	/** Return the normalized forward-slash spelling used in diagnostics. */
	public inline function text():String
		return value;

	/** Return the immutable-by-API number of validated components. */
	public inline function componentCount():Int
		return components.length;

	/** Return one previously validated component by its checked caller index. */
	public inline function component(index:Int):String
		return components[index];

	/**
		Compare portable path identity without allocating or consulting host locale.

		ZIP archives and manifests both reject names that differ only by ASCII case,
		because those names collide on common filesystems even when the current host
		would keep them separate.
	**/
	public function asciiCaseEquals(other:ContentPackagePath):Bool {
		if (value.length != other.value.length)
			return false;
		for (index in 0...value.length) {
			final maybeLeft = value.charCodeAt(index);
			final maybeRight = other.value.charCodeAt(index);
			if (maybeLeft == null || maybeRight == null)
				return false;
			final left:Int = maybeLeft;
			final right:Int = maybeRight;
			final foldedLeft = left >= 65 && left <= 90 ? left + 32 : left;
			final foldedRight = right >= 65 && right <= 90 ? right + 32 : right;
			if (foldedLeft != foldedRight)
				return false;
		}
		return true;
	}

	/**
		Validate one untrusted logical path without touching the filesystem.

		Both POSIX and Windows escape spellings are rejected on every host. That
		deliberately makes packages portable: a name accepted on macOS cannot turn
		into a drive, device, or alternate-separator path on Windows later.
	**/
	public static function parse(value:String):ContentPackagePathResult {
		if (value.length == 0)
			return PathRejected(EmptyPath);
		if (value.charCodeAt(0) == 47)
			return PathRejected(AbsolutePath);
		if (value.indexOf("\\") >= 0)
			return PathRejected(AlternateSeparator);
		if (value.indexOf("\x00") >= 0)
			return PathRejected(ContainsNul);
		if (looksLikeDriveOrDevice(value))
			return PathRejected(DriveOrDevicePath);

		final pathBytes = Bytes.ofString(value).length;
		if (pathBytes > MAXIMUM_PATH_BYTES)
			return PathRejected(PathTooLong(MAXIMUM_PATH_BYTES));

		final parts = value.split("/");
		if (parts.length > MAXIMUM_COMPONENTS)
			return PathRejected(TooManyComponents(MAXIMUM_COMPONENTS));
		for (index in 0...parts.length) {
			final part = parts[index];
			if (part.length == 0)
				return PathRejected(EmptyComponent(index));
			if (part == ".")
				return PathRejected(DotComponent(index));
			if (part == "..")
				return PathRejected(ParentComponent(index));
			if (endsWithDotOrSpace(part))
				return PathRejected(TrailingDotOrSpace(index));
			if (isReservedWindowsDevice(part))
				return PathRejected(ReservedDeviceComponent(index));
			if (Bytes.ofString(part).length > MAXIMUM_COMPONENT_BYTES)
				return PathRejected(ComponentTooLong(index, MAXIMUM_COMPONENT_BYTES));
		}
		return PathAccepted(new ContentPackagePath(value, parts));
	}

	/** Recognize drive prefixes and URI/device-like colon prefixes. */
	static function looksLikeDriveOrDevice(value:String):Bool {
		if (value.indexOf(":") >= 0)
			return true;
		return value.length >= 2 && value.charCodeAt(0) == 47 && value.charCodeAt(1) == 47;
	}

	/** Windows removes a trailing dot or space while resolving a component. */
	static function endsWithDotOrSpace(value:String):Bool {
		final finalCode = value.charCodeAt(value.length - 1);
		return finalCode == 46 || finalCode == 32;
	}

	/** Reject DOS device names even when a later extension is present. */
	static function isReservedWindowsDevice(value:String):Bool {
		final dot = value.indexOf(".");
		final stem = asciiUpper(dot < 0 ? value : value.substring(0, dot));
		if (stem == "CON" || stem == "PRN" || stem == "AUX" || stem == "NUL")
			return true;
		if (stem.length != 4)
			return false;
		final prefix = stem.substring(0, 3);
		final digit = stem.charCodeAt(3);
		return (prefix == "COM" || prefix == "LPT") && digit >= 49 && digit <= 57;
	}

	/** Fold ASCII letters without locale-sensitive or target-specific behavior. */
	static function asciiUpper(value:String):String {
		var result = "";
		for (index in 0...value.length) {
			final maybeCode = value.charCodeAt(index);
			if (maybeCode == null)
				return result;
			final code:Int = maybeCode;
			result += code >= 97 && code <= 122 ? String.fromCharCode(code - 32) : String.fromCharCode(code);
		}
		return result;
	}
}
