(*[[
    UserURL= "ATCMFile:LOG.htm"
]]*)

FUNCTION LOG : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.log(%1)/java.lang.Math.log(10)}

END_FUNCTION
