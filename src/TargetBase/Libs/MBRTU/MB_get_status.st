(*[[
    UserHelpURL= "ATCMfile:MB_get_status.HTML",
]]*)
FUNCTION MB_get_status : UINT
VAR_OUTPUT
    data : ARRAY[0..2] of UINT; (*[return value]*)
END_VAR
INLINE {STDLIB(MB_get_status)}
END_FUNCTION
