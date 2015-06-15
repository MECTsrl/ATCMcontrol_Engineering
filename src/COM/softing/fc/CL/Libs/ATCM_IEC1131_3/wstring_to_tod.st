(*[[
    UserURL= "ATCMFile:WSTRING_TO_TOD.htm"
]]*)

FUNCTION WSTRING_TO_TOD : TOD
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_tod(tc,%1)}

END_FUNCTION
