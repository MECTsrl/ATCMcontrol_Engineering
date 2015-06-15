(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:WD_GetTrigger.htm"
]]*)

(* WD_GetTrigger
 * ----------------------------------------------------------------------------
 * Gets the watchdog trigger level and the actual watchdog trigger count of 
 * the corresponding task.
 *
 * in   sTask       Task name.
 * out  WDTrigger   Watchdog trigger level.
 * out  WDCount     Actual watchdog trigger count.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION WD_GetTrigger : WORD

VAR_INPUT
    sTask     : STRING;
END_VAR

VAR_OUTPUT
    WDTrigger   : UDINT;
    WDCount     : UDINT;
END_VAR

inline{STDLIB(WD_GetTrigger)}

END_FUNCTION
