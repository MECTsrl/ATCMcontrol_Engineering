(*[[
    UserURL= "ResizeDB.htm"
]]*)
FUNCTION ResizeDB:DWORD
VAR_INPUT
	DbName 			: STRING;
	DbType 			: INT;
	NewRecordsNum 	: INT;
END_VAR

inline { STDLIB(RESIZEDB) }

END_FUNCTION

