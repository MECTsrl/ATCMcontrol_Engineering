
(*[[
    UserURL= "ATCM:StopLocalTask.htm"
]]*)
(* 
 *  ATCMControl Standard Function StopLocalTask
 *  Purpose: Put the named task to the stopped state.
 *  WARNING: never try to stop the own task.
 *)
FUNCTION StopLocalTask
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (WSTRING) inline{((COM.softing.fc.CC.system.ITask)globals.getObject(%1.toLowerCase())).itStopTask()}

END_FUNCTION
