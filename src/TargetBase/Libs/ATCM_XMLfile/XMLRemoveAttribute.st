(*[[
    UserURL="ATCMfile:XMLRemoveAttribute.htm",
]]*)
FUNCTION XMLRemoveAttribute:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    XmlAttribName : STRING;
END_VAR

inline { STDLIB(XMLRemoveAttribute) }

END_FUNCTION
