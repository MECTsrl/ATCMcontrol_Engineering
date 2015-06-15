
(*[[
    UserURL= "ATCM:LINT_TO_WSTRING.htm"
]]*)

FUNCTION LINT_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : LINT; 
END_VAR

inline{java.lang.Long.toString(%1)}

END_FUNCTION
