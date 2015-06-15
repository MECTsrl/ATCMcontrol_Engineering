
(*[[
    UserURL= "ATCM:WSTRING_TO_LINT.htm"
]]*)

FUNCTION WSTRING_TO_LINT : LINT
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_lint(tc,%1)}

END_FUNCTION
