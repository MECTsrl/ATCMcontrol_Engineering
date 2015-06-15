
(*[[
    UserURL= "ATCM:DINT_TO_WSTRING.htm"
]]*)

FUNCTION DINT_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : DINT; 
END_VAR

inline{java.lang.Integer.toString(%1)}

END_FUNCTION
