
(*[[
    UserURL= "ATCM:FA_SYNC_CREATEDIRECTORY.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_CREATEDIRECTORY
 *  Purpose: Create a new directory.
 *)
FUNCTION FA_SYNC_CREATEDIRECTORY:BOOL   (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;                 (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;         (* file instance to operate on *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).createDir(%1)
}
END_FUNCTION
