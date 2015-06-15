(*[[
    UserText1= "fn_11_report_slave_id",
    UserHelpURL= "ATCMfile:MB_f11.HTML",
]]*)
(*
Return bits:
[0] = # of the received byte, each of them is stored in the following array location
      in the same arrangement they are received
[i] = data byte
*)
FUNCTION MB_f11 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
END_VAR
INLINE {STDLIB(MB_f11)}
END_FUNCTION
