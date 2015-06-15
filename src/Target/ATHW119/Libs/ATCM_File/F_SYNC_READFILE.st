(*[[
    UserURL= "ATCMfile:F_SYNC_READFILE.htm"
]]*)

(* F_SYNC_READFILE
 * ----------------------------------------------------------------------------
 * Reads data from a file into a string.
 *
 * io   FILE        File instance to operate on.
 * io   LENGTH      in: bytes to read   out: bytes read
 * out  DATA        Data.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_READFILE : BOOL
VAR_IN_OUT
    FILE        : F_SYNC_FILE;
    LENGTH      : DINT;
END_VAR
VAR_OUTPUT
    DATA        : STRING;
END_VAR

inline {STDLIB(f_sync_readFile)}

END_FUNCTION
