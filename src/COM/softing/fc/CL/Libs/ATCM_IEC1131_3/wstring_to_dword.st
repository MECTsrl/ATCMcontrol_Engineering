(*[[
    UserURL= "ATCMFile:WSTRING_TO_DWORD.htm"
]]*)

FUNCTION WSTRING_TO_DWORD : DWORD
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_dword(tc,%1)}

END_FUNCTION
