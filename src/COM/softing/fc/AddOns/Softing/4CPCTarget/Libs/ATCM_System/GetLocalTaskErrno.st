
(*[[
    UserURL= "ATCM:GetLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskErrno
 *  Purpose: get the task error number of the named task.
 *
 *  possible return values are (among others):
 *  NO_ERROR            = 0
 *  DEADLOCK_DETECTED   = 2
 *  RUNTIME_EXCEPTION   = 3
 *)
FUNCTION GetLocalTaskErrno : DINT
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload DINT(WSTRING) inline{((COM.softing.fc.CC.system.ITask)globals.getObject(%1.toLowerCase())).itGetTaskErrNo()}

END_FUNCTION
