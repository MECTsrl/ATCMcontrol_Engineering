(*[[
    UserURL= "ATCMFile:DWORD_TO_LWORD.htm"
]]*)

FUNCTION DWORD_TO_LWORD : LWORD
VAR_INPUT 
    IN : DWORD; 
END_VAR

inline{((long)%1)&0xffffffffl}

END_FUNCTION
