
(*[[
    UserURL= "ATCM:FA_SYNC_CLOSEFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_CLOSEFILE
 *  Purpose: Closes a file after read, write or getsize operation.
 *)
FUNCTION FA_SYNC_CLOSEFILE:BOOL     (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).closeFile(%1)
}
END_FUNCTION
