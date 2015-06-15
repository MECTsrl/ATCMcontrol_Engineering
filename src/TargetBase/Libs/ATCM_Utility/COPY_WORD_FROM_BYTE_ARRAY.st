(*[[
    UserURL= "ATCMfile:COPY_WORD_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte array BYTES at the position POS *)

FUNCTION COPY_WORD_FROM_BYTE_ARRAY : WORD
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR
    
inline { STDLIB ( copy_word_from_byte_array ) }

END_FUNCTION
