(*[[
    UserURL= "ATCMfile:STRING_TO_BYTE.htm"
]]*)

FUNCTION STRING_TO_BYTE : BYTE
VAR_INPUT 
    IN : STRING; 
END_VAR

inline{ STDLIB(string_to_byte) }

END_FUNCTION
