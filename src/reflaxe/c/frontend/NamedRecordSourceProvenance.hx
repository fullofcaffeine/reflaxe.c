package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.io.Path;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.Type.AnonStatus;
import haxe.macro.Type.AnonType;
import haxe.macro.Type.Ref;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import sys.FileSystem;
import sys.io.File;

/**
	Owns exact source positions for fields declared by named anonymous records.

	A Haxe compilation-server request can reuse a typed `typedef` while replacing
	the field positions visible through its anonymous type with positions from an
	object literal. Generated C layout is unchanged, but diagnostics and source
	maps would then point at the use instead of the declaration. This module
	accepts current positions only when every field is visibly inside the
	typedef's own source range. A later warm request may recover the positions
	only from a bounded process cache keyed by the exact source bytes and define
	set; the returned `Position` values are always rebuilt for the current
	request and current source file.
**/
class NamedRecordSourceProvenance {
	static inline final CACHE_SCHEMA = 1;
	static inline final MAX_CACHE_ENTRIES = 2048;

	/**
		Keep only verified plain-data offsets between requests to one Haxe server.

		Macro module statics are normally recreated for each compilation request.
		On Haxe's Eval macro host, `@:persistent` gives these two collections the
		process lifetime needed to repair a later reused typedef. The cache remains
		safe because it stores no `Type`, `Position`, filename, or application
		value, and `remember` removes the oldest entry above the fixed limit.
	**/
	#if (eval && macro)
	@:persistent
	#end
	static var cacheByContent:Map<String, CachedNamedRecordSource> = [];

	#if (eval && macro)
	@:persistent
	#end
	static var cacheInsertionOrder:Array<String> = [];

	/**
		Build the immutable source-position plan used by one compiler request.

		Cold or rebuilt typedefs supply declaration positions directly. Reused
		typedefs must find an exact content-keyed record captured by an earlier
		request in this Haxe process. A missing record fails before HxcIR is built
		instead of silently attaching a plausible but wrong source location.
	**/
	public static function plan(declarations:Array<TypedAstDeclaration>):NamedRecordSourcePlan {
		final records:Map<String, Map<String, Position>> = [];
		final sourcesByPath:Map<String, NamedRecordSourceInput> = [];
		final defineDigest = currentDefineDigest();
		for (declaration in declarations) {
			switch declaration.raw {
				case TTypeDecl(reference):
					final definition = reference.get();
					final anonymous = directAnonymous(definition.type);
					if (anonymous != null && hasClosedFieldSet(anonymous)) {
						records.set(declaration.path, resolveRecord(declaration.path, definition.pos, anonymous, defineDigest, sourcesByPath));
					}
				case TClassDecl(_) | TEnumDecl(_) | TAbstract(_):
			}
		}
		return new NamedRecordSourcePlan(records);
	}

	/**
		Build the same exact plan for one direct semantic-lowering probe.

		Production lowering receives the complete `TypedProgramInput` plan above.
		Focused compiler tests sometimes exercise `CBodyLowering` directly with a
		typed record reference instead. They must preserve the same cold/warm
		source rule rather than falling back to Haxe's possibly mutated field
		position.
	**/
	public static function planRecord(declarationPath:String, definitionPosition:Position, anonymous:Ref<AnonType>):NamedRecordSourcePlan {
		if (!hasClosedFieldSet(anonymous)) {
			throw new NamedRecordSourceProvenanceError('Named record `$declarationPath` does not have a closed field set.', definitionPosition);
		}
		final records:Map<String, Map<String, Position>> = [];
		records.set(declarationPath, resolveRecord(declarationPath, definitionPosition, anonymous, currentDefineDigest(), []));
		return new NamedRecordSourcePlan(records);
	}

