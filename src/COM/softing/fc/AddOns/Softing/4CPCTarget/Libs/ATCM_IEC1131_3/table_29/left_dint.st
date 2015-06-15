
FUNCTION LEFT_DINT : ANY_STRING
VAR_INPUT
    IN : ANY_STRING;
    L  : DINT;
END_VAR

overload() inline{Utils.leftString(%1,%2)}

END_FUNCTION