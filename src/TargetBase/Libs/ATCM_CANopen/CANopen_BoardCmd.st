(*[[
    UserURL= "ATCMfile:CANopen_BoardCmd.htm"
]]*)
FUNCTION CANopen_BoardCmd:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
   Command:        INT;
   CfgFileName:    STRING;
END_VAR

inline {STDLIB(CAN_BOARD_CMD)}

END_FUNCTION
