(*[[
    UserHelpURL= "ATCMfile:Usb_mkdir.HTML",
]]*)
FUNCTION Usb_mkdir : UINT
VAR_INPUT
	dirname : STRING;
	index : UINT;
END_VAR
	INLINE {STDLIB(Usb_mkdir)}
END_FUNCTION
