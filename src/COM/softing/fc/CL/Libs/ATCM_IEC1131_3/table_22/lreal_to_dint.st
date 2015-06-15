(*[[
    UserURL= "ATCMFile:LREAL_TO_DINT.htm"
]]*)

FUNCTION LREAL_TO_DINT : DINT
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{Utils.doubleToIntIEC599(%1)}

END_FUNCTION
