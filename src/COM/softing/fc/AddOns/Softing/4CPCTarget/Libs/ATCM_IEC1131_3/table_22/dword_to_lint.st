
(*[[
    UserURL= "ATCM:DWORD_TO_LINT.htm"
]]*)

FUNCTION DWORD_TO_LINT : LINT
VAR_INPUT 
    IN : DWORD; 
END_VAR

inline{((long)%1)&0xffffffffl}

END_FUNCTION
