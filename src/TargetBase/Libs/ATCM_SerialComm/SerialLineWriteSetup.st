(*[[
    UserURL= "SerialLineWriteSetup.htm",
]]*)

FUNCTION SerialLineWriteSetup:DWORD

VAR_INPUT

     Line    : INT;
     Setup   : SerialLineCFG_struct;

END_VAR

inline { STDLIB(SLCWCFG) }

END_FUNCTION

