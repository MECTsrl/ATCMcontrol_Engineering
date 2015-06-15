(*[[
    UserURL= "ATCMFile:REPLACE_DINT.htm"
]]*)

FUNCTION REPLACE_DINT : ANY_STRING
VAR_INPUT  
    IN1 : ANY_STRING; 
    IN2 : ANY_STRING; 
    L   : DINT;
    P   : DINT;
END_VAR

overload() inline{Utils.replaceString(%1,%2,%3,%4)}

END_FUNCTION



