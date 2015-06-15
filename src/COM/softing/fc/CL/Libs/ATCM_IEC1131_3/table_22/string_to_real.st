(*[[
    UserURL= "ATCMFile:STRING_TO_REAL.htm"
]]*)

FUNCTION STRING_TO_REAL : REAL
VAR_INPUT 
    IN : STRING; 
END_VAR

inline{ STDLIB(string_to_real) }
END_FUNCTION
