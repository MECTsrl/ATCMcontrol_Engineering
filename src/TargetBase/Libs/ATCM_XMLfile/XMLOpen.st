(*[[
    UserURL="ATCMfile:XMLOpen.htm",
]]*)
FUNCTION XMLOpen:DWORD
VAR_INPUT
    path : STRING;
    mode : WORD;
END_VAR
VAR_OUTPUT
    XMLhandle : DINT;
END_VAR

inline { STDLIB(XMLOpen) }

END_FUNCTION
