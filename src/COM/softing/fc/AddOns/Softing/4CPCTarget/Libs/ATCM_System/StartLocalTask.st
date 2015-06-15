
(*[[
    UserURL= "ATCM:StartLocalTask.htm"
]]*)
(* 
 *  ATCMControl Standard Function StartLocalTask
 *  Purpose: Put the named task to the running state.
 *)
FUNCTION StartLocalTask
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (WSTRING) inline{((COM.softing.fc.CC.system.ITask)globals.getObject(%1.toLowerCase())).itStartTask()}

END_FUNCTION
