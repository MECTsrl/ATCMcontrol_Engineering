(*[[
    UserURL= "ATCMfile:CANopen_GetStatus.htm"
]]*)
FUNCTION CANopen_GetStatus:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
END_VAR

VAR_OUTPUT
   StatusData:     CANopen_StatusType;
END_VAR

inline { STDLIB(GET_STATUS) }

END_FUNCTION
