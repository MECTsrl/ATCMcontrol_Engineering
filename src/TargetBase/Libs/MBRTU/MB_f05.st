(*[[
    UserHelpURL= "ATCMfile:MB_f05.HTML",
    UserText1= "fn_05_force_single_coil",
]]*)
FUNCTION MB_f05 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    coil_addr  : UINT; (*[coil address]*)
    coil_state : UINT; (*[coil state]*)
END_VAR
INLINE {STDLIB(MB_f05)}
END_FUNCTION
