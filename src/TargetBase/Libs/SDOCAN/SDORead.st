(*[[
    UserHelpURL= "ATCMfile:SDORead.HTML",
]]*)
FUNCTION SDORead : UINT
VAR_INPUT
	channel : UINT; (*[channel number]*)
	varname : ARRAY[0..256] of BYTE; (*[variable name]*)
END_VAR
INLINE {STDLIB(SDORead)}
END_FUNCTION
