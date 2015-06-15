(*[[
    UserURL= "ATCMFile:COPY_LREAL_TO_BYTE_STRING.htm"
]]*)

(* copies the value VALUE to the byte string BSTR at the position POS *)

FUNCTION COPY_LREAL_TO_BYTE_STRING
VAR_INPUT
    VALUE : LREAL;
END_VAR
VAR_IN_OUT
    POS : DINT;
    BSTR : BYTE_STRING;
END_VAR
VAR
    LWORD_VALUE : LWORD;
END_VAR
    
    NATIVE
        _lword_value = Double.doubleToLongBits(_value);
    END_NATIVE;
        
    COPY_NUMERIC_TO_BYTE_STRING(LWORD_VALUE,8,POS,BSTR);

END_FUNCTION
