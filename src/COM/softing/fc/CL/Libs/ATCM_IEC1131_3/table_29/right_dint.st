(*[[
    UserURL= "ATCMFile:RIGHT_DINT.htm"
]]*)

FUNCTION RIGHT_DINT : ANY_STRING
VAR_INPUT  
    IN : ANY_STRING; 
    L  : DINT;
END_VAR

overload() inline{Utils.rightString(%1,%2)}

END_FUNCTION



