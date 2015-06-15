(*[[
    UserURL= "ATCMfile:COPY_LREAL_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_REAL_FROM_BYTE_ARRAY : REAL
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_real_from_byte_array ) }    

END_FUNCTION
