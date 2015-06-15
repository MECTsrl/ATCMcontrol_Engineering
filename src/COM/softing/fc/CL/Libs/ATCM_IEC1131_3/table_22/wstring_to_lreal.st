(*[[
    UserURL= "ATCMFile:WSTRING_TO_LREAL.htm"
]]*)

FUNCTION WSTRING_TO_LREAL : LREAL
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_lreal(tc,%1)}

END_FUNCTION
