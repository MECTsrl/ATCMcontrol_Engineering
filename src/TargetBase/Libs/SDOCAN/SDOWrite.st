(*[[
    UserHelpURL= "ATCMfile:SDOWrite.HTML",
]]*)
FUNCTION SDOWrite : UINT
VAR_INPUT
	channel : UINT; (*[channel number]*)
	varname : ARRAY[0..256] of BYTE; (*[variable name]*)
	data : UDINT; (*[data to write]*)
END_VAR
INLINE {STDLIB(SDOWrite)}
END_FUNCTION
