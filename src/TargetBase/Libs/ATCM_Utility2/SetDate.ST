(*[[
    UserURL= "SetDate.htm",
]]*)

FUNCTION SetDate:DWORD

VAR_INPUT

   Day:       ANY_ELEMENTARY;
   Month:     ANY_ELEMENTARY;
   Year:      ANY_ELEMENTARY;

END_VAR

overload DWORD(INT,INT,INT) inline { STDLIB(SETDATE) }
overload DWORD(WORD,WORD,WORD) inline { STDLIB(SETDATE) }

END_FUNCTION
