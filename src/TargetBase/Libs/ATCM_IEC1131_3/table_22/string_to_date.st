(*[[
    UserURL= "ATCMfile:STRING_TO_DATE.htm"
]]*)

FUNCTION STRING_TO_DATE : DATE
VAR_INPUT 
    IN : STRING; 
END_VAR

inline{ STDLIB(string_to_date) }

END_FUNCTION
