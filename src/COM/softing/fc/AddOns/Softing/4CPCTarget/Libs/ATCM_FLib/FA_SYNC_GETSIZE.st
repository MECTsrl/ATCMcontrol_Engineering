
(*[[
    UserURL= "ATCM:FA_SYNC_GETSIZE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_GETSIZE
 *  Purpose: Retrieves the length in bytes of a file.
 *)
FUNCTION FA_SYNC_GETSIZE:BOOL       (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
END_VAR
VAR_OUTPUT
    SIZE        : LINT;             (* file size in bytes *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).getSize(%1, %3)
}
END_FUNCTION
