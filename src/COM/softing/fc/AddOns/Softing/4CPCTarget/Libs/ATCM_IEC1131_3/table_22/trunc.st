
(*[[
    UserURL= "ATCM:TRUNC.htm"
]]*)

FUNCTION TRUNC : ANY_INT
VAR_INPUT 
    IN : ANY_REAL;
END_VAR

overload DINT(ANY_REAL) inline{(int)%1}
overload LINT(ANY_REAL) inline{(long)%1}

END_FUNCTION
