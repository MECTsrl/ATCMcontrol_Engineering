(*[[
    UserURL= "ATCMFile:WSTRING_TO_BOOL.htm"
]]*)

FUNCTION WSTRING_TO_BOOL : BOOL
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_bool(tc,%1)}

END_FUNCTION
