(*[[
    UserURL= "ATCMfile:F_SYNC_READCSVFILE.htm"
]]*)

(* F_SYNC_READCSVFILE
 * ----------------------------------------------------------------------------
 * Reads a record data from CSV file.
 *
 * in   FIELDSTRUCT specifies the maximum record field number and record field 
 *                  format for all file records.
 * io   FILE        File instance to operate on.
 * out  DATA        Data.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_READCSVFILE: BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

VAR_INPUT
    FIELDSTRUCT : ARRAY OF BYTE;
    ACTRECORD   : DINT;
END_VAR

VAR_OUTPUT
    DATA        : ARRAY OF BYTE;
END_VAR

inline {STDLIB(f_sync_readCsvFile)}


END_FUNCTION
