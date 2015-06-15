(*[[
    UserURL= "RenameDB.htm"
]]*)
FUNCTION RenameDB:DWORD
VAR_INPUT
	CurrDbName 	: STRING;
	NewDbName 	: STRING;
	DbType 		: INT;
END_VAR

inline { STDLIB(RENAMEDB) }

END_FUNCTION
