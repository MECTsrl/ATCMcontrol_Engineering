(*[[
    UserURL= "ATCMfile:StopLocalTask.htm"
]]*)
(* 
 *  ATCMControl Standard Function StopLocalTask
 *  Purpose: Put the named task to the stopped state.
 *           NOTE: name must be given in "lower case".
 *  WARNING: never try to stop the own task.
 *)
FUNCTION StopLocalTask
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline{STDLIB(StopLocalTask)}

END_FUNCTION
