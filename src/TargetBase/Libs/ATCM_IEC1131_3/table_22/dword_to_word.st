(*[[
    UserURL= "ATCMfile:DWORD_TO_WORD.htm"
]]*)

FUNCTION DWORD_TO_WORD : WORD
VAR_INPUT 
    IN : DWORD; 
END_VAR

inline{VMOP(dword_to_uint)}

END_FUNCTION
