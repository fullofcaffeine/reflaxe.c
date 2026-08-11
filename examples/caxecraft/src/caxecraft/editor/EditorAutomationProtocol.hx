package caxecraft.editor;

import caxecraft.content.ContentJson;
import caxecraft.content.ContentJson.ContentJsonField;
import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJson.ContentJsonReadResult;
import caxecraft.content.ContentJson.ContentJsonValue;
import caxecraft.content.ContentJsonWriter.appendJsonString;
import caxecraft.editor.EditorPackageSession.EditorPackageSaveResult;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorMutation;
import caxecraft.editor.EditorTypes.EditorMutationResult;
import caxecraft.editor.EditorTypes.EditorObservation;
import caxecraft.editor.EditorTypes.EditorQuery;
import caxecraft.editor.EditorWorldViewport.projectWorld;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import haxe.io.Bytes;

/**
 * Converts bounded JSON requests into the shared editor command model.
 *
 * The local process and a later MCP adapter can use this protocol. JSON is a
 * transport detail only. Each accepted mutation still passes through
 * `EditorPackageSession` and `EditorSession`.
 */
enum EditorAutomationAction {
	ReadState;
	ReadSurface;
	ReadColumn(x:Int, z:Int);
	Paint(baseRevision:Int, paletteCode:Int, points:Array<VoxelPoint>);
	Erase(baseRevision:Int, points:Array<VoxelPoint>);
	Undo(baseRevision:Int);
	Redo(baseRevision:Int);
	Validate;
	Save(baseRevision:Int);
	Quit;
}

/** One request ID and its closed editor action. */
typedef EditorAutomationRequest = {
	final requestId:Int;
	final action:EditorAutomationAction;
}

/** A decoded request or one source-located protocol error. */
enum EditorAutomationReadResult {
	AutomationRequestReady(request:EditorAutomationRequest);
	AutomationRequestRejected(detail:String);
}

/** One encoded response and whether the local process must stop. */
typedef EditorAutomationResponse = {
	final json:String;
	final quit:Bool;
}

/** Decode one complete JSON request into closed types with exact fields. */
function readEditorAutomationRequest(input:Bytes):EditorAutomationReadResult {
	final root = switch ContentJson.read(input) {
		case ContentJsonRejected(diagnostic): return
				AutomationRequestRejected('invalid JSON at ${diagnostic.line}:${diagnostic.column}: ${Std.string(diagnostic.kind)}');
		case ContentJsonReady(value): value;
	};
	final fields = switch root.value {
		case JsonObject(value): value;
		case _: return AutomationRequestRejected("request must be a JSON object");
	};
	final schemaVersion = integerField(fields, "schemaVersion", 1, 1);
	final requestId = integerField(fields, "requestId", 0, 2147483647);
	final command = stringField(fields, "command", 64);
	if (schemaVersion == null || requestId == null || command == null)
		return AutomationRequestRejected("request has a missing or invalid schemaVersion, requestId, or command");
	return switch command {
		case "state": exact(fields, ["schemaVersion", "requestId", "command"]) ? ready(requestId, ReadState) : unknownFields();
		case "surface": exact(fields, ["schemaVersion", "requestId", "command"]) ? ready(requestId, ReadSurface) : unknownFields();
		case "column": readColumn(fields, requestId);
		case "paint": readVoxelMutation(fields, requestId, true);
		case "erase": readVoxelMutation(fields, requestId, false);
		case "undo": readRevisionAction(fields, requestId, true);
		case "redo": readRevisionAction(fields, requestId, false);
		case "validate": exact(fields, ["schemaVersion", "requestId", "command"]) ? ready(requestId, Validate) : unknownFields();
		case "save": readSave(fields, requestId);
		case "quit": exact(fields, ["schemaVersion", "requestId", "command"]) ? ready(requestId, Quit) : unknownFields();
		case _: AutomationRequestRejected('unknown editor command: $command');
	};
}

