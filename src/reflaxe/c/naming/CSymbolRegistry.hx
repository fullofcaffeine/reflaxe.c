package reflaxe.c.naming;

import haxe.Json;
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.naming.CSymbolRequest.CSymbolKind;
import reflaxe.c.naming.CSymbolRequest.CSymbolNamespace;
import reflaxe.c.naming.CSymbolRequest.CSymbolVisibility;

typedef CSymbolNamespaceRecord = {
	final kind:String;
	final scope:String;
}

typedef CSymbolRecord = {
	final semanticKey:String;
	final sourceSymbol:String;
	final kind:String;
	final namespace:CSymbolNamespaceRecord;
	final visibility:String;
	final naming:String;
	final requestedName:Null<String>;
	final readableName:Array<String>;
	final baseName:String;
	final cName:String;
	final collisionResolved:Bool;
	final escapeReasons:Array<String>;
	final overloadSignature:Array<String>;
	final specializationArguments:Array<String>;
	final sourceOrdinal:Null<Int>;
}

typedef CSymbolCollisionEntry = {
	final sourceSymbol:String;
	final cName:String;
}

typedef CSymbolCollisionRecord = {
	final namespace:CSymbolNamespaceRecord;
	final baseName:String;
	final symbols:Array<CSymbolCollisionEntry>;
}

typedef CSymbolTableSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final symbols:Array<CSymbolRecord>;
	final collisions:Array<CSymbolCollisionRecord>;
}

private typedef GeneratedDraft = {
	final request:CSymbolRequest;
	final baseName:String;
	final escapeReasons:Array<String>;
}

private typedef GeneratedState = {
	final draft:GeneratedDraft;
	final hash:String;
	var hashLength:Int;
}

private typedef CollisionEvent = {
	final namespace:CSymbolNamespace;
	final baseName:String;
	final requests:Array<CSymbolRequest>;
}

private typedef AssignedDraft = {
	final request:CSymbolRequest;
	final baseName:String;
	final cName:String;
	final collisionResolved:Bool;
	final escapeReasons:Array<String>;
}

/**
	Per-compilation deterministic registry for every emitted C identifier.

	Registration is deliberately separate from finalization: the complete set is
	sorted and collision groups are resolved as a batch, so discovery order and
	hash-map iteration cannot change output names.
 */
class CSymbolRegistry {
	public static inline final SCHEMA_VERSION = 2;
	public static inline final ALGORITHM = "hxc-c-symbol-v2";
	public static inline final MAX_GENERATED_LENGTH = 120;

	static final C_KEYWORDS = [
		"_Alignas",
		"_Alignof",
		"_Atomic",
		"_Bool",
		"_Complex",
		"_Generic",
		"_Imaginary",
		"_Noreturn",
		"_Static_assert",
		"_Thread_local",
		"auto",
		"break",
		"case",
		"char",
		"const",
		"continue",
		"default",
		"do",
		"double",
		"else",
		"enum",
		"extern",
		"float",
		"for",
		"goto",
		"if",
		"inline",
		"int",
		"long",
		"register",
		"restrict",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"struct",
		"switch",
		"typedef",
		"union",
		"unsigned",
		"void",
		"volatile",
		"while"
	];

	static final LIBRARY_NAMES = [
		"assert",
		"errno",
		"offsetof",
		"setjmp",
		"longjmp",
		"stdin",
		"stdout",
		"stderr",
		"va_arg",
		"va_copy",
		"va_end",
		"va_start"
	];

	final requestsByKey:Map<String, CSymbolRequest> = [];
	final identifiersByKey:Map<String, CIdentifier> = [];
	var finalizedSnapshot:Null<CSymbolTableSnapshot> = null;

	public function new() {}

	public function register(request:CSymbolRequest):Void {
		if (finalizedSnapshot != null) {
			internalFailure("cannot register a C symbol after the registry has been finalized", [request.sourceSymbol()]);
		}
		validateExactName(request);
		final key = request.stableKey();
		final existing = requestsByKey.get(key);
		if (existing != null) {
			if (existing.namingFingerprint() != request.namingFingerprint()) {
				internalFailure('conflicting naming facts were registered for semantic symbol `${request.sourceSymbol()}`',
					[existing.sourceSymbol(), request.sourceSymbol()]);
			}
			return;
		}
		requestsByKey.set(key, request);
	}

