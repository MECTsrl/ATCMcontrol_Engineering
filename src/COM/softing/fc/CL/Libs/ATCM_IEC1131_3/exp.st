(*[[
    UserURL= "ATCMFile:EXP.htm"
]]*)

FUNCTION EXP : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.exp(%1)}

END_FUNCTION
