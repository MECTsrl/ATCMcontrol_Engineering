(*[[
    UserURL= "ATCMFile:COPY_BYTE_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_BYTE_FROM_BYTE_STRING : BYTE
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR

    COPY_BYTE_FROM_BYTE_STRING := LWORD_TO_BYTE(
        COPY_NUMERIC_FROM_BYTE_STRING(1,POS,BSTR));  

END_FUNCTION
