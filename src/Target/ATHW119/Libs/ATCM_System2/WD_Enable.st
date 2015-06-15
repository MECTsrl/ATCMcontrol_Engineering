(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:WD_Enable.htm"
]]*)

(* WD_Enable
 * ----------------------------------------------------------------------------
 * Enables (start) the watchdog for the corresponding task. If the watchdog
 * doesn't already exist, a new watchdog is created.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION WD_Enable : WORD

VAR_INPUT
    sTask    : STRING;
END_VAR


inline{STDLIB(WD_Enable)}

END_FUNCTION
