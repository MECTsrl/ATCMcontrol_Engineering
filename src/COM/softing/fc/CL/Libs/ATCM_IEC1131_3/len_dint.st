(*[[
    UserURL= "ATCMFile:LEN_DINT.htm"
]]*)

FUNCTION LEN_DINT : DINT
VAR_INPUT  
    IN : ANY_STRING; 
END_VAR

overload() inline{%1.length()}

END_FUNCTION



