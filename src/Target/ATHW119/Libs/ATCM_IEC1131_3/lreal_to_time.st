(*[[
    UserURL= "ATCMfile:LREAL_TO_TIME.htm"
]]*)

FUNCTION LREAL_TO_TIME : TIME
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline
{
    %1
	lreal_to_dint
}

END_FUNCTION
