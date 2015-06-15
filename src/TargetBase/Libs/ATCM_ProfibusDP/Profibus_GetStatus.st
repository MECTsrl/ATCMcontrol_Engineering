(*[[
    UserURL= "ATCMfile:Profibus_GetStatus.htm"
]]*)
FUNCTION Profibus_GetStatus:DWORD

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
   StatusData:     Profibus_StatusType;
END_VAR

inline { STDLIB(PROFI_GET_STATUS) }

END_FUNCTION
