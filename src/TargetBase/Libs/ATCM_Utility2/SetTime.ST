(*[[
    UserURL= "SetTime.htm",
]]*)

FUNCTION SetTime:DWORD

VAR_INPUT

   Hour:       ANY_ELEMENTARY;
   Minute:     ANY_ELEMENTARY;
   Second:     ANY_ELEMENTARY;
   MilliSec:   ANY_ELEMENTARY;

END_VAR

overload DWORD(INT,INT,INT,INT) inline { STDLIB(SETTIME) }
overload DWORD(WORD,WORD,WORD,WORD) inline { STDLIB(SETTIME) }

END_FUNCTION
