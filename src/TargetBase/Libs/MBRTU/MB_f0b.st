(*[[
    UserHelpURL= "ATCMfile:MB_f0b.HTML",
    UserText1= "fn_0b_read_event_counter",
]]*)
(*
Return bits:
[0] = status word
[1] = event count
*)
FUNCTION MB_f0b : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
END_VAR
INLINE {STDLIB(MB_f0b)}
END_FUNCTION

