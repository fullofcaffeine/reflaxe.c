package reflaxe.c.lowering;

import reflaxe.c.ir.HxcSourceSpan;
#if (macro || reflaxe_runtime)
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchCallKind;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchCatalog;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchGraph;
import reflaxe.c.lowering.CBodyDispatch.CBodyDispatchPreparer;
import reflaxe.c.lowering.CBodyDispatch.CLoweredBodyDispatch;
#end

typedef CDispatchSourceSnapshot = {
	final file:String;
	final startLine:Int;
	final startColumn:Int;
	final endLine:Int;
	final endColumn:Int;
}

typedef CDispatchCallSnapshot = {
	final id:String;
	final callerFunctionId:String;
	final methodFunctionId:String;
	final receiverStaticClass:String;
	final dispatch:String;
	final reason:String;
	final targetFunctionId:Null<String>;
	final slotId:Null<String>;
	final source:CDispatchSourceSnapshot;
}

typedef CDispatchLayoutSnapshot = {
	final id:String;
	final rootInstanceId:String;
	final cTag:String;
	final slotIds:Array<String>;
}

typedef CDispatchSlotSnapshot = {
	final id:String;
	final ownerInstanceId:String;
	final cMember:String;
	final parameterRepresentations:Array<String>;
	final returnRepresentation:String;
}

typedef CDispatchTableEntrySnapshot = {
	final slotId:String;
	final implementationFunctionId:Null<String>;
	final adapterCName:Null<String>;
}

typedef CDispatchTableSnapshot = {
	final id:String;
	final layoutId:String;
	final classInstanceId:String;
	final cName:String;
	final entries:Array<CDispatchTableEntrySnapshot>;
}

typedef CDispatchSummarySnapshot = {
	final instanceCalls:Int;
	final directCalls:Int;
	final indirectCalls:Int;
	final layouts:Int;
	final slots:Int;
	final tables:Int;
	final adapters:Int;
}

/** Deterministic explanation of every reachable instance-dispatch choice. */
typedef CDispatchReportSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:String;
	final tablePolicy:String;
	final adapterPolicy:String;
	final runtimeFeatures:Array<String>;
	final summary:CDispatchSummarySnapshot;
	final calls:Array<CDispatchCallSnapshot>;
	final layouts:Array<CDispatchLayoutSnapshot>;
	final slots:Array<CDispatchSlotSnapshot>;
	final tables:Array<CDispatchTableSnapshot>;
}

#if (macro || reflaxe_runtime)
class CDispatchReportBuilder {
	public static inline final SCHEMA_VERSION = 1;
	public static inline final ALGORITHM = "hxc-closed-world-virtual-dispatch-v1";
	public static inline final STATUS = "analyzed-reachable-instance-dispatch";
	public static inline final TABLE_POLICY = "one-root-layout-reachable-virtual-slots-only";
	public static inline final ADAPTER_POLICY = "representation-identical-overrides-with-typed-self-thunks";

	public function new() {}

	public function build(graph:CBodyDispatchGraph, lowered:CLoweredBodyDispatch):Null<CDispatchReportSnapshot> {
		if (graph.calls.length == 0 && lowered.layouts.length == 0 && lowered.slots.length == 0 && lowered.tables.length == 0)
			return null;
		final calls:Array<CDispatchCallSnapshot> = [];
		var directCalls = 0;
		var indirectCalls = 0;
		for (index in 0...graph.calls.length) {
			final call = graph.calls[index];
			final choice = switch call.kind {
				case CBDDirect(targetFunctionId, reason):
					directCalls++;
					{
						dispatch: "direct",
						reason: reason,
						targetFunctionId: targetFunctionId,
						slotId: null
					};
				case CBDVirtual(slotId, reason):
					indirectCalls++;
					{
						dispatch: "virtual",
						reason: reason,
						targetFunctionId: null,
						slotId: slotId
					};
			};
			calls.push({
				id: 'dispatch.call.$index',
				callerFunctionId: call.callerFunctionId,
				methodFunctionId: call.methodFunctionId,
				receiverStaticClass: call.receiverStaticClass,
				dispatch: choice.dispatch,
				reason: choice.reason,
				targetFunctionId: choice.targetFunctionId,
				slotId: choice.slotId,
				source: sourceSnapshot(call.source)
			});
		}
		final layouts:Array<CDispatchLayoutSnapshot> = lowered.layouts.map(layout -> {
			id: layout.prepared.id,
			rootInstanceId: layout.prepared.root.instanceId,
			cTag: layout.cTag.value,
			slotIds: layout.slots.map(slot -> slot.prepared.input.id)
		});
		final slots:Array<CDispatchSlotSnapshot> = lowered.slots.map(slot -> {
			id: slot.prepared.input.id,
			ownerInstanceId: slot.prepared.owner.instanceId,
			cMember: slot.cMember.value,
			parameterRepresentations: slot.prepared.parameters.map(value -> CBodyDispatchPreparer.typeKey(value.irType)),
			returnRepresentation: CBodyDispatchPreparer.typeKey(slot.prepared.returnType.irType)
		});
		final tables:Array<CDispatchTableSnapshot> = lowered.tables.map(table -> {
			id: table.prepared.input.id,
			layoutId: table.prepared.layout.id,
			classInstanceId: table.prepared.classValue.instanceId,
			cName: table.cName.value,
			entries: table.entries.map(entry -> {
				slotId: entry.slot.prepared.input.id,
				implementationFunctionId: entry.implementationFunctionId,
				adapterCName: entry.thunk == null ? null : entry.thunk.cName.value
			})
		});
		layouts.sort((left, right) -> CBodyDispatchCatalog.compareUtf8(left.id, right.id));
		slots.sort((left, right) -> CBodyDispatchCatalog.compareUtf8(left.id, right.id));
		tables.sort((left, right) -> CBodyDispatchCatalog.compareUtf8(left.id, right.id));
		return {
			schemaVersion: SCHEMA_VERSION,
			algorithm: ALGORITHM,
			status: STATUS,
			tablePolicy: TABLE_POLICY,
			adapterPolicy: ADAPTER_POLICY,
			runtimeFeatures: [],
			summary: {
				instanceCalls: calls.length,
				directCalls: directCalls,
				indirectCalls: indirectCalls,
				layouts: layouts.length,
				slots: slots.length,
				tables: tables.length,
				adapters: lowered.thunks.length
			},
			calls: calls,
			layouts: layouts,
			slots: slots,
			tables: tables
		};
	}

	static function sourceSnapshot(source:HxcSourceSpan):CDispatchSourceSnapshot
		return {
			file: source.file,
			startLine: source.startLine,
			startColumn: source.startColumn,
			endLine: source.endLine,
			endColumn: source.endColumn
		};
}
#else
class CDispatchReportBuilder {
	public static inline final SCHEMA_VERSION = 1;
	public static inline final ALGORITHM = "hxc-closed-world-virtual-dispatch-v1";
	public static inline final STATUS = "analyzed-reachable-instance-dispatch";
	public static inline final TABLE_POLICY = "one-root-layout-reachable-virtual-slots-only";
	public static inline final ADAPTER_POLICY = "representation-identical-overrides-with-typed-self-thunks";

	public function new() {}
}
#end
