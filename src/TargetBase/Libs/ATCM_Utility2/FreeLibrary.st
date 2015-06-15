(*[[
    UserURL= "FreeLibrary.htm",
]]*)

FUNCTION FreeLibrary:DWORD

VAR_INPUT
   LibraryName      : STRING;
END_VAR

inline { STDLIB(FREELIBRARY) }

END_FUNCTION
