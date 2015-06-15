(*[[
    UserURL= "ATCMfile:F_SYNC_READLINE.htm"
]]*)

(* F_SYNC_READLINE
 * ----------------------------------------------------------------------------
 * Reads a line from a file into a string.
 *
 * io   FILE        File instance to operate on.
 * out  DATA        Data.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_READLINE : BOOL
VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR
VAR_OUTPUT
    DATA        : STRING;
END_VAR

INLINE { STDLIB(f_sync_readLine) }

END_FUNCTION
