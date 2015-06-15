
(*[[
    UserURL= "ATCM:FA_SYNC_EXISTSFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_EXISTSFILE
 *  Purpose: Determines whether a file exists.
 *)
FUNCTION FA_SYNC_EXISTSFILE:BOOL    (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
END_VAR
VAR_OUTPUT
    EXISTS      : BOOL;             (* TRUE if files exists, else FALSE *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).existsFile(%1, %3)
}
END_FUNCTION
