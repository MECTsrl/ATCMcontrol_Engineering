(*[[
    UserURL= "ATCMFile:COPY_BYTE_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte array at the position POS *)

FUNCTION COPY_BYTE_FROM_BYTE_ARRAY : BYTE
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_byte_from_byte_array ) }

END_FUNCTION
