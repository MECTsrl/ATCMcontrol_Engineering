(*[[
    UserURL= "CopyDB.htm"
]]*)
FUNCTION CopyDB:DWORD
VAR_INPUT
	SourceDbName 	: STRING;
	SourceDbType 	: INT;
	DestDbName 		: STRING;
	DestDbType 		: INT;
	Mode 			: INT;
END_VAR

inline { STDLIB(COPYDB) }

END_FUNCTION
