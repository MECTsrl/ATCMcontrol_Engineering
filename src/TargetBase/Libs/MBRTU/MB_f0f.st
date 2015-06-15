(*[[
    UserHelpURL= "ATCMfile:MB_f0f.HTML",
    UserText1= "fn_0f_force_multiple_coil",
]]*)
FUNCTION MB_f0f : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    coil_addr  : UINT; (*[coil address]*)
    coil_count : UINT; (*[coil counter]*)
    values : ARRAY[0..257] of UINT; (*[coils values to force]*)
END_VAR
INLINE {STDLIB(MB_f0f)}
END_FUNCTION
