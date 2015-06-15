(*[[
    UserURL= "ATCMFile:COPY_DATE_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_DATE_FROM_BYTE_STRING : DATE
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid warnings, true value is assigned within native code *)
    COPY_DATE_FROM_BYTE_STRING := DATE#0001-01-01;  
    LWORD_VALUE := COPY_NUMERIC_FROM_BYTE_STRING(4,POS,BSTR);
    
    NATIVE
        _copy_date_from_byte_string = (int) _lword_value;    
    END_NATIVE;

END_FUNCTION
