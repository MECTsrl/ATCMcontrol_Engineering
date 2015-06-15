
(*[[
    UserURL= "ATCM:DT_TO_WSTRING.htm"
]]*)

FUNCTION DT_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : DT; 
END_VAR

inline{Utils.dt_to_wstring(%1)}

END_FUNCTION
