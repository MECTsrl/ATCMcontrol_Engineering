
(*[[
    UserURL= "ATCM:WORD_TO_BOOL.htm"
]]*)

FUNCTION WORD_TO_BOOL : BOOL
VAR_INPUT 
    IN : WORD; 
END_VAR

inline{%1==0?false:true}

END_FUNCTION
