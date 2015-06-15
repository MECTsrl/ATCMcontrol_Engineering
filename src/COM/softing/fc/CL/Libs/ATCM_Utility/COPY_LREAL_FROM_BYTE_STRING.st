(*[[
    UserURL= "ATCMFile:COPY_LREAL_FROM_BYTE_STRING.htm"
]]*)

(* returns a value copied from the byte string BSTR at the position POS *)

FUNCTION COPY_LREAL_FROM_BYTE_STRING : LREAL
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid warnings, true value is assigned within native code *)
    COPY_LREAL_FROM_BYTE_STRING := 0.0;  
    LWORD_VALUE := COPY_NUMERIC_FROM_BYTE_STRING(8,POS,BSTR);
    
    NATIVE
        _copy_lreal_from_byte_string = Double.longBitsToDouble(_lword_value);    
    END_NATIVE;

END_FUNCTION
