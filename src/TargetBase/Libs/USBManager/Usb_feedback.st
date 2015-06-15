(*[[
    UserHelpURL= "ATCMfile:Usb_feedback.HTML",
]]*)
FUNCTION Usb_feedback : UINT
VAR_OUTPUT
	USBfeedback : ARRAY[0..2] of INT;
END_VAR
	INLINE {STDLIB(Usb_feedback)}
END_FUNCTION
