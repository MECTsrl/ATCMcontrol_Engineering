(*[[
    UserURL= "OpenDB.htm"
]]*)
FUNCTION OpenDB:DWORD
VAR_INPUT
	DbName		: STRING;
	DbType		: INT;
END_VAR

VAR_OUTPUT
	DbHandle 	: DINT;
END_VAR

inline { STDLIB(OPENDB) }

END_FUNCTION

