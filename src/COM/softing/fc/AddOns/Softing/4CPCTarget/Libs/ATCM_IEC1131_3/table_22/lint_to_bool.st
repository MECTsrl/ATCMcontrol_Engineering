
(*[[
    UserURL= "ATCM:LINT_TO_BOOL.htm"
]]*)

FUNCTION LINT_TO_BOOL : BOOL
VAR_INPUT 
    IN : LINT; 
END_VAR

inline{%1==0l?false:true}

END_FUNCTION
