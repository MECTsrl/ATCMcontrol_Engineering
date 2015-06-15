(*[[
    UserURL= "ATCMfile:BOOL_TO_DINT.htm"
]]*)

FUNCTION BOOL_TO_INT : INT
VAR_INPUT 
    IN : BOOL; 
END_VAR

inline{ VMOP(bool_to_word) }

END_FUNCTION
