(*[[
    UserURL= "ATCMfile:F_SYNC_WRITEFILE.htm"
]]*)

(* F_SYNC_WRITEFILE
 * ----------------------------------------------------------------------------
 * Writes data from the given string to the specified file.
 *
 * in   DATA        Data to be written.
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_WRITEFILE : BOOL
VAR_INPUT
    DATA        : STRING;
END_VAR
VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_writeFile)}

END_FUNCTION
