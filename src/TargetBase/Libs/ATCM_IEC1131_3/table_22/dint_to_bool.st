(*[[
    UserURL= "ATCMfile:DINT_TO_BOOL.htm"
]]*)

FUNCTION DINT_TO_BOOL : BOOL
VAR_INPUT 
    IN : DINT; 
END_VAR

inline{ VMOP(dword_to_bool) }

END_FUNCTION
