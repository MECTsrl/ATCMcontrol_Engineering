(*[[
    UserURL= "ATCMFile:COPY_TOD_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_TOD_FROM_BYTE_STRING : TIME_OF_DAY
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid warnings, true value is assigned within native code *)
    COPY_TOD_FROM_BYTE_STRING := TIME_OF_DAY#00:00:00.000;  
    LWORD_VALUE := COPY_NUMERIC_FROM_BYTE_STRING(4,POS,BSTR);
    
    NATIVE
        _copy_tod_from_byte_string = (int) _lword_value;    
    END_NATIVE;

END_FUNCTION
