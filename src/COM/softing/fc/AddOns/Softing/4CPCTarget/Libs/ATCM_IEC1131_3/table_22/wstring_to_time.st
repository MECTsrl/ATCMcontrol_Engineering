
(*[[
    UserURL= "ATCM:WSTRING_TO_TIME.htm"
]]*)

FUNCTION WSTRING_TO_TIME : TIME
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_time(tc,%1)}

END_FUNCTION
