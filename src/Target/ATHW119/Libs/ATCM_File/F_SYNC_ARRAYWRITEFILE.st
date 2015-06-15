(*[[
    UserURL= "ATCMfile:F_SYNC_ARRAYWRITEFILE.htm"
]]*)

(* F_SYNC_ARRAYWRITEFILE
 * ----------------------------------------------------------------------------
 * Writes data from the given array to the specified file.
 *
 * in   DATA        Data to be written.
 * in   POS         Position in array where to store data.
 *                  (It is relative to lower bound (0 <= pos <= (hbound - lowbound).)
 * in   LENGTH      Bytes to be written.
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_ARRAYWRITEFILE : BOOL

VAR_INPUT
    DATA        : ARRAY OF BYTE;
    POS         : DINT;
    LENGTH      : DINT;
END_VAR

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

inline {STDLIB(f_sync_arrayWriteFile)}

END_FUNCTION
