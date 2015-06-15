(*[[
    UserURL= "ATCMfile:COPY_LREAL_TO_BYTE_ARRAY.htm"
]]*)

(* copies the value VALUE to the byte string BSTR at the position POS *)

FUNCTION COPY_REAL_TO_BYTE_ARRAY
VAR_INPUT
    VALUE : REAL;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_real_to_byte_array ) }    

END_FUNCTION
