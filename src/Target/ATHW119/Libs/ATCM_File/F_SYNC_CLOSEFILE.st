(*[[
    UserURL= "ATCMfile:F_SYNC_CLOSEFILE.htm"
]]*)

(* F_SYNC_CLOSEFILE
 * ----------------------------------------------------------------------------
 * Close the given file instance.
 *
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_CLOSEFILE : BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_closeFile)}

END_FUNCTION
