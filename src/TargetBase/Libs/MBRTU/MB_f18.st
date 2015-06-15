(*[[
    UserText1= "fn_18_read_fifo_queue",
    UserHelpURL= "ATCMfile:MB_f18.HTML",
]]*)
(*
Return bits:
all
*)
FUNCTION MB_f18 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
END_VAR
INLINE {STDLIB(MB_f18)}
END_FUNCTION
