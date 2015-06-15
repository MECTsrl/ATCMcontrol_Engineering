(*[[
    UserURL="ATCMfile:XMLRead.htm",
]]*)
FUNCTION XMLRead:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLRead) }

END_FUNCTION
