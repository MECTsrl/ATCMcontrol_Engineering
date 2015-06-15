(*[[
    UserURL= "ATCMFile:GetLocalTaskState.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskState
 *  Purpose: get the current task execution state of the named task.
 *
 *  Possible return values are (see system.cst):
 *  CONST
 *     FC_TASK_STATE_ERROR       : DINT :=-1;
 *     FC_TASK_STATE_ONCREATION  : DINT := 0;
 *     FC_TASK_STATE_STOPPED     : DINT := 1;
 *     FC_TASK_STATE_RUNNING     : DINT := 2;
 *     FC_TASK_STATE_BREAK       : DINT := 3;
 *  END_CONST
 *)
FUNCTION GetLocalTaskState : DINT
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline {(* add target specific implementaion here *)}
overload (WSTRING) inline {(* add target specific implementaion here *)}

END_FUNCTION
