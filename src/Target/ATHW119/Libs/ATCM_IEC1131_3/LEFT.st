FUNCTION LEFT : ANY_STRING
VAR_INPUT
    IN : ANY_STRING;
    L : ANY_INT;
END_VAR

overload STRING(STRING, INT)          inline{ STDLIB(LeftString_int) }
overload STRING(STRING, DINT to INT)  inline{ STDLIB(LeftString_int) }

END_FUNCTION
