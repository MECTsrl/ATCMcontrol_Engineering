
(*[[
    UserURL= "ATCM:LN.htm"
]]*)

FUNCTION LN : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.log(%1)}

END_FUNCTION
