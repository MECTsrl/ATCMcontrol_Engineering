(*[[
    UserURL= "ATCMFile:BOOL_TO_INT.htm"
]]*)

FUNCTION BOOL_TO_INT : INT
VAR_INPUT 
    IN : BOOL; 
END_VAR

inline{ VMOP(bool_to_int) }

END_FUNCTION
