(*[[
    UserURL= "ATCMfile:COPY_DWORD_FROM_BYTE_ARRAY.htm"
]]*)

(* returns a value copied from the byte array at the position POS *)

FUNCTION COPY_DWORD_FROM_BYTE_ARRAY : DWORD
VAR_IN_OUT
    POS : DINT;
    BYTES : ARRAY OF BYTE;
END_VAR

inline { STDLIB ( copy_dword_from_byte_array ) }

END_FUNCTION
