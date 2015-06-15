(*[[
    UserURL= "ATCMfile:F_GET_DISKFREESPACE.htm"
]]*)

(* F_GET_DISKFREESPACE
 * ----------------------------------------------------------------------------
 * Retrieves the free disk space in kilo bytes for specified path.
 *
 * in   PATH        Path to the disk.
 * out  FREE_KB     Free disk space in kilo bytes.
 * ret              Error code              
 *)
FUNCTION F_GET_DISKFREESPACE  : DINT

VAR_INPUT
    PATH : STRING;
END_VAR

VAR_OUTPUT
    FREE_KB : DINT;
END_VAR

inline {STDLIB(f_get_diskFreeSpace)}

END_FUNCTION
