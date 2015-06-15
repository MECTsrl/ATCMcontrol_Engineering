(*[[
    UserHelpURL= "ATCMfile:SDOGetStatus.HTML",
]]*)
FUNCTION SDOGetStatus : UINT
VAR_OUTPUT
	status : UINT; (*[status 0: not ready; <> 0 : ready]*)
END_VAR
INLINE {STDLIB(SDOGetStatus)}
END_FUNCTION
