(*[[
    UserURL= "ATCMfile:DINT_TO_BYTE.htm"
]]*)

FUNCTION INT_TO_BYTE : BYTE
VAR_INPUT 
    IN : INT; 
END_VAR

inline{VMOP(word_to_byte)}

END_FUNCTION
