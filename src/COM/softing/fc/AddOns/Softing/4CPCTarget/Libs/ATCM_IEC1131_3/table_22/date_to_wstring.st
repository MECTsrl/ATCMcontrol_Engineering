
(*[[
    UserURL= "ATCM:DATE_TO_WSTRING.htm"
]]*)

FUNCTION DATE_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : DATE; 
END_VAR

inline{Utils.date_to_wstring(%1)}

END_FUNCTION
