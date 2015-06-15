
(*[[
    UserURL= "ATCM:TOD_TO_WSTRING.htm"
]]*)

FUNCTION TOD_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : TOD; 
END_VAR

inline{Utils.tod_to_wstring(%1)}

END_FUNCTION
