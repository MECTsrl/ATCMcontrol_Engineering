(*[[
    UserHelpURL= "ATCMfile:MB_f01.HTML",
    UserText1= "fn_01_read_coil_status",
]]*)
FUNCTION MB_f01 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
END_VAR
INLINE {STDLIB(MB_f01)}
END_FUNCTION