	public function registerAll(requests:Array<CSymbolRequest>):Void {
		for (request in requests) {
			register(request);
		}
	}

	public function finalizeSymbols():CSymbolTableSnapshot {
		if (finalizedSnapshot != null) {
			return finalizedSnapshot;
		}

		final requests = [for (request in requestsByKey) request];
		requests.sort(compareRequests);
		validateExactCollisions(requests);

		final assigned:Array<AssignedDraft> = [];
		final generatedStates:Array<GeneratedState> = [];
		final exactByCandidate:Map<String, Array<AssignedDraft>> = [];
		for (request in requests) {
			if (request.explicitName != null) {
				final exact:AssignedDraft = {
					request: request,
					baseName: request.explicitName,
					cName: request.explicitName,
					collisionResolved: false,
					escapeReasons: []
				};
				assigned.push(exact);
				final candidateKey = namespacedCandidate(request.namespace, request.explicitName);
				var exactGroup = exactByCandidate.get(candidateKey);
				if (exactGroup == null) {
					exactGroup = [];
					exactByCandidate.set(candidateKey, exactGroup);
				}
				exactGroup.push(exact);
				continue;
			}
			final generated = generatedBase(request);
			generatedStates.push({
				draft: generated,
				hash: Sha256.encode(request.stableKey()),
				hashLength: 0
			});
		}

		final events:Map<String, CollisionEvent> = [];
		var rounds = 0;
		while (true) {
			final generatedByCandidate:Map<String, Array<GeneratedState>> = [];
			for (state in generatedStates) {
				final key = namespacedCandidate(state.draft.request.namespace, generatedName(state));
				var group = generatedByCandidate.get(key);
				if (group == null) {
					group = [];
					generatedByCandidate.set(key, group);
				}
				group.push(state);
			}

			final candidateKeys = [for (key in generatedByCandidate.keys()) key];
			for (key in exactByCandidate.keys()) {
				addUnique(candidateKeys, key);
			}
			candidateKeys.sort(compareUtf8);
			var foundCollision = false;
			for (key in candidateKeys) {
				final generatedGroup = generatedByCandidate.get(key);
				final exactGroup = exactByCandidate.get(key);
				final generatedCount = generatedGroup == null ? 0 : generatedGroup.length;
				final exactCount = exactGroup == null ? 0 : exactGroup.length;
				if (generatedCount + exactCount < 2) {
					continue;
				}
				if (generatedGroup == null || generatedGroup.length == 0) {
					internalFailure("exact C symbols remained colliding after contract validation",
						exactGroup == null ? [] : exactGroup.map(item -> item.request.sourceSymbol()));
				}
				final colliding:Array<GeneratedState> = generatedGroup == null ? [] : generatedGroup;

				foundCollision = true;
				registerCollisionEvent(events, key, colliding, exactGroup);
				for (state in colliding) {
					state.hashLength = state.hashLength == 0 ? 12 : state.hashLength + 4;
					if (state.hashLength > 64) {
						internalFailure("SHA-256 could not disambiguate distinct C semantic symbol keys", collisionSources(colliding, exactGroup));
					}
				}
			}
			if (!foundCollision) {
				break;
			}
			rounds++;
			if (rounds > generatedStates.length * 15 + 1) {
				internalFailure("C symbol collision resolution did not converge", requests.map(request -> request.sourceSymbol()));
			}
		}

		for (state in generatedStates) {
			assigned.push({
				request: state.draft.request,
				baseName: state.draft.baseName,
				cName: generatedName(state),
				collisionResolved: state.hashLength > 0,
				escapeReasons: state.draft.escapeReasons
			});
		}

		assigned.sort((left, right) -> compareRequests(left.request, right.request));
		validateAssignedUnique(assigned);
		final finalNamesByKey:Map<String, String> = [];
		for (item in assigned) {
			finalNamesByKey.set(item.request.stableKey(), item.cName);
		}
		final collisions = collisionRecords(events, finalNamesByKey);
		final records:Array<CSymbolRecord> = [];
		for (item in assigned) {
			final identifier = new CIdentifier(item.cName);
			identifiersByKey.set(item.request.stableKey(), identifier);
			records.push(record(item));
		}
		collisions.sort(compareCollisions);
		finalizedSnapshot = {
			schemaVersion: SCHEMA_VERSION,
			algorithm: ALGORITHM,
			symbols: records,
			collisions: collisions
		};
		return finalizedSnapshot;
	}

