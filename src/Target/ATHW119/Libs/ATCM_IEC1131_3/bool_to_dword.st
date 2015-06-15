(*[[
    UserURL= "ATCMfile:BOOL_TO_DWORD.htm"
]]*)

FUNCTION BOOL_TO_DWORD : DWORD
VAR_INPUT 
    IN : BOOL; 
END_VAR

inline{ VMOP(bool_to_dword) }

END_FUNCTION
