
(*[[
    UserURL= "ATCM:COS.htm"
]]*)

FUNCTION COS : ANY_REAL
VAR_INPUT 
    IN : ANY_REAL; 
END_VAR

overload () inline{java.lang.Math.cos(%1)}

END_FUNCTION
