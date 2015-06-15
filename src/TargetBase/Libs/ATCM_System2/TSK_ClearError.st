(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_ClearError.htm"
]]*)

(* TSK_ClearError
 * ----------------------------------------------------------------------------
 * Clears the task error number of the corresponding task.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_ClearError : WORD

VAR_INPUT
    sTask : STRING;
END_VAR


inline{STDLIB(TSK_ClearError)}

END_FUNCTION
