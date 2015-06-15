
(*[[
    UserURL= "ATCM:LREAL_TO_DWORD.htm"
]]*)

FUNCTION LREAL_TO_DWORD : DWORD
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{(int)Math.round(%1)}

END_FUNCTION
