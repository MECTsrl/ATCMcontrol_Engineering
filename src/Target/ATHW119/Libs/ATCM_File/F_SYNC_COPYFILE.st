(*[[
    UserURL= "ATCMfile:F_SYNC_COPYFILE.htm"
]]*)

(* F_SYNC_COPYFILE
 * ----------------------------------------------------------------------------
 * Copies file from source pathname to destination pathname.
 *
 * in   SrcPathName        Source pathname
 * in   DestPathName       Destination pathname
 *)
FUNCTION F_SYNC_COPYFILE : DINT

VAR_INPUT
SrcPathName  :   STRING;      
DestPathName :   STRING;      
END_VAR


inline {STDLIB(f_sync_copyFile)}

END_FUNCTION
