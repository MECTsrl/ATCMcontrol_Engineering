
(*[[
    UserURL= "ATCM:FA_SYNC_OPENFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_OPENFILE
 *  Purpose: Opens a file for read, write or getsize operations.
 *)
FUNCTION FA_SYNC_OPENFILE:BOOL      (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).createFile(%1)
}
END_FUNCTION
