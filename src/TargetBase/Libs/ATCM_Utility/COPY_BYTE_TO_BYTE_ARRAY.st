(*[[
    UserURL= "ATCMfile:COPY_BYTE_TO_BYTE_ARRAY.htm"
]]*)

(* copies the value VALUE to the byte array BYTES at the position POS *)

FUNCTION COPY_BYTE_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : BYTE;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_byte_to_byte_array ) }

END_FUNCTION
