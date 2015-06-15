
(*[[
    UserURL= "ATCM:BYTE_TO_BOOL.htm"
]]*)

FUNCTION BYTE_TO_BOOL : BOOL
VAR_INPUT 
    IN : BYTE; 
END_VAR

inline{%1==0?false:true}

END_FUNCTION
