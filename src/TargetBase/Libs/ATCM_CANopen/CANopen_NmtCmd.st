(*[[
    UserURL= "ATCMfile:CANopen_NmtCmd.htm"
]]*)
FUNCTION CANopen_NmtCmd:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
   NodeId:         INT;
   Cmd:            INT;
   Timeout:        INT;
END_VAR

inline { STDLIB(NMT_CMD) }

END_FUNCTION
