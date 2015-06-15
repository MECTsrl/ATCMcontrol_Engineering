(*[[
    UserURL="ATCMfile:XMLWriteAttribute.htm",
]]*)
FUNCTION XMLWriteAttribute:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    XmlAttribName : STRING;
    Mode    : INT;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLWriteAttribute) }

END_FUNCTION
