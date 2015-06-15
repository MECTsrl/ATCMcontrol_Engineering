
(*[[
    UserURL= "ATCM:WSTRING_TO_BYTE.htm"
]]*)

FUNCTION WSTRING_TO_BYTE : BYTE
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_byte(tc,%1)}

END_FUNCTION
