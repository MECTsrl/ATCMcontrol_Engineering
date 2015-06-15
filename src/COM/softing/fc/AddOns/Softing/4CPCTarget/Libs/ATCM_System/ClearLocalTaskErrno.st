
(*[[
    UserURL= "ATCM:ClearLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function ClearLocalTaskErrno
 *  Purpose: Clears the error state of the given task.
 *)
FUNCTION ClearLocalTaskErrno
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (WSTRING) inline{((COM.softing.fc.CC.system.ITask)globals.getObject(%1.toLowerCase())).setErrorNumber(0)}

END_FUNCTION
