(*[[
    UserURL= "SerialLineReadSetup.htm",
]]*)

FUNCTION SerialLineReadSetup:DWORD

VAR_INPUT

     Line    : INT;

END_VAR

VAR_OUTPUT

     Setup   : SerialLineCFG_struct;

END_VAR

inline { STDLIB(SLCRCFG) }

END_FUNCTION

