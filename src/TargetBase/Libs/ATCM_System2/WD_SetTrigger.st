(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:WD_SetTrigger.htm"
]]*)

(* WD_SetTrigger
 * ----------------------------------------------------------------------------
 * Sets the watchdog trigger level of the corresponding task.
 *
 * in   sTask       Task name.
 * in   WDTrigger   Watchdog trigger level.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION WD_SetTrigger : WORD

VAR_INPUT
    sTask       : STRING;
END_VAR

VAR_INPUT
    WDTrigger   : UDINT;
END_VAR

inline{STDLIB(WD_SetTrigger)}

END_FUNCTION
