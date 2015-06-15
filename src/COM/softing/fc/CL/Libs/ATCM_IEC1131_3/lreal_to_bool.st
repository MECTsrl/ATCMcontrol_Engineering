(*[[
    UserURL= "ATCMFile:LREAL_TO_BOOL.htm"
]]*)

FUNCTION LREAL_TO_BOOL : BOOL
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{%1==0.0?false:true}

END_FUNCTION
