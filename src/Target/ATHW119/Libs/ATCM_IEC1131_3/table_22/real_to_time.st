(*[[
    UserURL= "ATCMfile:REAL_TO_TIME.htm"
]]*)

FUNCTION REAL_TO_TIME : TIME
VAR_INPUT 
    IN : REAL; 
END_VAR

inline
{
    %1
    real_to_dint
}

END_FUNCTION
