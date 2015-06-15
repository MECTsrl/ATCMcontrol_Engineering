(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:SYS_Coldstart.htm"
]]*)

(* SYS_Coldstart
 * ----------------------------------------------------------------------------
 * Executes a cold start of the ATCMControl Run Time System.
 *
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION SYS_Coldstart : WORD

inline{STDLIB(SYS_Coldstart)}

END_FUNCTION
