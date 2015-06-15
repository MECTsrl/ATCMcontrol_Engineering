(*[[
    UserURL= "GetDate.htm"
]]*)

FUNCTION GetDate:DWORD

VAR_OUTPUT

   Day:         ANY_ELEMENTARY;
   Month:       ANY_ELEMENTARY;
   Year:        ANY_ELEMENTARY;
   DayOfWeek:   ANY_ELEMENTARY;

END_VAR

overload DWORD(INT,INT,INT,INT) inline { STDLIB(GETDATE) }
overload DWORD(WORD,WORD,WORD,WORD) inline { STDLIB(GETDATE) }

END_FUNCTION
