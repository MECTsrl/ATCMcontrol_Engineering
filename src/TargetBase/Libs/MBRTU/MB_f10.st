(*[[
    UserText1= "fn_10_preset_multiple_registers",
    UserHelpURL= "ATCMfile:MB_f10.HTML",
]]*)
FUNCTION MB_f10 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
    value : ARRAY[0..257] of UINT; (*[registers value to set]*)
END_VAR
INLINE {STDLIB(MB_f10)}
END_FUNCTION