/** Apply one decoded request and encode its copy-owned result. */
function runEditorAutomationRequest(editor:EditorPackageSession, request:EditorAutomationRequest):EditorAutomationResponse {
	return switch request.action {
		case ReadState: response(encodeState(editor, request.requestId), false);
		case ReadSurface: response(encodeSurface(editor, request.requestId), false);
		case ReadColumn(x, z): response(encodeColumn(editor, request.requestId, x, z), false);
		case Paint(baseRevision, paletteCode, points):
			response(encodeMutation(request.requestId, editor.mutate({baseRevision: baseRevision, mutation: Apply(PaintVoxels(points, paletteCode))})), false);
		case Erase(baseRevision, points):
			response(encodeMutation(request.requestId, editor.mutate({baseRevision: baseRevision, mutation: Apply(EraseVoxels(points))})), false);
		case Undo(baseRevision): response(encodeMutation(request.requestId, editor.mutate({baseRevision: baseRevision, mutation: EditorMutation.Undo})), false);
		case Redo(baseRevision): response(encodeMutation(request.requestId, editor.mutate({baseRevision: baseRevision, mutation: EditorMutation.Redo})), false);
		case Validate: response(encodeValidation(editor, request.requestId), false);
		case Save(baseRevision): response(encodeSave(request.requestId, editor.save(baseRevision)), false);
		case Quit: response(successStart(request.requestId, "bye") + '}', true);
	};
}

/** Encode the message emitted when one real level is ready. */
function editorAutomationReady(levelPath:String, revision:Int):String {
	final output = new StringBuf();
	output.add('{"schemaVersion":1,"type":"ready","level":');
	appendJsonString(output, levelPath);
	output.add(',"revision":$revision}');
	return output.toString();
}

/** Encode an error that has no accepted request ID. */
function editorAutomationError(detail:String):String {
	final output = new StringBuf();
	output.add('{"schemaVersion":1,"type":"error","detail":');
	appendJsonString(output, detail);
	output.add('}');
	return output.toString();
}

private function readVoxelMutation(fields:Array<ContentJsonField>, requestId:Int, paint:Bool):EditorAutomationReadResult {
	final expected = paint ? ["schemaVersion", "requestId", "command", "baseRevision", "paletteCode", "points"] : ["schemaVersion", "requestId", "command", "baseRevision", "points"];
	if (!exact(fields, expected))
		return unknownFields();
	final baseRevision = integerField(fields, "baseRevision", 0, 2147483647);
	final paletteCode = paint ? integerField(fields, "paletteCode", 0, 255) : 0;
	final points = pointField(fields, "points");
	if (baseRevision == null || paletteCode == null || points == null)
		return AutomationRequestRejected("voxel command has an invalid revision, palette code, or point list");
	return ready(requestId, paint ? Paint(baseRevision, paletteCode, points) : Erase(baseRevision, points));
}

private function readColumn(fields:Array<ContentJsonField>, requestId:Int):EditorAutomationReadResult {
	if (!exact(fields, ["schemaVersion", "requestId", "command", "x", "z"]))
		return unknownFields();
	final x = integerField(fields, "x", 0, 2147483647);
	final z = integerField(fields, "z", 0, 2147483647);
	return x == null
		|| z == null ? AutomationRequestRejected("column command has an invalid x or z coordinate") : ready(requestId, ReadColumn(x, z));
}

private function readRevisionAction(fields:Array<ContentJsonField>, requestId:Int, undo:Bool):EditorAutomationReadResult {
	if (!exact(fields, ["schemaVersion", "requestId", "command", "baseRevision"]))
		return unknownFields();
	final baseRevision = integerField(fields, "baseRevision", 0, 2147483647);
	return baseRevision == null ? AutomationRequestRejected("history command has an invalid baseRevision") : ready(requestId,
		undo ? Undo(baseRevision) : Redo(baseRevision));
}

