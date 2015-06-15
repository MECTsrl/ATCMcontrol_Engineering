(*[[
    UserURL= "ATCMfile:F_SYNC_DELETEFILE.htm"
]]*)

(* F_SYNC_DELETEFILE
 * ----------------------------------------------------------------------------
 * Deletes the specified file.
 *
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_DELETEFILE : BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_deleteFile)}

END_FUNCTION
