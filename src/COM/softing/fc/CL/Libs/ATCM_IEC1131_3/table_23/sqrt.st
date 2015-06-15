(*[[
    UserURL= "ATCMFile:SQRT.htm"
]]*)

FUNCTION SQRT : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.sqrt(%1)}

END_FUNCTION
