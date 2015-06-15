(*[[
    UserURL= "ATCMFile:LEFT_DINT.htm"
]]*)

FUNCTION LEFT_DINT : ANY_STRING
VAR_INPUT  
    IN : ANY_STRING; 
    L  : DINT;
END_VAR

overload() inline{%1.substring(0,%2)}

END_FUNCTION



