(*[[
    UserURL= "ATCMFile:LREAL_TO_BYTE.htm"
]]*)

FUNCTION LREAL_TO_BYTE : BYTE
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{((int)Math.round(%1))&0xff}

END_FUNCTION 
