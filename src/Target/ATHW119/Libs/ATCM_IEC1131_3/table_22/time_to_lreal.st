(*[[
    UserURL= "ATCMfile:TIME_TO_LREAL.htm"
]]*)

FUNCTION TIME_TO_LREAL : LREAL
VAR_INPUT 
    IN : TIME; 
END_VAR

inline
{
    %1
    dint_to_lreal
}

END_FUNCTION
