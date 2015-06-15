(*[[
    UserURL= "ATCMfile:UDINT_TO_STRING.htm"
]]*)
FUNCTION UDINT_TO_STRING : STRING
VAR_INPUT
    IN : UDINT;
END_VAR

inline{ STDLIB(dword_to_string) }

END_FUNCTION
