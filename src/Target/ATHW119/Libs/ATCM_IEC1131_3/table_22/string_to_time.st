(*[[
    UserURL= "ATCMfile:WSTRING_TO_TIME.htm"
]]*)

FUNCTION STRING_TO_TIME : TIME
VAR_INPUT 
    IN : STRING; 
END_VAR

inline{ STDLIB(string_to_time) }

END_FUNCTION
