(*[[
    UserURL= "LoadLibrary.htm",
]]*)

FUNCTION LoadLibrary:DWORD

VAR_INPUT
   LibraryName      : STRING;
   StartFunctionOrd : INT;
   EndFunctionOrd   : INT;
   StartFBOrd       : INT;
   EndFBOrd         : INT;
END_VAR

inline { STDLIB(LOADLIBRARY) }

END_FUNCTION
