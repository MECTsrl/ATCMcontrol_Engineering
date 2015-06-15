(*[[
    UserURL= "ATCMfile:DINT_TO_DWORD.htm"
]]*)

FUNCTION INT_TO_DWORD : DWORD
VAR_INPUT 
    IN : INT; 
END_VAR

inline{VMOP(int_to_dword)}

END_FUNCTION
