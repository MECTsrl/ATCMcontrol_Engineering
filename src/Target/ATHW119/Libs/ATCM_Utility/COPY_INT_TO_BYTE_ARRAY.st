(*[[
    UserURL= "ATCMfile:COPY_INT_TO_BYTE_ARRAY.htm"
]]*)

(* copies the value VALUE to the byte array BYTES at the position POS *)
FUNCTION COPY_INT_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : INT;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR
        
inline { STDLIB ( copy_int_to_byte_array ) }
    
END_FUNCTION
