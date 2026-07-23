package reflaxe.c.lowering;

import reflaxe.c.ast.CAST.CIdentifier;

/**
	Closed authority for fixed names declared by checked-in hxrt headers.

	These names are compiler/runtime ABI facts, never derived from Haxe input.
	Keeping the vocabulary closed prevents runtime dispatch from becoming a raw-C
	symbol escape hatch while still routing every token through CIdentifier.
**/
enum CBodyRuntimeName {
	CBRNStringType;
	CBRNPrintln;
	CBRNStatusType;
	CBRNStatusOk;
	CBRNAbort;
	CBRNDefaultAllocator;
	CBRNAllocatorType;
	CBRNAllocate;
	CBRNFree;
	CBRNGcRootFrameType;
	CBRNGcRootFrameInitializer;
	CBRNGcRootFramePush;
	CBRNGcRootFramePop;
	CBRNGcType;
	CBRNGcThreadType;
	CBRNGcConfigType;
	CBRNGcInitializer;
	CBRNGcThreadInitializer;
	CBRNGcInit;
	CBRNGcAllocate;
	CBRNGcDispose;
	CBRNGcThreadRegister;
	CBRNGcThreadUnregister;
	CBRNArrayCreate;
	CBRNArrayCreateTrivial;
	CBRNArrayInitInPlace;
	CBRNArrayDisposeInPlace;
	CBRNArrayElementOpsType;
	CBRNArrayGetCopy;
	CBRNArrayLength;
	CBRNArrayPushCopy;
	CBRNArraySetCopy;
	CBRNArrayRetain;
	CBRNArrayRelease;
	CBRNArrayStoragePushCopy;
	CBRNArrayValueMember;
	CBRNStringMapCreate;
	CBRNStringMapRetain;
	CBRNStringMapRelease;
	CBRNStringMapSetCopy;
	CBRNStringMapExists;
	CBRNStringMapGetCopy;
	CBRNStringMapRemove;
	CBRNStringMapClear;
	CBRNBytesCreateZeroed;
	CBRNBytesCreateUtf8Copy;
	CBRNBytesLength;
	CBRNBytesGet;
	CBRNBytesSet;
	CBRNBytesSub;
	CBRNBytesBlit;
	CBRNBytesFill;
	CBRNBytesCompare;
	CBRNBytesRetain;
	CBRNBytesRelease;
}

class CBodyRuntimeNames {
	public static function identifier(name:CBodyRuntimeName):CIdentifier {
		return new CIdentifier(switch name {
			case CBRNStringType: "hxc_string";
			case CBRNPrintln: "hxc_io_println";
			case CBRNStatusType: "hxc_status";
			case CBRNStatusOk: "HXC_STATUS_OK";
			case CBRNAbort: "abort";
			case CBRNDefaultAllocator: "hxc_default_allocator";
			case CBRNAllocatorType: "hxc_allocator";
			case CBRNAllocate: "hxc_alloc";
			case CBRNFree: "hxc_free";
			case CBRNGcRootFrameType: "hxc_gc_root_frame";
			case CBRNGcRootFrameInitializer: "HXC_GC_ROOT_FRAME_INITIALIZER";
			case CBRNGcRootFramePush: "hxc_gc_root_frame_push";
			case CBRNGcRootFramePop: "hxc_gc_root_frame_pop";
			case CBRNGcType: "hxc_gc";
			case CBRNGcThreadType: "hxc_gc_thread";
			case CBRNGcConfigType: "hxc_gc_config";
			case CBRNGcInitializer: "HXC_GC_INITIALIZER";
			case CBRNGcThreadInitializer: "HXC_GC_THREAD_INITIALIZER";
			case CBRNGcInit: "hxc_gc_init";
			case CBRNGcAllocate: "hxc_gc_allocate";
			case CBRNGcDispose: "hxc_gc_dispose";
			case CBRNGcThreadRegister: "hxc_gc_thread_register";
			case CBRNGcThreadUnregister: "hxc_gc_thread_unregister";
			case CBRNArrayCreate: "hxc_array_ref_create";
			case CBRNArrayCreateTrivial: "hxc_array_ref_create_trivial";
			case CBRNArrayInitInPlace: "hxc_array_ref_init_in_place";
			case CBRNArrayDisposeInPlace: "hxc_array_ref_dispose_in_place";
			case CBRNArrayElementOpsType: "hxc_array_element_ops";
			case CBRNArrayGetCopy: "hxc_array_ref_get_copy";
			case CBRNArrayLength: "hxc_array_ref_length";
			case CBRNArrayPushCopy: "hxc_array_ref_push_copy";
			case CBRNArraySetCopy: "hxc_array_ref_set_copy";
			case CBRNArrayRetain: "hxc_array_ref_retain";
			case CBRNArrayRelease: "hxc_array_ref_release";
			case CBRNArrayStoragePushCopy: "hxc_array_push_copy";
			case CBRNArrayValueMember: "value";
			case CBRNStringMapCreate: "hxc_string_map_ref_create";
			case CBRNStringMapRetain: "hxc_string_map_ref_retain";
			case CBRNStringMapRelease: "hxc_string_map_ref_release";
			case CBRNStringMapSetCopy: "hxc_string_map_ref_set_copy";
			case CBRNStringMapExists: "hxc_string_map_ref_exists";
			case CBRNStringMapGetCopy: "hxc_string_map_ref_get_copy";
			case CBRNStringMapRemove: "hxc_string_map_ref_remove";
			case CBRNStringMapClear: "hxc_string_map_ref_clear";
			case CBRNBytesCreateZeroed: "hxc_bytes_ref_create_zeroed";
			case CBRNBytesCreateUtf8Copy: "hxc_bytes_ref_create_utf8_copy";
			case CBRNBytesLength: "hxc_bytes_ref_length";
			case CBRNBytesGet: "hxc_bytes_ref_get";
			case CBRNBytesSet: "hxc_bytes_ref_set";
			case CBRNBytesSub: "hxc_bytes_ref_sub";
			case CBRNBytesBlit: "hxc_bytes_ref_blit";
			case CBRNBytesFill: "hxc_bytes_ref_fill";
			case CBRNBytesCompare: "hxc_bytes_ref_compare";
			case CBRNBytesRetain: "hxc_bytes_ref_retain";
			case CBRNBytesRelease: "hxc_bytes_ref_release";
		});
	}
}
