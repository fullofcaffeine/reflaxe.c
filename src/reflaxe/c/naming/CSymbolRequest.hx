package reflaxe.c.naming;

import haxe.io.Bytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** The semantic role of one Haxe or compiler-owned name in generated C. */
enum CSymbolKind {
	CSKPackage;
	CSKModule;
	CSKType;
	CSKField;
	CSKMethod;
	CSKLocal;
	CSKTemporary;
	CSKSpecialization;
	CSKClosure;
	CSKClosureEnvironment;
	CSKVTable;
	CSKInterfaceTable;
	CSKTypeDescriptor;
	CSKReflectionEntry;
	CSKStaticInitializer;
	CSKExport;
	CSKRuntimePrivate;
}

/**
	A real C identifier namespace plus its stable semantic scope.

	The ordinary namespace contains functions, objects, typedefs, and enum
	constants. Tags are separate, members are separate per aggregate, and labels
	are separate per function. Preprocessor names are tracked independently
	because they can replace tokens before the C namespace rules apply.
 */
enum CSymbolNamespace {
	CNSOrdinary(scopeKey:String);
	CNSTag(scopeKey:String);
	CNSMember(ownerTypeKey:String);
	CNSLabel(ownerFunctionKey:String);
	CNSPreprocessor;
}

/** Whether a generated default is compiler-private or C-user-visible. */
enum CSymbolVisibility {
	CSVInternal;
	CSVPublic;
	CSVExternal;
}

/**
	One path-independent symbol request.

	Qualified semantic coordinates, normalized overload signatures, normalized
	type arguments, and source-stable ordinals are the only identity inputs.
	Host paths, discovery counters, object addresses, and map iteration never
	enter the key.
 */
class CSymbolRequest {
	public final kind:CSymbolKind;
	public final qualifiedName:Array<String>;
	public final namespace:CSymbolNamespace;
	public final visibility:CSymbolVisibility;
	public final explicitName:Null<String>;
	public final overloadSignature:Array<String>;
	public final specializationArguments:Array<String>;
	public final sourceOrdinal:Null<Int>;

	public function new(kind:CSymbolKind, qualifiedName:Array<String>, namespace:CSymbolNamespace, ?visibility:CSymbolVisibility, ?explicitName:String,
			?overloadSignature:Array<String>, ?specializationArguments:Array<String>, ?sourceOrdinal:Int) {
		this.kind = kind;
		this.qualifiedName = qualifiedName.copy();
		this.namespace = namespace;
		this.visibility = visibility == null ? CSVInternal : visibility;
		this.explicitName = explicitName;
		this.overloadSignature = overloadSignature == null ? [] : overloadSignature.copy();
		this.specializationArguments = specializationArguments == null ? [] : specializationArguments.copy();
		this.sourceOrdinal = sourceOrdinal;
		validate();
	}

	public function stableKey():String {
		return [
			canonicalPart(kindName(kind)),
			canonicalPart(namespaceKey(namespace)),
			canonicalArray(qualifiedName),
			canonicalArray(overloadSignature),
			canonicalArray(specializationArguments),
			canonicalPart(sourceOrdinal == null ? "" : Std.string(sourceOrdinal))
		].join("|");
	}

	public function namingFingerprint():String {
		return stableKey()
			+ "|"
			+ canonicalPart(visibilityName(visibility))
			+ "|"
			+ canonicalPart(explicitName == null ? "" : explicitName);
	}

	public function sourceSymbol():String {
		var display = qualifiedName.join(".");
		if (overloadSignature.length > 0) {
			display += "(" + overloadSignature.join(", ") + ")";
		}
		if (specializationArguments.length > 0) {
			display += "<" + specializationArguments.join(", ") + ">";
		}
		if (sourceOrdinal != null) {
			display += "#" + sourceOrdinal;
		}
		return display;
	}

