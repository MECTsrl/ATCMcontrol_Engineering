(*[[
    UserURL= "ATCMFile:STRING_TO_LREAL.htm"
]]*)

FUNCTION STRING_TO_LREAL : LREAL
VAR_INPUT 
    IN : STRING; 
END_VAR

inline{ STDLIB(string_to_lreal) }

END_FUNCTION
