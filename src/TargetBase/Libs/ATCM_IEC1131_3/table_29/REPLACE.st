FUNCTION REPLACE : ANY_STRING
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
    L : ANY_INT;
    P : ANY_INT;
END_VAR

overload STRING(STRING,STRING, INT,INT)                  inline{ STDLIB(ReplaceString_int) }    
overload STRING(STRING,STRING, DINT to INT ,DINT to INT) inline{ STDLIB(ReplaceString_int) }    


END_FUNCTION
