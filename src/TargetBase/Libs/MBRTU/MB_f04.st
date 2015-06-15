(*[[
    UserHelpURL= "ATCMfile:MB_f04.HTML",
    UserText1= "fn_04_read_input_registers",
]]*)
FUNCTION MB_f04 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
    byteorder  : UINT; (*[byte order]*)
END_VAR
INLINE {STDLIB(MB_f04)}
END_FUNCTION
