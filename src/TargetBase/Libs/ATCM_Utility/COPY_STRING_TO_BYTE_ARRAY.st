(*[[
    UserURL= "ATCMfile:COPY_STRING_TO_BYTE_ARRAY.htm"
]]*)

(* Copies the value VALUE to the byte array BYTES starting at the position POS.
 *)
FUNCTION COPY_STRING_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : STRING;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_string_to_byte_array ) }

END_FUNCTION
