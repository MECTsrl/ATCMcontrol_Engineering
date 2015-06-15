(*[[
    UserURL= "DeleteDB.htm"
]]*)
FUNCTION DeleteDB:DWORD
VAR_INPUT
	DbName 		: STRING;
	DbType 		: INT;
END_VAR

inline { STDLIB(DELETEDB) }

END_FUNCTION
