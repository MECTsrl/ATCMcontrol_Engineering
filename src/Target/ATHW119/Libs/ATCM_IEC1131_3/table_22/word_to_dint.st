(*[[
    UserURL= "ATCMfile:WORD_TO_DINT.htm"
]]*)

FUNCTION WORD_TO_DINT : DINT
VAR_INPUT 
    IN : WORD; 
END_VAR

inline{VMOP(word_to_dword)}

END_FUNCTION
