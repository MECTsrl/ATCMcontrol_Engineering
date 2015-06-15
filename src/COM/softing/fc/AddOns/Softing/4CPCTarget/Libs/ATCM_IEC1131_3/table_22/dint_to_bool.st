
(*[[
    UserURL= "ATCM:DINT_TO_BOOL.htm"
]]*)

FUNCTION DINT_TO_BOOL : BOOL
VAR_INPUT 
    IN : DINT; 
END_VAR

inline{%1==0?false:true}

END_FUNCTION
