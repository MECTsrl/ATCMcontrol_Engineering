
(*[[
    UserURL= "ATCM:DWORD_TO_WSTRING.htm"
]]*)

FUNCTION DWORD_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : DWORD; 
END_VAR          

inline{java.lang.Long.toString(0x00000000ffffffffL & %1)}

END_FUNCTION
