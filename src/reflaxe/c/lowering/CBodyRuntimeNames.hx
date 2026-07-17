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
	CBRNStatusOk;
	CBRNAbort;
}

class CBodyRuntimeNames {
	public static function identifier(name:CBodyRuntimeName):CIdentifier {
		return new CIdentifier(switch name {
			case CBRNStringType: "hxc_string";
			case CBRNPrintln: "hxc_io_println";
			case CBRNStatusOk: "HXC_STATUS_OK";
			case CBRNAbort: "abort";
		});
	}
}
