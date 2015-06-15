(*[[
    UserURL= "ATCMFile:LWORD_TO_WORD.htm"
]]*)

FUNCTION LWORD_TO_WORD : WORD
VAR_INPUT 
    IN : LWORD; 
END_VAR

inline{(int)%1&0xffff}

END_FUNCTION
