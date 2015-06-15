(*[[
    UserURL= "ATCMfile:F_SYNC_GETSIZE.htm"
]]*)

(* F_SYNC_GETSIZE
 * ----------------------------------------------------------------------------
 * Gets the size of the given file.
 *
 * io   FILE        File instance to operate on.
 * out  SIZE        File size of given file.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_GETSIZE : BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

VAR_OUTPUT
    SIZE        : DINT;
END_VAR

inline {STDLIB(f_sync_getSize)}

END_FUNCTION
