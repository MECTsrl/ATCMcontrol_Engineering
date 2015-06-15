(*[[
    UserHelpURL= "ATCMfile:MB_f03.HTML",
    UserText1= "fn_03_read_holding_register",
]]*)
FUNCTION MB_f03 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
    byteorder  : UINT; (*[byte order]*)
END_VAR
INLINE {STDLIB(MB_f03)}
END_FUNCTION
