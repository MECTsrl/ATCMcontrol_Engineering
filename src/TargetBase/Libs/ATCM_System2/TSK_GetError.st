(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetError.htm"
]]*)

(* TSK_GetError
 * ----------------------------------------------------------------------------
 * Gets the task error state of the corresponding task.
 *
 * in   sTask       Task name.
 * out  ulError     Task error state. (See ErrNo.cst for possible values.)
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_GetError : WORD

VAR_INPUT
    sTask   : STRING;
END_VAR

VAR_OUTPUT
    ulError : DINT;
END_VAR


inline{STDLIB(TSK_GetError)}

END_FUNCTION
