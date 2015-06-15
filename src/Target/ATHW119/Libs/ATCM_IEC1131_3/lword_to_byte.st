(*[[
    UserURL= "ATCMfile:LWORD_TO_BYTE.htm"
]]*)

FUNCTION LWORD_TO_BYTE : BYTE
VAR_INPUT 
    IN : LWORD; 
END_VAR

inline{(int)%1&0xff}

END_FUNCTION
