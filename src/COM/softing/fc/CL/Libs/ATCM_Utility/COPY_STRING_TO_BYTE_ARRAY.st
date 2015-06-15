(*[[
    UserURL= "ATCMFile:COPY_STRING_TO_BYTE_ARRAY.htm"
]]*)

(* Copies the value VALUE to the byte array starting at the position POS.
 * The unicode wstring VALUE will be converted to a multibyte representation
 * according to the current code page. 
 * POS will be incremented by the actual number of multi bytes written to byte array.
 * 
 * Note: If a unicode char cannot be converted, the result is undefined.
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
