(*[[
    UserURL= "ATCMfile:COPY_STRING_FROM_BYTE_ARRAY.htm"
]]*)

(* Returns a string copied from the byte array
 * at the position POS containing NUM bytes.
 *)
FUNCTION COPY_STRING_FROM_BYTE_ARRAY : STRING
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_string_from_byte_array ) }

END_FUNCTION
