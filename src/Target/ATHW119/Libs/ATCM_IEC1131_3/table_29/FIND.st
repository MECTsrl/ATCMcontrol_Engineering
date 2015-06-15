FUNCTION FIND : ANY_INT
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
END_VAR

overload INT        (STRING, STRING) inline{ STDLIB(FindString_int) }
overload INT to DINT(STRING, STRING) inline{ STDLIB(FindString_int) }

END_FUNCTION
