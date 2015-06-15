FUNCTION INSERT : ANY_STRING
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
    P : ANY_INT;
END_VAR

overload STRING(STRING, STRING,         INT) inline{ STDLIB(InsertString_int)}
overload STRING(STRING, STRING, DINT to INT) inline{ STDLIB(InsertString_int)}

END_FUNCTION
