(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetInfo.htm"
]]*)

(* TSK_GetInfo
 * ----------------------------------------------------------------------------
 * Gets the task information structure of the corresponding task.
 *
 * in   sTask       Task name.
 * out  TaskInfo    Task information structure of task.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_GetInfo : WORD

VAR_INPUT
    sTask    : STRING;
END_VAR

VAR_OUTPUT
    TaskInfo : TSK_INFO;
END_VAR


inline{STDLIB(TSK_GetInfo)}

END_FUNCTION
