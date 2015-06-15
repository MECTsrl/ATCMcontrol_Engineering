
(*[[
    UserURL= "ATCM:COPY_TIME_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_TIME_FROM_BYTE_STRING : TIME
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid warnings, true value is assigned within native code *)
    COPY_TIME_FROM_BYTE_STRING := TIME#0ms;  
    LWORD_VALUE := COPY_NUMERIC_FROM_BYTE_STRING(8,POS,BSTR);
    
    NATIVE
        _copy_time_from_byte_string = _lword_value;
    END_NATIVE;

END_FUNCTION
