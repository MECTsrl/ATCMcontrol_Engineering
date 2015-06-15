(*[[
    UserText1= "fn_17_read_write_registers",
    UserHelpURL= "ATCMfile:MB_f17.HTML",
]]*)
FUNCTION MB_f17 : UINT
(*
Return bits:

*)
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
    count      : UINT; (*[counter]*)
    reg_addr   : UINT; (*[register address]*)
    byteorder  : BYTE; (*[byte order]*)
    data : ARRAY[0..257] of UINT; (*[value to write]*)
END_VAR
INLINE {STDLIB(MB_f17)}
END_FUNCTION
