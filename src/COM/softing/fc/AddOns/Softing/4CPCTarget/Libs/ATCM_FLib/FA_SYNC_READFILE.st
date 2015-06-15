
(*[[
    UserURL= "ATCM:FA_SYNC_READFILE.htm"
]]*)

(* 
 *  ATCMControl FileAccess Function FA_SYNC_READFILE
 *  Purpose: Reads data from a file.
 *)
FUNCTION FA_SYNC_READFILE:BOOL      (* Return Value: error flag *)
VAR_INPUT
    EXEC        : BOOL;             (* start operation *)
END_VAR
VAR_IN_OUT
    FILE        : FA_SYNC_FILE;     (* file instance to operate on *)
    LENGTH      : DINT;             (* bytes to read; return read bytes *)
END_VAR
VAR_OUTPUT
    DATA        : BYTE_STRING;      (* read data *)
END_VAR

inline
{
    ((COM.softing.fclib.flib.SyncFLib)%2).readFile(%1, %3, %4)
}
END_FUNCTION
