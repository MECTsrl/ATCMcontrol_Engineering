(*[[
    UserHelpURL= "ATCMfile:MB_f0c.HTML",
    UserText1= "fn_0c_read_event_log",
]]*)
(*
Return bits:
[0] = Number of event (0-64)
[1] = Status Word
[2] = Event Count
[3] = Message Count
[4] = Event 0 byte
[5] = Event 1 byte ....
*)
FUNCTION MB_f0c : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
END_VAR
INLINE {STDLIB(MB_f0c)}
END_FUNCTION
