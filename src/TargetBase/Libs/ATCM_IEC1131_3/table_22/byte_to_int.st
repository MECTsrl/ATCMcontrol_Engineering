(*[[
    UserURL= "ATCMfile:BYTE_TO_DINT.htm"
]]*)

FUNCTION BYTE_TO_INT : INT
VAR_INPUT 
    IN : BYTE; 
END_VAR

inline{ VMOP(byte_to_word) }

END_FUNCTION
