(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:SYS_Reboot.htm"
]]*)

(* SYS_Reboot
 * ----------------------------------------------------------------------------
 * Reboots the controller hardware. (Only if supported on the corresponding
 * hardware environment!)
 *
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION SYS_Reboot : WORD

inline{STDLIB(SYS_Reboot)}

END_FUNCTION
