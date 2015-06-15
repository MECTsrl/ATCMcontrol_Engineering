(*[[
    UserURL= "ATCMfile:BYTE_TO_DINT.htm"
]]*)

FUNCTION BYTE_TO_DINT : DINT
VAR_INPUT 
    IN : BYTE; 
END_VAR

inline{ VMOP(byte_to_dword) }

END_FUNCTION
