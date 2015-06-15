
(*[[
    UserURL= "ATCM:SEL.htm"
]]*)

FUNCTION SEL : ANY
VAR_INPUT 
    G   : BOOL; 
    IN0 : ANY; 
    IN1 : ANY; 
END_VAR

overload ANY(BOOL, ANY, ANY) inline{%1 ? %3 : %2}

END_FUNCTION
