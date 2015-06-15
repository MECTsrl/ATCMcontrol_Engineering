
(*[[
    UserURL= "ATCM:WSTRING_TO_LWORD.htm"
]]*)

FUNCTION WSTRING_TO_LWORD : LWORD
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_lword(tc,%1)}

END_FUNCTION
