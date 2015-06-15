(*[[
    UserURL= "ATCMFile:LREAL_TO_WSTRING.htm"
]]*)

FUNCTION LREAL_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{java.lang.Double.toString(%1)}

END_FUNCTION
