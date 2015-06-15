(*[[
    UserURL= "ATCMfile:F_SYNC_WRITELINE.htm"
]]*)

(* F_SYNC_WRITELINE
 * ----------------------------------------------------------------------------
 * Writes a line to a file from a string.
 *
 * in   DATA        Data to be written.
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_WRITELINE : BOOL
VAR_INPUT
    DATA        : STRING;
END_VAR
VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

INLINE { STDLIB(f_sync_writeLine) }

END_FUNCTION
