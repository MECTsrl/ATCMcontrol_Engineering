(*[[
    UserURL= "ATCMFile:COPY_DT_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_DT_FROM_BYTE_STRING : DATE_AND_TIME
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid warnings, true value is assigned within native code *)
    COPY_DT_FROM_BYTE_STRING := DATE_AND_TIME#0001-01-01-00:00:00.000;  
    LWORD_VALUE := COPY_NUMERIC_FROM_BYTE_STRING(8,POS,BSTR);
    
    NATIVE
        _copy_dt_from_byte_string = _lword_value;
    END_NATIVE;

END_FUNCTION