private function readSave(fields:Array<ContentJsonField>, requestId:Int):EditorAutomationReadResult {
	if (!exact(fields, ["schemaVersion", "requestId", "command", "baseRevision"]))
		return unknownFields();
	final baseRevision = integerField(fields, "baseRevision", 0, 2147483647);
	return baseRevision == null ? AutomationRequestRejected("save command has an invalid baseRevision") : ready(requestId, Save(baseRevision));
}

private function pointField(fields:Array<ContentJsonField>, name:String):Null<Array<VoxelPoint>> {
	final node = find(fields, name);
	if (node == null)
		return null;
	final values = switch node.value {
		case JsonArray(entries): entries;
		case _: return null;
	};
	if (values.length == 0)
		return null;
	final points:Array<VoxelPoint> = [];
	for (entry in values) {
		final pointFields = switch entry.value {
			case JsonObject(value): value;
			case _: return null;
		};
		if (!exact(pointFields, ["x", "y", "z"]))
			return null;
		final x = integerField(pointFields, "x", 0, 2147483647);
		final y = integerField(pointFields, "y", 0, 2147483647);
		final z = integerField(pointFields, "z", 0, 2147483647);
		if (x == null || y == null || z == null)
			return null;
		points.push({x: x, y: y, z: z});
	}
	return points;
}

private function integerField(fields:Array<ContentJsonField>, name:String, minimum:Int, maximum:Int):Null<Int> {
	final node = find(fields, name);
	if (node == null)
		return null;
	return switch node.value {
		case JsonNumber(lexeme): parseInteger(lexeme, minimum, maximum);
		case _: null;
	};
}

private function parseInteger(value:String, minimum:Int, maximum:Int):Null<Int> {
	if (value.length == 0 || value.indexOf(".") >= 0 || value.indexOf("e") >= 0 || value.indexOf("E") >= 0)
		return null;
	var parsed = 0;
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (code < 48 || code > 57 || parsed > Std.int((maximum - (code - 48)) / 10))
			return null;
		parsed = parsed * 10 + code - 48;
	}
	return parsed < minimum || parsed > maximum ? null : parsed;
}

private function stringField(fields:Array<ContentJsonField>, name:String, maximumBytes:Int):Null<String> {
	final node = find(fields, name);
	if (node == null)
		return null;
	return switch node.value {
		case JsonString(value) if (value.length > 0 && Bytes.ofString(value).length <= maximumBytes): value;
		case _: null;
	};
}

private function find(fields:Array<ContentJsonField>, name:String):Null<ContentJsonNode> {
	for (field in fields)
		if (field.name == name)
			return field.value;
	return null;
}

private function exact(fields:Array<ContentJsonField>, expected:Array<String>):Bool {
	if (fields.length != expected.length)
		return false;
	for (name in expected)
		if (find(fields, name) == null)
			return false;
	return true;
}

private function ready(requestId:Int, action:EditorAutomationAction):EditorAutomationReadResult
	return AutomationRequestReady({requestId: requestId, action: action});

private function unknownFields():EditorAutomationReadResult
	return AutomationRequestRejected("request fields do not match the selected command");

private function response(json:String, quit:Bool):EditorAutomationResponse
	return {json: json, quit: quit};

private function encodeState(editor:EditorPackageSession, requestId:Int):String {
	return switch editor.query(InspectState) {
		case StateObserved(state):
			final output = new StringBuf();
			output.add(successStart(requestId, "state"));
			output.add(',"revision":${state.revision},"undoDepth":${state.undoDepth},"redoDepth":${state.redoDepth},"editing":${state.editing}}');
			output.toString();
		case _: errorFor(requestId, "editor returned the wrong state observation");
	};
}

