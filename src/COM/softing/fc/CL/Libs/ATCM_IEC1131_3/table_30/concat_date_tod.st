(*[[
    UserURL= "ATCMFile:CONCAT_DATE_TOD.htm"
]]*)

FUNCTION CONCAT_DATE_TOD : DT
VAR_INPUT  
    IN1 : DATE; 
    IN2 : TOD; 
END_VAR

inline{Utils.concat_date_tod(%1,%2)}

END_FUNCTION



