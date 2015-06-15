(*[[
    UserHelpURL= "ATCMfile:Usb_diskcopy.HTML",
]]*)
FUNCTION Usb_diskcopy : UINT
VAR_INPUT
	srcfile : STRING;
	dstfile : STRING;
	index : UINT;
END_VAR
	INLINE {STDLIB(Usb_diskcopy)}
END_FUNCTION
