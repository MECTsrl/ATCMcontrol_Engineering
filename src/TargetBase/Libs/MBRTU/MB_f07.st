(*[[
    UserHelpURL= "ATCMfile:MB_f07.HTML",
    UserText1= "fn_07_read_exception_status",
]]*)
(*
Return bits:
[0] = Number of the exception status coils (8);
[1] = exception status  coil1  value (ON/OFF) 
[2] = exception status  coil2  value (ON/OFF) ...
*)
FUNCTION MB_f07 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
END_VAR
INLINE {STDLIB(MB_f07)}
END_FUNCTION
