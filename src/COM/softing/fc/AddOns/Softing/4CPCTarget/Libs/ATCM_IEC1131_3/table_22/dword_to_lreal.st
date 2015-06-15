
(*[[
    UserURL= "ATCM:DWORD_TO_LREAL.htm"
]]*)

FUNCTION DWORD_TO_LREAL : LREAL
VAR_INPUT 
    IN : DWORD; 
END_VAR

inline{(double)(0x00000000ffffffffL & %1)}

END_FUNCTION
