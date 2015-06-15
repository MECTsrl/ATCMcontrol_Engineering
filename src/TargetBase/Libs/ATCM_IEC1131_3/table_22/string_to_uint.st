(*[[
    UserURL= "ATCMfile:STRING_TO_UINT.htm"
]]*)
FUNCTION STRING_TO_UINT : UINT
VAR_INPUT
    IN : STRING;
END_VAR

inline{ STDLIB(string_to_word) }

END_FUNCTION
