
(*[[
    UserURL= "ATCM:LREAL_TO_LINT.htm"
]]*)

FUNCTION LREAL_TO_LINT : LINT
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{Utils.doubleToLongIEC599(%1)}

END_FUNCTION
