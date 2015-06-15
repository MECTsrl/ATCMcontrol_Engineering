(*[[
    UserURL= "ATCMfile:F_SYNC_ARRAYREADFILE.htm"
]]*)

(* F_SYNC_ARRAYREADFILE
 * ----------------------------------------------------------------------------
 * Reads data from a file into an byte array.
 *
 * in   POS         Position in array where to store data.
 *                  (It is relative to lower bound (0 <= pos <= (hbound - lowbound).)
 * io   FILE        File instance to operate on.
 * io   LENGTH      in: bytes to read   out: bytes read
 * out  DATA        Data.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_ARRAYREADFILE : BOOL
VAR_INPUT
    POS         : DINT;
END_VAR
VAR_IN_OUT
    FILE        : F_SYNC_FILE;
    LENGTH      : DINT;
END_VAR
VAR_OUTPUT
    DATA        : ARRAY OF BYTE;
END_VAR

inline {STDLIB(f_sync_arrayReadFile)}

END_FUNCTION
