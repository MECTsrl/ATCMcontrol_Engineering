(*[[
    UserURL= "ATCMfile:F_SYNC_WRITECSVFILE.htm"
]]*)

(* F_SYNC_READCSVFILE
 * ----------------------------------------------------------------------------
 * Writes a record data buffer to CSV file 
 *
 * in   FIELDSTRUCT Maximum record field number and record field 
 *                  format for all file records.
 * in   DATA        Data to be written.
 * io   FILE        File instance to operate on.
 * ret              Error flag: FALSE if successful - TRUE if failed.
 *)
FUNCTION F_SYNC_WRITECSVFILE: BOOL

VAR_IN_OUT
    FILE        : F_SYNC_FILE;
END_VAR

VAR_INPUT
    FIELDSTRUCT : ARRAY OF BYTE;
    ACTRECORD   : DINT;
    DATA        : ARRAY OF BYTE;
END_VAR

inline {STDLIB(f_sync_writeCsvFile)}


END_FUNCTION
