(*[[
    UserText1= "fn_06_preset_single_register",
    UserHelpURL= "ATCMfile:MB_f06.HTML",
]]*)
FUNCTION MB_f06 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    reg_addr   : UINT; (*[register address]*)
	value      : UINT; (*[value to set]*)
END_VAR
INLINE {STDLIB(MB_f06)}
END_FUNCTION
