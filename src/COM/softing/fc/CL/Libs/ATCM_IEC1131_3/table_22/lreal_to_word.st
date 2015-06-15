(*[[
    UserURL= "ATCMFile:LREAL_TO_WORD.htm"
]]*)

FUNCTION LREAL_TO_WORD : WORD
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{((int)Math.round(%1))&0xffff}

END_FUNCTION
