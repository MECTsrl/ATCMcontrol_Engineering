(*[[
    UserURL= "ATCMfile:DWORD_TO_WSTRING.htm"
]]*)

FUNCTION DWORD_TO_STRING : STRING
VAR_INPUT 
    IN : DWORD; 
END_VAR          

inline{ STDLIB(dword_to_string) }

END_FUNCTION
