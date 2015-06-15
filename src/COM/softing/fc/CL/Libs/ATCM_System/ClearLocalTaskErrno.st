(*[[
    UserURL= "ATCMFile:ClearLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function ClearLocalTaskErrno
 *  Purpose: Clears the error state of the given task.
 *)
FUNCTION ClearLocalTaskErrno
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline {(* add target specific implementaion here *)}
overload (WSTRING) inline {(* add target specific implementaion here *)}

END_FUNCTION
