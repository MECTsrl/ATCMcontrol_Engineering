(*[[
    UserURL= "ATCMfile:CANopen_SyncCmd.htm"
]]*)
FUNCTION CANopen_SyncCmd:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
   Timeout:        INT;
END_VAR

inline {STDLIB(SYNC_CMD)}

END_FUNCTION
