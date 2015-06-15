(*[[
    UserURL= "ATCMfile:F_SYNC_EXISTSFILE.htm"
]]*)

(* F_SYNC_EXISTSFILE
 * ----------------------------------------------------------------------------
 * Tests if the given file exists.
 *
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_EXISTSFILE : BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_existsFile)}

END_FUNCTION
