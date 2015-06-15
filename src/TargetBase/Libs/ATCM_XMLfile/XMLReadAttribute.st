(*[[
    UserURL="ATCMfile:XMLReadAttribute.htm",
]]*)
FUNCTION XMLReadAttribute:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    XmlAttribName : STRING;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLReadAttribute) }

END_FUNCTION
