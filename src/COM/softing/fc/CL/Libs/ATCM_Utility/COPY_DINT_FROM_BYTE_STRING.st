(*[[
    UserURL= "ATCMFile:COPY_DINT_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_DINT_FROM_BYTE_STRING : DINT
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
    
    COPY_DINT_FROM_BYTE_STRING := 
        LWORD_TO_DINT(COPY_NUMERIC_FROM_BYTE_STRING(4,POS,BSTR));
    
END_FUNCTION
