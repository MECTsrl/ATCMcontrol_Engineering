(*[[
    UserURL= "ATCMfile:TIME_TO_WSTRING.htm"
]]*)

FUNCTION TIME_TO_STRING : STRING
VAR_INPUT 
   IN : TIME; 
END_VAR

inline{ STDLIB(time32_to_string) }

END_FUNCTION
