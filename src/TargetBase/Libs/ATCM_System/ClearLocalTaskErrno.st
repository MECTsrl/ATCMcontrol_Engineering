(*[[
    UserURL= "ATCMfile:ClearLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function ClearLocalTaskErrno
 *  Purpose: Clears the error state of the given task.
 *)
FUNCTION ClearLocalTaskErrno
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline{STDLIB(ClearLocalTaskErrno)}

END_FUNCTION
