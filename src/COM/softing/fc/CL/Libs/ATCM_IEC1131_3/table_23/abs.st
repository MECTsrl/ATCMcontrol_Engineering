(*[[
    UserURL= "ATCMFile:ABS.htm"
]]*)

FUNCTION ABS : ANY_NUM
VAR_INPUT 
    IN : ANY_NUM; 
END_VAR

overload () inline{java.lang.Math.abs(%1)}

END_FUNCTION
