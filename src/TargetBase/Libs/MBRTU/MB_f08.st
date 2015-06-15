(*[[
    UserHelpURL= "ATCMfile:MB_f08.HTML",
    UserText1= "fn_08_diagnostics",
]]*)
FUNCTION MB_f08 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
	subfunction: UINT; (*[sub-function pointer to test]*)
END_VAR
INLINE {STDLIB(MB_f08)}
END_FUNCTION
