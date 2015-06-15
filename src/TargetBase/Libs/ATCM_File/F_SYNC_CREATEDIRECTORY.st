(*[[
    UserURL= "ATCMfile:F_SYNC_CREATEDIRECTORY.htm"
]]*)

(* F_SYNC_CREATEDIRECTORY
 * ----------------------------------------------------------------------------
 * Creates a new directory.
 *
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_CREATEDIRECTORY : BOOL 

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_createDirectory)}

END_FUNCTION
