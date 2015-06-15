(*[[
    UserHelpURL= "ATCMfile:Usb_delete.HTML",
]]*)
FUNCTION Usb_delete : UINT
VAR_INPUT
	file : STRING;
	index : UINT;
END_VAR
	INLINE {STDLIB(Usb_delete)}
END_FUNCTION
