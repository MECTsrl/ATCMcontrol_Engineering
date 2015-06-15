(*[[
    UserURL= "ATCMFile:MID_DINT.htm"
]]*)

FUNCTION MID_DINT : ANY_STRING
VAR_INPUT  
    IN : ANY_STRING; 
    L  : DINT;
    P  : DINT;
END_VAR

overload() inline{Utils.midString(%1,%2,%3)}

END_FUNCTION



