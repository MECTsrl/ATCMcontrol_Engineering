
(*[[
    UserURL= "ATCM:WSTRING_TO_DATE.htm"
]]*)

FUNCTION WSTRING_TO_DATE : DATE
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_date(tc,%1)}

END_FUNCTION
