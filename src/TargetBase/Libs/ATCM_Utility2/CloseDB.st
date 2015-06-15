(*[[
    UserURL= "CloseDB.htm"
]]*)
FUNCTION CloseDB:DWORD
VAR_INPUT
	DbHandle	: DINT;
	DbType 		: INT;
END_VAR

inline { STDLIB(READDB) }

END_FUNCTION
