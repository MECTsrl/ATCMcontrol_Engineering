(*[[
    UserURL= "ATCMFile:LINT_TO_STRING.htm"
]]*)

FUNCTION LINT_TO_STRING : STRING
VAR_INPUT 
    IN : LINT; 
END_VAR

inline{java.lang.Long.toString(%1)}

END_FUNCTION
