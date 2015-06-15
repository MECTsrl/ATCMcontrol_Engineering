(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_Stop.htm"
]]*)

(* TSK_Stop
 * ----------------------------------------------------------------------------
 * Stops the corresponding task.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_Stop : WORD

VAR_INPUT
    sTask : STRING;
END_VAR


inline{STDLIB(TSK_Stop)}

END_FUNCTION
