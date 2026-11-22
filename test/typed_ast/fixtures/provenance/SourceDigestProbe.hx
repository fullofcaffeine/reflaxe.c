import haxe.macro.Context;
import haxe.macro.Type;
import haxe.io.Bytes;
import haxe.io.Path;
import reflaxe.c.frontend.SourceContentDigest;
import reflaxe.c.frontend.NamedRecordSourceProvenance;
import reflaxe.c.frontend.TypedFunctionSourceProvenance;
import reflaxe.c.lowering.HaxeSourceSpan.HaxeSourceSpanResolver;
import sys.FileSystem;
import sys.io.File;

/** Checks that function and record provenance still observe their own file reads. */
class SourceDigestProbe {
	/** Resolve the temporary source only after macro initialization is complete. */
	public static function run():Void {
		Context.onAfterInitMacros(check);
	}

	/** File changes between the two owners must never reuse stale content identity. */
	static function check():Void {
		final definition = switch Context.getType("SharedSource.SharedRecord") {
			case TType(reference, _): reference.get();
			case _: throw new haxe.Exception("missing shared-source record");
		};
		final anonymous = switch definition.type {
			case TAnonymous(reference): reference;
			case _: throw new haxe.Exception("shared-source record is not anonymous");
		};
		final file = Context.getPosInfos(definition.pos).file;
		final original = File.getContent(file);
		final changed = StringTools.replace(original, "return 1", "return 2");
		if (changed == original || changed.length != original.length)
			throw new haxe.Exception("same-length source edit anchor missing");
		var digests = new SourceContentDigest();
		final owner = new TypedFunctionSourceProvenance(digests);
		final position = Context.makePosition({file: file, min: 1, max: 2});
		final expression:TypedExpr = {expr: TConst(TInt(1)), t: Context.getType("Int"), pos: position};
		function capture(current:TypedFunctionSourceProvenance):Bool {
			current.plan("SharedSource", "value", position, expression);
			final shifted = Context.makePosition({file: file, min: 3, max: 4});
			final next:TypedExpr = {expr: TConst(TInt(1)), t: expression.t, pos: shifted};
			final plan = current.plan("SharedSource", "value", position, next);
			final replacement = plan.positionOverrides.get(TypedFunctionSourceProvenance.exactPositionKey(shifted));
			if (replacement == null)
				return false;
			final info = Context.getPosInfos(replacement);
			if (info.file != file || info.min != 1 || info.max != 2)
				throw new haxe.Exception("readable source failed displaced warm recovery");
			return true;
		}
		function record():Void {
			final plan = NamedRecordSourceProvenance.planRecord("SharedSource.SharedRecord", definition.pos, anonymous, digests);
			final field = anonymous.get().fields[0];
			if (Context.getPosInfos(plan.requireFieldPosition("SharedSource.SharedRecord", field.name, field.pos)).min != Context.getPosInfos(field.pos).min)
				throw new haxe.Exception("record source position changed");
		}
		if (!capture(owner))
			throw new haxe.Exception("function source identity missing");
		final resolved = owner.plan("SharedSource", "value", position, expression);
		final resolver = new HaxeSourceSpanResolver("SharedSource.hx", false, resolved.positionOverrides);
		if (resolver.resolve(position).display() != "SharedSource.hx:1:2-1:3")
			throw new haxe.Exception("function source has incorrect effective coordinates");
		record();
		Sys.println("SOURCE_DIGEST unchanged");
		// Exercise public recovery with use-site positions before any edited record is remembered.
		final displacedFields = anonymous.get().fields;
		final declarationPosition = displacedFields[0].pos;
		displacedFields[0].pos = Context.makePosition({file: file, min: original.length - 2, max: original.length - 1});
		final displacedType:AnonType = {fields: displacedFields, status: AClosed};
		final displaced:Ref<AnonType> = {get: () -> displacedType, toString: () -> "displaced source fields"};
		final recovered = NamedRecordSourceProvenance.planRecord("SharedSource.SharedRecord", definition.pos, displaced, digests);
		if (Context.getPosInfos(recovered.requireFieldPosition("SharedSource.SharedRecord", "value", displacedFields[0].pos))
			.min != Context.getPosInfos(declarationPosition)
			.min)
			throw new haxe.Exception("unchanged source did not recover its declaration position");
		File.saveContent(file, changed);
		var editRejected = false;
		try {
			NamedRecordSourceProvenance.planRecord("SharedSource.SharedRecord", definition.pos, displaced, digests);
		} catch (error:reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourceProvenanceError) {
			editRejected = error.detail == "Cannot recover declaration positions for named record `SharedSource.SharedRecord`: this Haxe compiler process has no earlier "
				+ "declaration-position record for the exact current source bytes and defines. Start a fresh Haxe compiler process once; "
				+ "later unchanged requests may reuse that verified record.";
		}
		displacedFields[0].pos = declarationPosition;
		if (!editRejected)
			throw new haxe.Exception("same-length edit reused stale declaration recovery");
		Sys.println("SOURCE_DIGEST recovery-invalidated");
		File.saveContent(file, changed);
		record();
		if (!capture(owner))
			throw new haxe.Exception("function snapshot was discarded after an edit");
		Sys.println("SOURCE_DIGEST same-length-edit");
		FileSystem.deleteFile(file);
		record(); // Authoritative fields remain usable when their source cannot be read.
		final retry = new TypedFunctionSourceProvenance(digests);
		if (capture(retry))
			throw new haxe.Exception("missing source reused another owner's snapshot");
		Sys.println("SOURCE_DIGEST deleted");
		File.saveContent(file, original);
		if (!capture(retry))
			throw new haxe.Exception("recreated source was not retried");
		record();
		Sys.println("SOURCE_DIGEST recreated");
		File.saveContent(file, "");
		var rejected = false;
		try {
			record();
		} catch (error:reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourceProvenanceError) {
			rejected = error.detail == "Named record `SharedSource.SharedRecord` has a typedef range outside its current source bytes.";
		}
		if (!rejected)
			throw new haxe.Exception("record accepted offsets outside newly read bytes");
		Sys.println("SOURCE_DIGEST invalid-new-bytes");
		File.saveContent(file, original);
		digests = new SourceContentDigest();
		if (!capture(new TypedFunctionSourceProvenance(digests)))
			throw new haxe.Exception("new request lost source identity");
		record();
		Sys.println("SOURCE_DIGEST new-owner");
		checkPrivateBytes();
		checkLookupHints(file);
		if (Sys.systemName() == "Windows") {
			Sys.println("SOURCE_DIGEST read-errors-skipped: POSIX permissions unavailable");
		} else if (Sys.command("chmod", ["000", file]) != 0) {
			if (Sys.command("chmod", ["600", file]) != 0)
				throw new haxe.Exception("could not restore source permissions");
			Sys.println("SOURCE_DIGEST read-errors-skipped: chmod precondition unavailable");
		} else {
			var readFailed = false;
			try {
				File.getBytes(file);
			} catch (_:haxe.Exception) {
				readFailed = true;
			}
			if (!readFailed) {
				if (Sys.command("chmod", ["600", file]) != 0)
					throw new haxe.Exception("could not restore source permissions");
				Sys.println("SOURCE_DIGEST read-errors-skipped: host can read mode-000 files");
			} else {
				final functionRejectedRead = !capture(new TypedFunctionSourceProvenance(digests));
				var recordRejectedRead = false;
				try {
					record();
				} catch (_:haxe.Exception) {
					recordRejectedRead = true;
				}
				if (Sys.command("chmod", ["600", file]) != 0 || !functionRejectedRead || !recordRejectedRead)
					throw new haxe.Exception("source-read failure policy changed");
				Sys.println("SOURCE_DIGEST read-errors");
			}
		}
		Sys.println("HXC_SOURCE_DIGEST_OK");
	}

