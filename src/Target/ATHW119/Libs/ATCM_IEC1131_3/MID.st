FUNCTION MID : ANY_STRING
VAR_INPUT
    IN : ANY_STRING;
    L : ANY_INT;
    P : ANY_INT;
END_VAR

overload STRING(STRING,         INT,         INT) inline{ STDLIB(MidString_int) }
overload STRING(STRING, DINT to INT, DINT to INT) inline{ STDLIB(MidString_int) }

END_FUNCTION
