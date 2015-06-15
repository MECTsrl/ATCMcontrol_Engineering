(*[[
    UserHelpURL= "ATCMfile:Usb_copy.HTML",
]]*)
FUNCTION Usb_copy : UINT
VAR_INPUT
	srcfile : STRING;
	dstfile : STRING;
	src_index : UINT;
	dst_index : UINT;
END_VAR
	INLINE {STDLIB(Usb_copy)}
END_FUNCTION