private function encodeSurface(editor:EditorPackageSession, requestId:Int):String {
	return switch editor.query(InspectDraft) {
		case DraftObserved(revision, draft):
			final projection = projectWorld(draft.world);
			if (projection == null)
				return errorFor(requestId, "draft world cannot be projected");
			final output = new StringBuf();
			output.add(successStart(requestId, "surface"));
			output.add(',"revision":$revision,"width":${projection.width},"height":${projection.height},"depth":${projection.depth},"columns":[');
			var first = true;
			for (z in 0...projection.depth)
				for (x in 0...projection.width) {
					var surfaceY = -1;
					var paletteCode = 0;
					var y = projection.height - 1;
					while (y >= 0 && surfaceY < 0) {
						final candidate = projection.cells[(z * projection.height + y) * projection.width + x];
						if (candidate != 0) {
							surfaceY = y;
							paletteCode = candidate;
						}
						y--;
					}
					if (!first)
						output.add(',');
					first = false;
					output.add('{"x":$x,"z":$z,"surfaceY":$surfaceY,"paletteCode":$paletteCode}');
				}
			output.add(']}');
			output.toString();
		case _: errorFor(requestId, "editor returned the wrong draft observation");
	};
}

private function encodeColumn(editor:EditorPackageSession, requestId:Int, x:Int, z:Int):String {
	return switch editor.query(InspectDraft) {
		case DraftObserved(revision, draft):
			final projection = projectWorld(draft.world);
			if (projection == null)
				return errorFor(requestId, "draft world cannot be projected");
			if (x < 0 || z < 0 || x >= projection.width || z >= projection.depth)
				return errorFor(requestId, "column coordinate is outside the draft world");
			var surfaceY = -1;
			var paletteCode = 0;
			var y = projection.height - 1;
			while (y >= 0 && surfaceY < 0) {
				final candidate = projection.cells[(z * projection.height + y) * projection.width + x];
				if (candidate != 0) {
					surfaceY = y;
					paletteCode = candidate;
				}
				y--;
			}
			successStart(requestId, "column") + ',"revision":$revision,"x":$x,"z":$z,"surfaceY":$surfaceY,"paletteCode":$paletteCode}';
		case _: errorFor(requestId, "editor returned the wrong draft observation");
	};
}

private function encodeMutation(requestId:Int, result:EditorMutationResult):String {
	return switch result {
		case MutationApplied(_, changes, revision, undoDepth, redoDepth):
			successStart(requestId, "mutation") + ',"revision":$revision,"changed":${changes.length},"undoDepth":$undoDepth,"redoDepth":$redoDepth}';
		case MutationUnchanged(_, revision): successStart(requestId, "unchanged") + ',"revision":$revision}';
		case MutationRejected(error, revision): errorFor(requestId, '${Std.string(error)} at revision $revision');
	};
}

private function encodeValidation(editor:EditorPackageSession, requestId:Int):String {
	return switch editor.query(InspectValidation) {
		case ValidationObserved(revision, DraftPlayable(canonical)):
			successStart(requestId, "validation") + ',"revision":$revision,"playable":true,"canonicalBytes":${canonical.length}}';
		case ValidationObserved(revision, DraftInvalid(diagnostics)):
			successStart(requestId, "validation") + ',"revision":$revision,"playable":false,"diagnostics":${diagnostics.length}}';
		case ValidationObserved(revision, DraftUnreadable(error)):
			errorFor(requestId, '${Std.string(error)} at revision $revision');
		case _: errorFor(requestId, "editor returned the wrong validation observation");
	};
}

private function encodeSave(requestId:Int, result:EditorPackageSaveResult):String {
	return switch result {
		case EditorPackageSaved(revision, changed, warnings):
			successStart(requestId, "save") + ',"revision":$revision,"changedFiles":$changed,"cleanupWarnings":${warnings.length}}';
		case EditorPackageSaveRejected(error): errorFor(requestId, Std.string(error));
	};
}

private function successStart(requestId:Int, type:String):String
	return '{"schemaVersion":1,"requestId":$requestId,"type":"$type"';

private function errorFor(requestId:Int, detail:String):String {
	final output = new StringBuf();
	output.add('{"schemaVersion":1,"requestId":$requestId,"type":"error","detail":');
	appendJsonString(output, detail);
	output.add('}');
	return output.toString();
}