	public function identifierFor(request:CSymbolRequest):CIdentifier {
		finalizeSymbols();
		return identifiersByKey.get(request.stableKey()) ?? internalFailure('C symbol `${request.sourceSymbol()}` was not registered before finalization',
			[request.sourceSymbol()]);
	}

	public function toJson():String
		return Json.stringify(finalizeSymbols(), null, "  ") + "\n";

	static function validateExactName(request:CSymbolRequest):Void {
		final name = request.explicitName;
		if (name == null) {
			return;
		}
		var invalidLexical = name.length == 0 || !isIdentifierStart(name.charCodeAt(0));
		if (!invalidLexical) {
			for (index in 1...name.length) {
				if (!isIdentifierPart(name.charCodeAt(index))) {
					invalidLexical = true;
					break;
				}
			}
		}
		if (invalidLexical) {
			contractFailure('exact C name `$name` on `${request.sourceSymbol()}` is not an ASCII C identifier; @:c.name values are validated, never rewritten',
				[request.sourceSymbol()]);
		}
		if (C_KEYWORDS.indexOf(name) != -1) {
			contractFailure('exact C name `$name` on `${request.sourceSymbol()}` is a C11 keyword; choose a different @:c.name value',
				[request.sourceSymbol()]);
		}
		if (StringTools.startsWith(name, "_") || name.indexOf("__") != -1) {
			contractFailure('exact C name `$name` on `${request.sourceSymbol()}` is implementation-reserved in a C/C++ header context; choose a non-underscore @:c.name value',
				[request.sourceSymbol()]);
		}
		if (StringTools.startsWith(name, "hxc_") || StringTools.startsWith(name, "hxrt_")) {
			contractFailure('exact C name `$name` on `${request.sourceSymbol()}` uses the compiler/runtime-reserved namespace; choose a project-owned @:c.name value',
				[request.sourceSymbol()]);
		}
	}

	static function validateExactCollisions(requests:Array<CSymbolRequest>):Void {
		final groups:Map<String, Array<CSymbolRequest>> = [];
		for (request in requests) {
			if (request.explicitName == null) {
				continue;
			}
			final key = CSymbolRequest.namespaceKey(request.namespace) + "\x00" + request.explicitName;
			var group = groups.get(key);
			if (group == null) {
				group = [];
				groups.set(key, group);
			}
			group.push(request);
		}
		final keys = [for (key in groups.keys()) key];
		keys.sort(compareUtf8);
		for (key in keys) {
			final group = groups.get(key);
			if (group == null || group.length < 2) {
				continue;
			}
			group.sort(compareRequests);
			final sources = group.map(request -> request.sourceSymbol());
			sources.sort(compareUtf8);
			final namespace = namespaceRecord(group[0].namespace);
			contractFailure('exact C name `${group[0].explicitName}` collides in ${namespace.kind} namespace `${namespace.scope}` between '
				+ sources.map(source -> "`" + source + "`").join(" and ")
				+ "; assign distinct @:c.name values",
				sources);
		}
	}

	static function generatedBase(request:CSymbolRequest):GeneratedDraft {
		final reasons:Array<String> = [];
		final readable = request.readableName.length == 0 ? defaultReadableName(request) : request.readableName;
		for (part in readable) {
			collectEscapeReasons(part, reasons);
		}
		final prefix = switch request.visibility {
			case CSVInternal: "hxc_";
			case CSVPublic: "hxc_api_";
			case CSVExternal: "hxc_external_";
		};
		final readableBase = encodeReadableParts(readable);
		if (generatedNameNeedsPrefix(readableBase)) {
			addUnique(reasons, "c-namespace-escape");
		}
		var baseName = prefix + readableBase;
		if (request.sourceOrdinal != null && isAnonymousKind(request.kind)) {
			baseName += "_n" + request.sourceOrdinal;
		}
		if (baseName.length > MAX_GENERATED_LENGTH) {
			addUnique(reasons, "length-limit");
			baseName = appendHash(baseName, Sha256.encode(request.stableKey()), 16);
		}
		reasons.sort(compareUtf8);
		return {request: request, baseName: baseName, escapeReasons: reasons};
	}

