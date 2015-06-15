(*[[
    UserURL="ATCMfile:XMLReadEx.htm",
]]*)
FUNCTION XMLReadEx:DWORD
VAR_INPUT
    XMLhandle : DINT;
    XmlPath : STRING;
	Mode :    WORD;
    VarName : STRING;
END_VAR

inline { STDLIB(XMLReadEx) }

END_FUNCTION
