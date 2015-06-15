
(*[[
    UserURL= "ATCM:OutputDebugString.htm"
]]*)
(* 
 *  ATCMControl Standard Functionblock OutputDebugString
 *  Purpose: Write a string to the control's standard output
 *           and the log file. A new line character is added 
 *           automatically.
 *)
FUNCTION OutputDebugString
VAR_INPUT
    STR        : ANY_STRING;     (* a one-line message to the user*)
END_VAR

overload (wstring) inline{DebugManager.print(%1 + "\n")}
(* TODO : string *)

END_FUNCTION
