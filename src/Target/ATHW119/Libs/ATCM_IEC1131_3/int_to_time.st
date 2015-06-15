(*[[
    UserURL= "ATCMfile:DINT_TO_TIME.htm"
]]*)

FUNCTION INT_TO_TIME : TIME
VAR_INPUT 
    IN : INT; 
END_VAR

inline 
{
	%1
	int_to_dint
}

END_FUNCTION
