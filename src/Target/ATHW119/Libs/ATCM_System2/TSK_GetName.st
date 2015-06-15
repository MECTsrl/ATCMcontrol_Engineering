(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetName.htm"
]]*)

(* TSK_GetName
 * ----------------------------------------------------------------------------
 * Gets the task name of a task identified by its task index number.
 *
 * in   uTask       Task index number.   
 * out  sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_GetName : WORD

VAR_INPUT
    uTask   : INT;
END_VAR

VAR_OUTPUT
    sTask   : STRING;
END_VAR

inline{STDLIB(TSK_GetName)}

END_FUNCTION
