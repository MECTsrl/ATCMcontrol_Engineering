(*[[
    UserURL= "ATCMFile:ASIN.htm"
]]*)

FUNCTION ASIN : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.asin(%1)}

END_FUNCTION
