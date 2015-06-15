(*[[
    UserURL= "ATCMfile:BOOL_TO_DINT.htm"
]]*)

FUNCTION BOOL_TO_DINT : DINT
VAR_INPUT 
    IN : BOOL; 
END_VAR

inline{ VMOP(bool_to_dword) }

END_FUNCTION
