
FUNCTION FIND_DINT : DINT
VAR_INPUT  
    IN1 : ANY_STRING; 
    IN2 : ANY_STRING; 
END_VAR

overload() inline{%1.indexOf(%2)+1}

END_FUNCTION



