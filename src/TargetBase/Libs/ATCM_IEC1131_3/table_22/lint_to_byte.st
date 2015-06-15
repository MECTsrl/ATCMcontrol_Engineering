(*[[
    UserURL= "ATCMfile:LINT_TO_BYTE.htm"
]]*)

FUNCTION LINT_TO_BYTE : BYTE
VAR_INPUT 
    IN : LINT; 
END_VAR

inline{(int)%1&0xff}

END_FUNCTION
