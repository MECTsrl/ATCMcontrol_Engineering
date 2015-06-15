(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:EVT_Set.htm"
]]*)

(* EVT_SET
 * ----------------------------------------------------------------------------
 * Sets a givent event.
 *
 * in   uEvent      Event to be set.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION EVT_Set : WORD

VAR_INPUT
    uEvent       : INT;
END_VAR


inline{STDLIB(EVT_Set)}

END_FUNCTION
