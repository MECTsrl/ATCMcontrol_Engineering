(*[[
    UserURL= "ATCMfile:TIME_TO_LREAL.htm"
]]*)

FUNCTION TIME_TO_REAL : REAL
VAR_INPUT 
    IN : TIME; 
END_VAR

inline
{
    %1
    dint_to_real
}

END_FUNCTION
