FUNCTION LEN : ANY_INT
VAR_INPUT  
    IN : STRING;
END_VAR

overload INT        (STRING) inline { STDLIB(strlen_int) }
overload INT to DINT(STRING) inline { STDLIB(strlen_int) }

END_FUNCTION



