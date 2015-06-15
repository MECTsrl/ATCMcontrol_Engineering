
(*[[
    UserURL= "ATCM:DT_TO_DATE.htm"
]]*)

FUNCTION DT_TO_DATE : DATE
VAR_INPUT 
    IN : DT; 
END_VAR

inline{Utils.dt_to_date(%1)}

END_FUNCTION