	public static function kindName(kind:CSymbolKind):String {
		return switch kind {
			case CSKPackage: "package";
			case CSKModule: "module";
			case CSKType: "type";
			case CSKField: "field";
			case CSKMethod: "method";
			case CSKLocal: "local";
			case CSKTemporary: "temporary";
			case CSKSpecialization: "specialization";
			case CSKClosure: "closure";
			case CSKClosureEnvironment: "closure-environment";
			case CSKVTable: "vtable";
			case CSKInterfaceTable: "interface-table";
			case CSKTypeDescriptor: "type-descriptor";
			case CSKReflectionEntry: "reflection-entry";
			case CSKStaticInitializer: "static-initializer";
			case CSKExport: "export";
			case CSKRuntimePrivate: "runtime-private";
		};
	}

	public static function namespaceKey(namespace:CSymbolNamespace):String {
		return switch namespace {
			case CNSOrdinary(scopeKey): "ordinary:" + canonicalPart(scopeKey);
			case CNSTag(scopeKey): "tag:" + canonicalPart(scopeKey);
			case CNSMember(ownerTypeKey): "member:" + canonicalPart(ownerTypeKey);
			case CNSLabel(ownerFunctionKey): "label:" + canonicalPart(ownerFunctionKey);
			case CNSPreprocessor: "preprocessor";
		};
	}

	public static function namespaceKind(namespace:CSymbolNamespace):String {
		return switch namespace {
			case CNSOrdinary(_): "ordinary";
			case CNSTag(_): "tag";
			case CNSMember(_): "member";
			case CNSLabel(_): "label";
			case CNSPreprocessor: "preprocessor";
		};
	}

	public static function namespaceScope(namespace:CSymbolNamespace):String {
		return switch namespace {
			case CNSOrdinary(scopeKey) | CNSTag(scopeKey): scopeKey;
			case CNSMember(ownerTypeKey): ownerTypeKey;
			case CNSLabel(ownerFunctionKey): ownerFunctionKey;
			case CNSPreprocessor: "translation-unit";
		};
	}

	public static function visibilityName(visibility:CSymbolVisibility):String {
		return switch visibility {
			case CSVInternal: "internal";
			case CSVPublic: "public";
			case CSVExternal: "external";
		};
	}

	static function canonicalArray(values:Array<String>):String {
		return values.length + ":" + values.map(canonicalPart).join("");
	}

	static function canonicalPart(value:String):String {
		return Bytes.ofString(value).length + ":" + value;
	}

	function validate():Void {
		if (qualifiedName.length == 0) {
			internalFailure("symbol request has no qualified semantic name");
		}
		validateParts(qualifiedName, "qualified-name");
		validateParts(overloadSignature, "overload-signature");
		validateParts(specializationArguments, "specialization-argument");
		validateScope(namespaceScope(namespace));
		if (sourceOrdinal != null && sourceOrdinal < 0) {
			internalFailure('symbol `${qualifiedName.join(".")}` has a negative source ordinal');
		}
		switch kind {
			case CSKTemporary | CSKClosure | CSKClosureEnvironment:
				if (sourceOrdinal == null) {
					internalFailure('anonymous ${kindName(kind)} `${qualifiedName.join(".")}` requires a source-stable ordinal');
				}
			case CSKSpecialization:
				if (specializationArguments.length == 0) {
					internalFailure('specialization `${qualifiedName.join(".")}` requires normalized type arguments');
				}
			case _:
		}
	}

	static function validateParts(values:Array<String>, label:String):Void {
		for (value in values) {
			if (value == "" || containsControl(value) || containsPathSeparator(value)) {
				internalFailure('$label components must be non-empty path-independent semantic values with no control characters');
			}
		}
	}

	static function validateScope(value:String):Void {
		if (value == "" || containsControl(value) || containsPathSeparator(value)) {
			internalFailure("C symbol namespace scopes must be stable non-empty semantic keys");
		}
	}

	static function containsPathSeparator(value:String):Bool
		return value.indexOf("/") != -1 || value.indexOf("\\") != -1;

	static function containsControl(value:String):Bool {
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return true;
			}
		}
		return false;
	}

	static function internalFailure(detail:String):Dynamic {
		throw new CSymbolRegistryError(CDiagnosticId.InternalCompilerError, detail);
	}
}
