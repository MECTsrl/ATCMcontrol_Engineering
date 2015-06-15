
(*[[
    UserURL= "ATCM:WSTRING_TO_DT.htm"
]]*)

FUNCTION WSTRING_TO_DT : DT
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_dt(tc,%1)}

END_FUNCTION
