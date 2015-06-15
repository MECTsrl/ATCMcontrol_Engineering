(*[[
    UserURL= "ATCMfile:F_SYNC_OPENFILE.htm"
]]*)

(* F_SYNC_OPENFILE
 * ----------------------------------------------------------------------------
 * Opens the given file.
 *
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_OPENFILE : BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_openFile)}

END_FUNCTION
