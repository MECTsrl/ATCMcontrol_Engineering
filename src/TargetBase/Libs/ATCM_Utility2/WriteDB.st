(*[[
    UserURL= "WriteDB.htm"
]]*)
FUNCTION WriteDB:DWORD
VAR_INPUT
	VarName 	: STRING;
	RecordNum 	: INT;
	DbHandle	: DINT;
	DbType 		: INT;
	Mode 		: INT;
END_VAR

inline { STDLIB(WRITEDB) }

END_FUNCTION
