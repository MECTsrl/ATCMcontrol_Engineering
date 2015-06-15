(*[[
    UserHelpURL= "ATCMfile:MB_data.HTML",
]]*)
FUNCTION MB_data : UINT
VAR_OUTPUT
	data : ARRAY[0..257] OF UINT; (*[return value]*)
END_VAR
INLINE {STDLIB(MB_data)}
END_FUNCTION
