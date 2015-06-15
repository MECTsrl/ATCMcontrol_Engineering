(*[[
    UserURL= "ATCMFile:COPY_DINT_TO_BYTE_ARRAY.htm"
]]*)

(* copies the value VALUE to the byte array at the position POS *)

FUNCTION COPY_DINT_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : DINT;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR
        
inline { STDLIB ( copy_dint_to_byte_array ) }
    
END_FUNCTION