	/**
		Choose declaration positions or recover the exact earlier declaration.

		Haxe's current field positions are authoritative only when every field is
		inside the typedef's own source range. Otherwise this is a warm reused
		anonymous type, so recovery requires the same declaration path, source
		bytes, complete define set, typedef range, and field-name set. Any mismatch
		fails before semantic lowering rather than guessing from a use site.
	**/
	static function resolveRecord(declarationPath:String, definitionPosition:Position, anonymous:Ref<AnonType>, defineDigest:String,
			sourcesByPath:Map<String, NamedRecordSourceInput>):Map<String, Position> {
		final definitionInfo = Context.getPosInfos(definitionPosition);
		final fields = anonymous.get().fields.copy();
		fields.sort((left, right) -> compareStrings(left.name, right.name));
		final authoritative = hasOnlyDeclarationFields(fields, definitionInfo);
		if (authoritative) {
			final result = currentPositions(fields);
			final source = readSource(definitionInfo.file, sourcesByPath);
			if (source != null) {
				validateOffsets(declarationPath, definitionInfo.min, definitionInfo.max, fields, source.bytes.length, definitionPosition);
				final key = cacheKey(declarationPath, source.digest, defineDigest);
				remember(key, {
					typeMin: definitionInfo.min,
					typeMax: definitionInfo.max,
					fields: fields.map(field -> {
						final info = Context.getPosInfos(field.pos);
						{name: field.name, min: info.min, max: info.max};
					})
				});
			}
			return result;
		}

		final source = readSource(definitionInfo.file, sourcesByPath);
		if (source == null) {
			throw new NamedRecordSourceProvenanceError('Cannot recover declaration positions for named record `$declarationPath`: Haxe reused the type with use-site positions, '
				+ 'and `${definitionInfo.file}` is not a readable source file. Start a fresh Haxe compiler process for this build.',
				definitionPosition);
		}
		final key = cacheKey(declarationPath, source.digest, defineDigest);
		final cached = cacheByContent.get(key);
		if (cached == null) {
			throw new NamedRecordSourceProvenanceError('Cannot recover declaration positions for named record `$declarationPath`: this Haxe compiler process has no earlier '
				+ 'declaration-position record for the exact current source bytes and defines. Start a fresh Haxe compiler process once; '
				+ 'later unchanged requests may reuse that verified record.',
				definitionPosition);
		}
		if (cached.typeMin != definitionInfo.min || cached.typeMax != definitionInfo.max) {
			throw new NamedRecordSourceProvenanceError('Cached declaration positions for named record `$declarationPath` do not match its current typedef range. '
				+ 'The compiler refuses to guess; start a fresh Haxe compiler process.',
				definitionPosition);
		}
		validateCachedFields(declarationPath, fields, cached.fields, source.bytes.length, definitionPosition);
		final restored:Map<String, Position> = [];
		for (field in cached.fields) {
			restored.set(field.name, Context.makePosition({
				file: definitionInfo.file,
				min: field.min,
				max: field.max
			}));
		}
		return restored;
	}

	static function directAnonymous(type:Type):Null<Ref<AnonType>> {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : directAnonymous(resolved);
			case TLazy(resolve): directAnonymous(resolve());
			case TAnonymous(reference): reference;
			case TEnum(_, _) | TInst(_, _) | TType(_, _) | TFun(_, _) | TDynamic(_) | TAbstract(_, _): null;
		};
	}

	static function hasClosedFieldSet(reference:Ref<AnonType>):Bool {
		return switch reference.get().status {
			case AClosed | AConst: true;
			case AOpened | AExtend(_) | AClassStatics(_) | AEnumStatics(_) | AAbstractStatics(_): false;
		};
	}

	static function isDeclarationField(field:{file:String, min:Int, max:Int}, definition:{file:String, min:Int, max:Int}):Bool {
		return normalizedPath(field.file) == normalizedPath(definition.file)
			&& field.min >= definition.min
			&& field.max >= field.min
			&& field.max <= definition.max;
	}

	static function hasOnlyDeclarationFields(fields:Array<haxe.macro.Type.ClassField>, definition:{file:String, min:Int, max:Int}):Bool {
		for (field in fields) {
			if (!isDeclarationField(Context.getPosInfos(field.pos), definition)) {
				return false;
			}
		}
		return true;
	}

	static function currentPositions(fields:Array<haxe.macro.Type.ClassField>):Map<String, Position> {
		final result:Map<String, Position> = [];
		for (field in fields) {
			result.set(field.name, field.pos);
		}
		return result;
	}

	/**
		Read and hash each current-request source file at most once.

		A module can declare several record typedefs. Sharing one request-local
		snapshot avoids repeated file I/O and guarantees that all those records
		use the same bytes even if an editor saves the file during compilation.
		The map itself ends with this compiler request.
	**/
	static function readSource(rawPath:String, sourcesByPath:Map<String, NamedRecordSourceInput>):Null<NamedRecordSourceInput> {
		var path = Path.normalize(rawPath);
		if (!Path.isAbsolute(path)) {
			path = Path.join([Sys.getCwd(), path]);
		}
		final existing = sourcesByPath.get(path);
		if (existing != null) {
			return existing;
		}
		if (!FileSystem.exists(path) || FileSystem.isDirectory(path)) {
			return null;
		}
		final bytes = File.getBytes(path);
		final source:NamedRecordSourceInput = {bytes: bytes, digest: Sha256.make(bytes).toHex()};
		sourcesByPath.set(path, source);
		return source;
	}

	static function currentDefineDigest():String {
		final entries:Array<String> = [];
		for (name => value in Context.getDefines()) {
			entries.push('${name.length}:$name:${value.length}:$value');
		}
		entries.sort(compareStrings);
		// Defines can alter macro typing and may contain checkout-local paths.
		// Hashing the canonical complete set makes them part of invalidation
		// without retaining or reporting those path-bearing values.
		return Sha256.encode(entries.join("\n"));
	}

	static function cacheKey(declarationPath:String, sourceDigest:String, defineDigest:String):String {
		return '$CACHE_SCHEMA\u0000$declarationPath\u0000$sourceDigest\u0000$defineDigest';
	}

	static function remember(key:String, value:CachedNamedRecordSource):Void {
		if (!cacheByContent.exists(key)) {
			cacheInsertionOrder.push(key);
		}
		cacheByContent.set(key, copyCached(value));
		while (cacheInsertionOrder.length > MAX_CACHE_ENTRIES) {
			final oldest = cacheInsertionOrder.shift();
			if (oldest != null) {
				cacheByContent.remove(oldest);
			}
		}
	}

	static function copyCached(value:CachedNamedRecordSource):CachedNamedRecordSource {
		return {
			typeMin: value.typeMin,
			typeMax: value.typeMax,
			fields: value.fields.map(field -> {name: field.name, min: field.min, max: field.max})
		};
	}

	static function validateOffsets(declarationPath:String, typeMin:Int, typeMax:Int, fields:Array<haxe.macro.Type.ClassField>, sourceLength:Int,
			position:Position):Void {
		if (!validRange(typeMin, typeMax, sourceLength)) {
			throw new NamedRecordSourceProvenanceError('Named record `$declarationPath` has a typedef range outside its current source bytes.', position);
		}
		for (field in fields) {
			final info = Context.getPosInfos(field.pos);
			if (!validRange(info.min, info.max, sourceLength)) {
				throw new NamedRecordSourceProvenanceError('Named record `$declarationPath` field `${field.name}` has a declaration range outside its current source bytes.',
					position);
			}
		}
	}

	static function validateCachedFields(declarationPath:String, current:Array<haxe.macro.Type.ClassField>, cached:Array<CachedNamedRecordFieldSource>,
			sourceLength:Int, position:Position):Void {
		if (current.length != cached.length) {
			throw new NamedRecordSourceProvenanceError('Cached declaration positions for named record `$declarationPath` have ${cached.length} fields, '
				+ 'but the current typed record has ${current.length}.',
				position);
		}
		for (index in 0...current.length) {
			final expected = cached[index];
			if (current[index].name != expected.name || !validRange(expected.min, expected.max, sourceLength)) {
				throw new NamedRecordSourceProvenanceError('Cached declaration positions for named record `$declarationPath` do not match current field `${current[index].name}`.',
					position);
			}
		}
	}

	static inline function validRange(min:Int, max:Int, sourceLength:Int):Bool {
		return min >= 0 && max >= min && max <= sourceLength;
	}

	static function normalizedPath(value:String):String {
		return StringTools.replace(Path.normalize(value), "\\", "/");
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : left > right ? 1 : 0;
	}
}

