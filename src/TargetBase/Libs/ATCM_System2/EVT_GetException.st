(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:EVT_GetException.htm"
]]*)

(* EVT_GetException
 * ----------------------------------------------------------------------------
 * Gets the task number and exception number of the last raised task exception.
 * (To be used as an exception event handler.)
 *
 * out  uTask       Task index number of last exception.
 * out  ulException Exception number.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION EVT_GetException : WORD

VAR_OUTPUT
    uTask       : INT;
    ulException : DINT;
END_VAR


inline{STDLIB(EVT_GetException)}

END_FUNCTION