	static function generatedName(state:GeneratedState):String
		return state.hashLength == 0 ? state.draft.baseName : appendHash(state.draft.baseName, state.hash, state.hashLength);

	static function namespacedCandidate(namespace:CSymbolNamespace, candidate:String):String
		return CSymbolRequest.namespaceKey(namespace) + "\x00" + candidate;

	static function registerCollisionEvent(events:Map<String, CollisionEvent>, key:String, generated:Array<GeneratedState>,
			exact:Null<Array<AssignedDraft>>):Void {
		final firstRequest = generated[0].draft.request;
		var event = events.get(key);
		if (event == null) {
			event = {
				namespace: firstRequest.namespace,
				baseName: generatedName(generated[0]),
				requests: []
			};
			events.set(key, event);
		}
		for (state in generated) {
			addEventRequest(event.requests, state.draft.request);
		}
		if (exact != null) {
			for (item in exact) {
				addEventRequest(event.requests, item.request);
			}
		}
	}

	static function addEventRequest(requests:Array<CSymbolRequest>, request:CSymbolRequest):Void {
		for (existing in requests) {
			if (existing.stableKey() == request.stableKey()) {
				return;
			}
		}
		requests.push(request);
	}

	static function collisionSources(generated:Array<GeneratedState>, exact:Null<Array<AssignedDraft>>):Array<String> {
		final sources = generated.map(state -> state.draft.request.sourceSymbol());
		if (exact != null) {
			for (item in exact) {
				addUnique(sources, item.request.sourceSymbol());
			}
		}
		sources.sort(compareUtf8);
		return sources;
	}

	static function collisionRecords(events:Map<String, CollisionEvent>, finalNamesByKey:Map<String, String>):Array<CSymbolCollisionRecord> {
		final records:Array<CSymbolCollisionRecord> = [];
		final keys = [for (key in events.keys()) key];
		keys.sort(compareUtf8);
		for (key in keys) {
			final event = events.get(key);
			if (event == null) {
				throw "unreachable missing C symbol collision event";
			}
			event.requests.sort(compareRequests);
			final symbols:Array<CSymbolCollisionEntry> = [];
			for (request in event.requests) {
				final cName:String = finalNamesByKey.get(request.stableKey()) ?? internalFailure('collision ledger lost finalized C symbol `${request.sourceSymbol()}`',
					[request.sourceSymbol()]);
				symbols.push({sourceSymbol: request.sourceSymbol(), cName: cName});
			}
			records.push({
				namespace: namespaceRecord(event.namespace),
				baseName: event.baseName,
				symbols: symbols
			});
		}
		return records;
	}

	static function validateAssignedUnique(assigned:Array<AssignedDraft>):Void {
		final byCandidate:Map<String, AssignedDraft> = [];
		for (item in assigned) {
			final key = namespacedCandidate(item.request.namespace, item.cName);
			final existing = byCandidate.get(key);
			if (existing != null) {
				internalFailure('C symbol collision resolution produced duplicate identifier `${item.cName}`',
					[existing.request.sourceSymbol(), item.request.sourceSymbol()]);
			}
			byCandidate.set(key, item);
		}
	}

	static function appendHash(baseName:String, hash:String, hashLength:Int):String {
		final suffix = "_h" + hash.substr(0, hashLength);
		final keep = MAX_GENERATED_LENGTH - suffix.length;
		var readable = baseName.substr(0, keep);
		while (StringTools.endsWith(readable, "_")) {
			readable = readable.substr(0, readable.length - 1);
		}
		return readable + suffix;
	}

	static function encodeReadableParts(parts:Array<String>):String
		return parts.map(encodeReadablePart).join("_");

