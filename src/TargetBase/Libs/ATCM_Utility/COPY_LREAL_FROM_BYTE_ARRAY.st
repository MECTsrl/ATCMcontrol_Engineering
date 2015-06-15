(*[[
    UserURL= "ATCMfile:COPY_LREAL_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_LREAL_FROM_BYTE_ARRAY : LREAL
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_lreal_from_byte_array ) }    

END_FUNCTION
