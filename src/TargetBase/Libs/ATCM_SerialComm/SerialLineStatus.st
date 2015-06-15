(*[[
    UserURL= "SerialLineStatus.htm",
]]*)

FUNCTION SerialLineStatus:DWORD

VAR_INPUT

     Line    : INT;
     Status  : SerialLineSTS_struct;

END_VAR

inline { STDLIB(SLCSTS) }

END_FUNCTION


