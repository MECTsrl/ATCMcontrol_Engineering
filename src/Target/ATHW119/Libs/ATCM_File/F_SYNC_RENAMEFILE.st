(*[[
    UserURL= "ATCMfile:F_SYNC_RENAMEFILE.htm"
]]*)

(* F_SYNC_RENAMEFILE
 * ----------------------------------------------------------------------------
 * Renames the given file.
 *
 * in   NEWNAME     New file name.
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_RENAMEFILE : BOOL

VAR_INPUT
    NEWNAME     : STRING;
END_VAR

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_renameFile)}

END_FUNCTION