	/** Path aliases only select a comparison candidate; byte changes must still hash. */
	static function checkLookupHints(file:String):Void {
		final digests = new SourceContentDigest();
		final abc = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
		final abd = "a52d159f262b2c6ddb724a61840befc36eb30c88877a4030b65cbe86298449c9";
		final originalDirectory = Sys.getCwd();
		final firstDirectory = Path.join([Path.directory(file), "digest-first"]);
		final secondDirectory = Path.join([Path.directory(file), "digest-second"]);
		FileSystem.createDirectory(firstDirectory);
		FileSystem.createDirectory(secondDirectory);
		try {
			Sys.setCwd(firstDirectory);
			final absolute = Path.join([Sys.getCwd(), "source"]);
			// The nonexistent directory makes filesystem canonicalization inappropriate.
			final alias = "not-created/../source";
			if (FileSystem.exists("not-created"))
				throw new haxe.Exception("lexical alias precondition changed");
			if (digests.digest("source", Bytes.ofString("abc")) != abc
				|| digests.digest(absolute, Bytes.ofString("abc")) != abc
				|| digests.digest(alias, Bytes.ofString("abc")) != abc)
				throw new haxe.Exception("relative or lexical alias changed source digest");
			Sys.println("SOURCE_DIGEST lexical-aliases");
			if (digests.digest(alias, Bytes.ofString("abd")) != abd || digests.digest(absolute, Bytes.ofString("abc")) != abc)
				throw new haxe.Exception("a shared hint reused unequal bytes");
			Sys.println("SOURCE_DIGEST unequal-hint-collision");
			final changingDirectory = new SourceContentDigest();
			if (changingDirectory.digest("source", Bytes.ofString("abc")) != abc)
				throw new haxe.Exception("first directory source digest differs");
			Sys.setCwd(secondDirectory);
			final secondAbsolute = Path.join([Sys.getCwd(), "source"]);
			if (changingDirectory.digest("source", Bytes.ofString("abd")) != abd
				|| changingDirectory.digest(absolute, Bytes.ofString("abc")) != abc
				|| changingDirectory.digest(secondAbsolute, Bytes.ofString("abd")) != abd)
				throw new haxe.Exception("working-directory change reused unequal source bytes");
			Sys.println("SOURCE_DIGEST cwd-change");
		} catch (error:haxe.Exception) {
			Sys.setCwd(originalDirectory);
			throw error;
		}
		Sys.setCwd(originalDirectory);
	}

	/** A mutable input must not mutate the private bytes paired with an old digest. */
	static function checkPrivateBytes():Void {
		final digests = new SourceContentDigest();
		final bytes = Bytes.ofString("abc");
		final abc = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
		if (digests.digest("source", bytes) != abc)
			throw new haxe.Exception("known source digest differs");
		bytes.set(2, "d".code);
		if (digests.digest("source", Bytes.ofString("abc")) != abc || digests.digest("source", bytes) == abc)
			throw new haxe.Exception("caller mutation changed private digest bytes");
		Sys.println("SOURCE_DIGEST private-bytes");
	}
}
