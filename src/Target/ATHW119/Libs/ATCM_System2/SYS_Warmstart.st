(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:SYS_Warmstart.htm"
]]*)

(* SYS_Warmstart
 * ----------------------------------------------------------------------------
 * Executes a warm start of the ATCMControl Run Time System.
 *
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION SYS_Warmstart : WORD

inline{STDLIB(SYS_Warmstart)}

END_FUNCTION
