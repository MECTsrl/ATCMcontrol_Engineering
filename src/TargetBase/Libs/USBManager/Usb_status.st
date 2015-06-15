(*[[
    UserHelpURL= "ATCMfile:Usb_status.HTML",
]]*)
FUNCTION Usb_status : UINT
VAR_OUTPUT
	USBstatus : ARRAY[0..5] of INT;
END_VAR
	INLINE {STDLIB(Usb_status)}
END_FUNCTION
