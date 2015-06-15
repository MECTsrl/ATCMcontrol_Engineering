
(*[[
    UserURL= "ATCM:WORD_TO_WSTRING.htm"
]]*)

FUNCTION WORD_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : WORD; 
END_VAR

inline{java.lang.Integer.toString(%1)}

END_FUNCTION
