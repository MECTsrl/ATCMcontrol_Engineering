(*[[
    UserURL= "ATCMFile:DT_TO_TOD.htm"
]]*)

FUNCTION DT_TO_TOD : TOD
VAR_INPUT 
    IN : DT; 
END_VAR

inline{Utils.dt_to_tod(%1)}

END_FUNCTION
