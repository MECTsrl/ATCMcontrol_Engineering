(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:EVT_GetEvent.htm"
]]*)

(* EVT_GetEvent
 * ----------------------------------------------------------------------------
 * Gets the last event whiched caused the execution of the corresponding event 
 * driven task.
 *
 * in   sTask       Task name.
 * out  uEvent      Last task execution event number.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION EVT_GetEvent : WORD

VAR_INPUT
    sTask  : STRING;
END_VAR

VAR_OUTPUT
    uEvent : INT;
END_VAR


inline{STDLIB(EVT_GetEvent)}

END_FUNCTION
