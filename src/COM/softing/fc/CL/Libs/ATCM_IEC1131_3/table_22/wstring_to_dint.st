(*[[
    UserURL= "ATCMFile:WSTRING_TO_DINT.htm"
]]*)

FUNCTION WSTRING_TO_DINT : DINT
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_dint(tc,%1)}

END_FUNCTION
