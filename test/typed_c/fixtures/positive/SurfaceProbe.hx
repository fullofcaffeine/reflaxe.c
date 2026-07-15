package;

/** Compile-only coverage for every M0 `c.*` contract type. */
extern class SurfaceProbe {
	public static var ptr:c.Ptr<c.UInt8>;
	public static var constPtr:c.ConstPtr<c.UInt8>;
	public static var nullablePtr:c.NullablePtr<c.UInt8>;
	public static var ref:c.Ref<c.UInt8>;
	public static var constRef:c.ConstRef<c.UInt8>;
	public static var restrictPtr:c.RestrictPtr<c.UInt8>;
	public static var volatilePtr:c.VolatilePtr<c.UInt8>;
	public static var functionPtr:c.FunctionPtr<c.Int32->Void>;
	public static var span:c.Span<c.UInt8>;
	public static var constSpan:c.ConstSpan<c.UInt8>;
	public static var array:c.CArray<c.UInt8, Length16>;
	public static var cString:c.CString;
	public static var stringView:c.StringView;
	public static var volatileValue:c.Volatile<c.UInt32>;
	public static var atomic:c.Atomic<c.UInt32>;
	public static var owned:c.Owned<c.UInt8>;
	public static var borrowed:c.Borrowed<c.UInt8>;
	public static var result:c.Result<c.Int32, c.UInt32>;
	public static var allocator:c.Allocator;
	public static var arena:c.Arena;
	public static var int8:c.Int8;
	public static var uint8:c.UInt8;
	public static var int16:c.Int16;
	public static var uint16:c.UInt16;
	public static var int32:c.Int32;
	public static var uint32:c.UInt32;
	public static var int64:c.Int64;
	public static var uint64:c.UInt64;
	public static var size:c.Size;
	public static var ptrDiff:c.PtrDiff;
	public static var intPtr:c.IntPtr;
	public static var uintPtr:c.UIntPtr;
	public static var header:c.Header;
	public static var includeKind:c.IncludeKind;
	public static var layout:c.Layout;
	public static var linkage:c.Linkage;
	public static var callingConvention:c.CallingConvention;
	public static var visibility:c.Visibility;
	public static var memoryOrder:c.MemoryOrder;
	public static var unsafeBoundary:c.Unsafe;
	public static var syntaxBoundary:c.Syntax;
}
