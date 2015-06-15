(*[[
    UserURL= "ATCMFile:COPY_LWORD_TO_BYTE_ARRAY.htm"
]]*)

(* copies the value VALUE to the byte array at the position POS *)

FUNCTION COPY_LWORD_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : LWORD;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_lword_to_byte_array ) }

END_FUNCTION
