(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:WD_IsEnabled.htm"
]]*)

(* WD_IsEnabled
 * ----------------------------------------------------------------------------
 * Gets the watchdog state (enabled / disabled) for the given task.
 *
 * in   sTask       Task name.
 * ou   bEnabled    Watchdog state (enabled / disabled).
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION WD_IsEnabled : WORD

VAR_INPUT
    sTask    : STRING;
END_VAR

VAR_OUTPUT
    bEnabled : BOOL;
END_VAR

inline{STDLIB(WD_IsEnabled)}

END_FUNCTION
