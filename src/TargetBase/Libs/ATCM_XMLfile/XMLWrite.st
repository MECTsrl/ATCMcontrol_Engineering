(*[[
    UserURL="ATCMfile:XMLWrite.htm",
]]*)
FUNCTION XMLWrite:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    Mode    : INT;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLWrite) }

END_FUNCTION
