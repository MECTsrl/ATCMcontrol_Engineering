(*[[
    UserURL= "ATCMFile:COPY_LWORD_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte array at the position POS *)

FUNCTION COPY_LWORD_FROM_BYTE_ARRAY : LWORD
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_lword_from_byte_array ) }

END_FUNCTION
