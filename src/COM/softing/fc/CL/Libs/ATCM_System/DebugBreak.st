(*[[
    UserURL= "ATCMFile:DebugBreak.htm"
]]*)
(* 
 *  ATCMControl Standard Function DebugBreak
 *  Purpose: break into the debugger (only if one is attached)
 *)
FUNCTION DebugBreak
VAR_INPUT
    IN : BOOL; (* break only if IN = TRUE *)
END_VAR

;(* add target specific implementaion here *)

END_FUNCTION
