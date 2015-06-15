(*[[
    UserURL= "ATCMFile:StopLocalTask.htm"
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

overload (STRING)  inline {(* add target specific implementaion here *)}
overload (WSTRING) inline {(* add target specific implementaion here *)}

END_FUNCTION
