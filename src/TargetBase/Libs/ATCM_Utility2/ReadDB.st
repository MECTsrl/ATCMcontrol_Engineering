(*[[
    UserURL= "ReadDB.htm"
]]*)
FUNCTION ReadDB:DWORD
VAR_INPUT
	VarName 	: STRING;
	RecordNum 	: INT;
	DbHandle	: DINT;
	DbType 		: INT;
	Mode 		: INT;
END_VAR

inline { STDLIB(READDB) }

END_FUNCTION