	/**
		Keep ordinary ASCII source spelling readable. Punctuation becomes a word
		separator and non-ASCII bytes receive a compact `_xHH` spelling. Because the
		semantic key remains separate, two source spellings that normalize to the
		same base are safely resolved by the ordinary collision pass.
	**/
	static function encodeReadablePart(value:String):String {
		final bytes = Bytes.ofString(value);
		final output = new StringBuf();
		var separatorPending = false;
		for (index in 0...bytes.length) {
			final byte = bytes.get(index);
			if (byte >= 0x41 && byte <= 0x5A || byte >= 0x61 && byte <= 0x7A || byte >= 0x30 && byte <= 0x39) {
				if (separatorPending && output.length > 0)
					output.add("_");
				output.addChar(byte);
				separatorPending = false;
			} else if (byte == 0x5F || byte < 0x80) {
				separatorPending = output.length > 0;
			} else {
				if (output.length > 0)
					output.add("_");
				output.add("x" + StringTools.hex(byte, 2));
				separatorPending = true;
			}
		}
		final encoded = output.toString();
		return encoded == "" ? "symbol" : encoded;
	}

	static function defaultReadableName(request:CSymbolRequest):Array<String> {
		final last = request.qualifiedName[request.qualifiedName.length - 1];
		return switch request.kind {
			case CSKLocal: [last];
			case CSKTemporary: ["tmp", last];
			case CSKField:
				switch request.namespace {
					case CNSMember(_): [last];
					case _: request.qualifiedName.copy();
				}
			case CSKPackage | CSKModule | CSKClosure | CSKClosureEnvironment | CSKVTable | CSKInterfaceTable | CSKTypeDescriptor | CSKReflectionEntry |
				CSKStaticInitializer | CSKRuntimePrivate:
				[roleToken(request.kind)].concat(request.qualifiedName);
			case CSKType | CSKMethod | CSKSpecialization | CSKExport:
				request.qualifiedName.copy();
		};
	}

	static function isAnonymousKind(kind:CSymbolKind):Bool {
		return switch kind {
			case CSKTemporary | CSKClosure | CSKClosureEnvironment: true;
			case _: false;
		};
	}

	static function generatedNameNeedsPrefix(name:String):Bool {
		if (name == "" || !isIdentifierStart(name.charCodeAt(0)))
			return true;
		return C_KEYWORDS.indexOf(name) != -1
			|| StringTools.startsWith(name, "_")
			|| name.indexOf("__") != -1
			|| StringTools.startsWith(name, "hxc_")
			|| StringTools.startsWith(name, "hxrt_")
			|| isLibraryReserved(name);
	}

