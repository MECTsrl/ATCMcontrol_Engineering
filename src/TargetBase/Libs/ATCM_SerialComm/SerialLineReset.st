(*[[
    UserURL= "SerialLineReset.htm",
]]*)

FUNCTION SerialLineReset:DWORD

VAR_INPUT

   ExecMode   :  INT;

END_VAR

VAR_OUTPUT

   ExecStatus :  DWORD;

END_VAR

VAR_INPUT

     Line     : INT;

END_VAR

inline { STDLIB(SLCRST) }

END_FUNCTION

