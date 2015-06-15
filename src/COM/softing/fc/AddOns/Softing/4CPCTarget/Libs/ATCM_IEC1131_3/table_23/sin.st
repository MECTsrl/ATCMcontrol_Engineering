
(*[[
    UserURL= "ATCM:SIN.htm"
]]*)

FUNCTION SIN : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.sin(%1)}

END_FUNCTION
