
(*[[
    UserURL= "ATCM:FA_SYNC_WRITEFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_WRITEFILE
 *  Purpose: Writes data to a file.
 *)
FUNCTION FA_SYNC_WRITEFILE:BOOL     (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
    DATA        : BYTE_STRING;      (* data to write *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%3).writeFile(%1, %2)
}
END_FUNCTION
