(*[[
    UserURL= "FileListDB.htm"
]]*)
FUNCTION FileListDB:DWORD

VAR_IN_OUT
	DbNamesList	: array of string;
END_VAR

VAR_INPUT
	MaxRecordsNum 	: INT;
	DbType 			: INT;
END_VAR

VAR_OUTPUT
	RecordsNum 	: INT;
END_VAR


inline { STDLIB(FILELISTDB) }

END_FUNCTION
