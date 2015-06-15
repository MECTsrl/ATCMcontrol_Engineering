
(*[[
    UserURL= "ATCM:TIME_TO_DINT.htm",
    deprecated="to avoid unexpected truncation use TIME_TO_LINT instead"
]]*)

FUNCTION TIME_TO_DINT : DINT
VAR_INPUT 
    IN : TIME; 
END_VAR

inline{(int)%1}

END_FUNCTION