/**
	Provides request-local declaration positions to semantic record lowering.

	The plan contains Haxe `Position` objects only for the active request. Its
	process cache stores plain names and byte offsets instead, so no compiler
	object or checkout-specific filename survives into a later request.
**/
class NamedRecordSourcePlan {
	final records:Map<String, Map<String, Position>>;

	/** Create one immutable-by-convention lookup from already validated fields. */
	public function new(records:Map<String, Map<String, Position>>) {
		this.records = records;
	}

	/**
		Return the exact declaration position for one field of a named record.

		A missing record is an internal ownership failure: named-record lowering
		must not fall back to whichever use-site position Haxe happened to expose.
	**/
	public function requireFieldPosition(declarationPath:String, fieldName:String, fallback:Position):Position {
		final fields = records.get(declarationPath);
		if (fields == null) {
			throw new NamedRecordSourceProvenanceError('Named record `$declarationPath` reached lowering without a validated declaration-position plan.',
				fallback);
		}
		final position = fields.get(fieldName);
		if (position == null) {
			throw new NamedRecordSourceProvenanceError('Named record `$declarationPath` field `$fieldName` is absent from its declaration-position plan.',
				fallback);
		}
		return position;
	}
}

/**
	Reports that exact record declaration positions cannot be proven.

	The position identifies the affected typedef. `CReflaxeCompiler` turns this
	into the typed internal-compiler diagnostic before semantic lowering or
	output, so a warm build cannot look successful while carrying stale
	diagnostic coordinates.
**/
class NamedRecordSourceProvenanceError extends haxe.Exception {
	public final detail:String;
	public final position:Position;

	/** Preserve the actionable explanation and source anchor for the adapter. */
	public function new(detail:String, position:Position) {
		this.detail = detail;
		this.position = position;
		super(detail);
	}
}

private typedef CachedNamedRecordFieldSource = {
	final name:String;
	final min:Int;
	final max:Int;
}

private typedef CachedNamedRecordSource = {
	final typeMin:Int;
	final typeMax:Int;
	final fields:Array<CachedNamedRecordFieldSource>;
}

private typedef NamedRecordSourceInput = {
	final bytes:Bytes;
	final digest:String;
}
#else
class NamedRecordSourceProvenance {}
class NamedRecordSourcePlan {}
class NamedRecordSourceProvenanceError {}
#end
