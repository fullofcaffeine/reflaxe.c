class PrimitiveReference {
	public function new() {}
}

extern class PrimitiveFixture {
	var boolValue:Bool;
	var intValue:Int;
	var uintValue:UInt;
	var floatValue:Float;
	var int8Value:c.Int8;
	var int16Value:c.Int16;
	var int32Value:c.Int32;
	var int64Value:c.Int64;
	var uint8Value:c.UInt8;
	var uint16Value:c.UInt16;
	var uint32Value:c.UInt32;
	var uint64Value:c.UInt64;
	var sizeValue:c.Size;
	var ptrDiffValue:c.PtrDiff;
	var intPtrValue:c.IntPtr;
	var uintPtrValue:c.UIntPtr;
	var nullableBool:Null<Bool>;
	var nullableInt:Null<Int>;
	var nullableUInt:Null<UInt>;
	var nullableFloat:Null<Float>;
	var nullableInt8:Null<c.Int8>;
	var referenceValue:PrimitiveReference;
	var nullableReference:Null<PrimitiveReference>;
	var pointerValue:c.Ptr<c.Int8>;
	var nullablePointer:c.NullablePtr<c.Int8>;
	var nullWrappedPointer:Null<c.Ptr<c.Int8>>;
	function returnsVoid():Void;
}
