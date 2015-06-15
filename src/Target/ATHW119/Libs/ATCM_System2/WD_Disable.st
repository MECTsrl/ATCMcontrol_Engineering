(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:WD_Disable.htm"
]]*)

(* WD_Disable
 * ----------------------------------------------------------------------------
 * Disables (cancels) a watchdog for the corresponding task.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION WD_Disable : WORD

VAR_INPUT
    sTask : STRING;
END_VAR


inline{STDLIB(WD_Disable)}

END_FUNCTION
