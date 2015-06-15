(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_Start.htm"
]]*)

(* TSK_Start
 * ----------------------------------------------------------------------------
 * Starts the corresponding task.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_Start : WORD

VAR_INPUT
    sTask : STRING;
END_VAR


inline{STDLIB(TSK_Start)}

END_FUNCTION
