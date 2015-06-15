
(*[[
    UserURL= "ATCM:GetLocalTaskState.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskState
 *  Purpose: get the current task execution state of the named task.
 *
 *  possible return values are:
 *  STATE_ERROR       = -1
 *  STATE_ONCREATION  = 0
 *  STATE_STOPPED     = 1
 *  STATE_RUNNING     = 2
 *  STATE_BREAK       = 3
 *)
FUNCTION GetLocalTaskState : DINT
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload DINT(WSTRING) inline{((COM.softing.fc.CC.system.ITask)globals.getObject(%1.toLowerCase())).itGetTaskState()}

END_FUNCTION
