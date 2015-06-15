(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_Exception.htm"
]]*)

(* TSK_Exception
 * ----------------------------------------------------------------------------
 * Raises an exception in the current task.
 *
 * out  uExcepton   Exception index number.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_Exception : WORD

VAR_INPUT
    uException : INT;
END_VAR

inline{STDLIB(TSK_Exception)}

END_FUNCTION
