(*[[
    UserURL= "CreateDB.htm"
]]*)
FUNCTION CreateDB:DWORD
VAR_INPUT
	VarName 	: STRING;
	RecordsNum 	: INT;
	DbName 		: STRING;
	DbType 		: INT;
	Mode 		: INT;
END_VAR

inline { STDLIB(CREATEDB) }

END_FUNCTION
