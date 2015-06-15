(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:SYS_RetainWrite.htm"
]]*)

(* SYS_RetainWrite
 * ----------------------------------------------------------------------------
 * Writes the IEC retain segment to the file system. (Not supported on all 
 * targets and only if no real physical retentive memory is available.)
 *
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION SYS_RetainWrite : WORD

inline{STDLIB(SYS_RetainWrite)}

END_FUNCTION
