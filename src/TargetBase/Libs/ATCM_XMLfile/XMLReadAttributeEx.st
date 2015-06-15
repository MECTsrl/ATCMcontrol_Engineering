(*[[
    UserURL="ATCMfile:XMLReadAttributeEx.htm",
]]*)
FUNCTION XMLReadAttributeEx:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
    XmlAttribName : STRING;
	Mode :    WORD;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLReadAttributeEx) }

END_FUNCTION
