(*[[
    UserURL= "GetTime.htm",
]]*)

FUNCTION GetTime:DWORD

VAR_OUTPUT

   Hour:       ANY_ELEMENTARY;
   Minute:     ANY_ELEMENTARY;
   Second:     ANY_ELEMENTARY;
   MilliSec:   ANY_ELEMENTARY;

END_VAR

overload DWORD(INT,INT,INT,INT) inline { STDLIB(GETTIME) }
overload DWORD(WORD,WORD,WORD,WORD) inline { STDLIB(GETTIME) }

END_FUNCTION
