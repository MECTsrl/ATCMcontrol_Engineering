(*[[
    UserURL= "ATCMFile:ATAN.htm"
]]*)

FUNCTION ATAN : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.atan(%1)}

END_FUNCTION
