(*[[
    UserURL= "ATCMfile:UINT_TO_STRING.htm"
]]*)
FUNCTION UINT_TO_STRING : STRING
VAR_INPUT
    IN : UINT;
END_VAR

inline{ STDLIB(word_to_string) }

END_FUNCTION
