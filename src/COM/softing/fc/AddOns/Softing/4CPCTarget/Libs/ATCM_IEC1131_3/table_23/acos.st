(*[[
    UserURL= "ATCMFile:ACOS.htm"
]]*)

FUNCTION ACOS : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.acos(%1)}

END_FUNCTION
