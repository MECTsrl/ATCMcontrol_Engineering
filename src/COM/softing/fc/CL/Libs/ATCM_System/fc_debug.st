(*[[
    UserURL= "ATCMFile:FC_DEBUG.htm"
]]*)

(* 
 *  ATCMControl Standard Functionblock FC_DEBUG
 *  Purpose: Debug Output
 *)
FUNCTION_BLOCK FC_DEBUG
VAR_INPUT
    ENABLE     : BOOL:=TRUE;  (* enable output with this inst  *)
    STR        : WSTRING;     (* a one-line message to the user*)
END_VAR

IF ENABLE THEN
;(* add target specific implementaion here *)
END_IF;

END_FUNCTION_BLOCK
