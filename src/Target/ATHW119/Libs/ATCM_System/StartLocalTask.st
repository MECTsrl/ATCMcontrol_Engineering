(*[[
    UserURL= "ATCMfile:StartLocalTask.htm"
]]*)
(* 
 *  ATCMControl Standard Function StartLocalTask
 *  Purpose: Put the named task to the running state.
 *           NOTE: name must be given in "lower case".
 *)
FUNCTION StartLocalTask
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline{STDLIB(StartLocalTask)}

END_FUNCTION
