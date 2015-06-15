(*[[
    UserURL= "ATCMfile:BOOL_TO_BYTE.htm"
]]*)

FUNCTION BOOL_TO_BYTE : BYTE
VAR_INPUT 
    IN : BOOL; 
END_VAR

inline { VMOP(bool_to_byte) }

END_FUNCTION
