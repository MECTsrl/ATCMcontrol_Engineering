
(*[[
    UserURL= "ATCM:DINT_TO_WORD.htm"
]]*)

FUNCTION DINT_TO_WORD : WORD
VAR_INPUT 
    IN : DINT; 
END_VAR

inline{%1&0xffff}

END_FUNCTION
