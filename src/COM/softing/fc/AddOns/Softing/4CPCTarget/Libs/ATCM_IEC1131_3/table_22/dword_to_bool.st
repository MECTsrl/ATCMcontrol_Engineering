
(*[[
    UserURL= "ATCM:DWORD_TO_BOOL.htm"
]]*)

FUNCTION DWORD_TO_BOOL : BOOL
VAR_INPUT 
    IN : DWORD; 
END_VAR

inline{%1==0?false:true}

END_FUNCTION
