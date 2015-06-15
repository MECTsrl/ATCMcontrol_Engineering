(*[[
    UserHelpURL= "ATCMfile:MB_f02.HTML",
    UserText1= "fn_02_read_input_status",
]]*)
FUNCTION MB_f02 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
END_VAR
INLINE {STDLIB(MB_f02)}
END_FUNCTION
