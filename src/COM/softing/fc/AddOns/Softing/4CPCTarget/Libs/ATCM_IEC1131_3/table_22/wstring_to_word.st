
(*[[
    UserURL= "ATCM:WSTRING_TO_WORD.htm"
]]*)

FUNCTION WSTRING_TO_WORD : WORD
VAR_INPUT 
    IN : WSTRING; 
END_VAR

inline{Utils.wstring_to_word(tc,%1)}

END_FUNCTION
