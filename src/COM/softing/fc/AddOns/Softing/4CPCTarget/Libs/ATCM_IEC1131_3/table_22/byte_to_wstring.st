
(*[[
    UserURL= "ATCM:BYTE_TO_WSTRING.htm"
]]*)

FUNCTION BYTE_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : BYTE; 
END_VAR

inline{java.lang.Integer.toString(%1)}

END_FUNCTION
