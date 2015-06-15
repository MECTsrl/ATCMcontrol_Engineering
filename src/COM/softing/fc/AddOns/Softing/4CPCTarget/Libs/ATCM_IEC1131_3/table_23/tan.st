
(*[[
    UserURL= "ATCM:TAN.htm"
]]*)

FUNCTION TAN : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.tan(%1)}

END_FUNCTION
