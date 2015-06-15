
(*[[
    UserURL= "ATCM:LWORD_TO_BOOL.htm"
]]*)

FUNCTION LWORD_TO_BOOL : BOOL
VAR_INPUT 
    IN : LWORD; 
END_VAR

inline{%1==0l?false:true}

END_FUNCTION