	static function collectEscapeReasons(value:String, reasons:Array<String>):Void {
		if (C_KEYWORDS.indexOf(value) != -1) {
			addUnique(reasons, "c-keyword");
		}
		if (StringTools.startsWith(value, "_") || value.indexOf("__") != -1) {
			addUnique(reasons, "implementation-reserved");
		}
		if (StringTools.startsWith(value, "hxc_") || StringTools.startsWith(value, "hxrt_")) {
			addUnique(reasons, "reflaxe-reserved");
		}
		if (isLibraryReserved(value)) {
			addUnique(reasons, "standard-library-reserved");
		}
		var normalizedSeparator = StringTools.startsWith(value, "_") || StringTools.endsWith(value, "_") || value.indexOf("__") != -1;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (!isIdentifierPart(code)) {
				addUnique(reasons, "non-c-identifier-byte");
				normalizedSeparator = true;
			}
		}
		if (normalizedSeparator)
			addUnique(reasons, "separator-normalized");
	}

	static function isLibraryReserved(value:String):Bool {
		if (LIBRARY_NAMES.indexOf(value) != -1) {
			return true;
		}
		if (StringTools.startsWith(value, "atomic_")
			|| StringTools.startsWith(value, "cnd_")
			|| StringTools.startsWith(value, "mtx_")
			|| StringTools.startsWith(value, "thrd_")
			|| StringTools.startsWith(value, "tss_")) {
			return true;
		}
		return reservedLowercaseFamily(value, "is")
			|| reservedLowercaseFamily(value, "to")
			|| reservedLowercaseFamily(value, "str")
			|| reservedLowercaseFamily(value, "mem")
			|| reservedLowercaseFamily(value, "wcs");
	}

	static function reservedLowercaseFamily(value:String, prefix:String):Bool {
		if (!StringTools.startsWith(value, prefix) || value.length <= prefix.length) {
			return false;
		}
		final next = value.charCodeAt(prefix.length);
		return next != null && next >= 0x61 && next <= 0x7A;
	}

	static function roleToken(kind:CSymbolKind):String {
		return switch kind {
			case CSKPackage: "package";
			case CSKModule: "module";
			case CSKType: "type";
			case CSKField: "field";
			case CSKMethod: "method";
			case CSKLocal: "local";
			case CSKTemporary: "temp";
			case CSKSpecialization: "spec";
			case CSKClosure: "closure";
			case CSKClosureEnvironment: "closureenv";
			case CSKVTable: "vtable";
			case CSKInterfaceTable: "itable";
			case CSKTypeDescriptor: "typedesc";
			case CSKReflectionEntry: "reflection";
			case CSKStaticInitializer: "init";
			case CSKExport: "export";
			case CSKRuntimePrivate: "runtime";
		};
	}

	static function record(item:AssignedDraft):CSymbolRecord {
		return {
			semanticKey: item.request.stableKey(),
			sourceSymbol: item.request.sourceSymbol(),
			kind: CSymbolRequest.kindName(item.request.kind),
			namespace: namespaceRecord(item.request.namespace),
			visibility: CSymbolRequest.visibilityName(item.request.visibility),
			naming: item.request.explicitName == null ? "generated" : "explicit",
			requestedName: item.request.explicitName,
			readableName: item.request.readableName.length == 0 ? defaultReadableName(item.request) : item.request.readableName.copy(),
			baseName: item.baseName,
			cName: item.cName,
			collisionResolved: item.collisionResolved,
			escapeReasons: item.escapeReasons.copy(),
			overloadSignature: item.request.overloadSignature.copy(),
			specializationArguments: item.request.specializationArguments.copy(),
			sourceOrdinal: item.request.sourceOrdinal
		};
	}

	static function namespaceRecord(namespace:CSymbolNamespace):CSymbolNamespaceRecord {
		return {
			kind: CSymbolRequest.namespaceKind(namespace),
			scope: CSymbolRequest.namespaceScope(namespace)
		};
	}

	static function compareRequests(left:CSymbolRequest, right:CSymbolRequest):Int {
		final sourceOrder = compareUtf8(left.sourceSymbol(), right.sourceSymbol());
		if (sourceOrder != 0) {
			return sourceOrder;
		}
		final kindOrder = compareUtf8(CSymbolRequest.kindName(left.kind), CSymbolRequest.kindName(right.kind));
		if (kindOrder != 0) {
			return kindOrder;
		}
		final namespaceOrder = compareUtf8(CSymbolRequest.namespaceKey(left.namespace), CSymbolRequest.namespaceKey(right.namespace));
		return namespaceOrder != 0 ? namespaceOrder : compareUtf8(left.stableKey(), right.stableKey());
	}

	static function compareCollisions(left:CSymbolCollisionRecord, right:CSymbolCollisionRecord):Int
		return compareUtf8('${left.namespace.kind}\x00${left.namespace.scope}\x00${left.baseName}',
			'${right.namespace.kind}\x00${right.namespace.scope}\x00${right.baseName}');

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function addUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function isIdentifierStart(code:Null<Int>):Bool
		return code != null && (code == 0x5F || code >= 0x41 && code <= 0x5A || code >= 0x61 && code <= 0x7A);

	static function isIdentifierPart(code:Null<Int>):Bool
		return isIdentifierStart(code) || code != null && code >= 0x30 && code <= 0x39;

	static function contractFailure<T>(detail:String, sources:Array<String>):T
		throw new CSymbolRegistryError(CDiagnosticId.InvalidTypedCContract, detail, sources);

	static function internalFailure<T>(detail:String, sources:Array<String>):T
		throw new CSymbolRegistryError(CDiagnosticId.InternalCompilerError, detail, sources);
}
