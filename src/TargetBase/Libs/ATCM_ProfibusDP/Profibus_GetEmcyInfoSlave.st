(*[[
    UserURL= "ATCMfile:Profibus_GetEmcyInfoSlave.htm"
]]*)

FUNCTION Profibus_GetEmcyInfoSlave
VAR_INPUT
END_VAR

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
   TaskState : WORD;
   ErrorCnt  : WORD;
   LastError : WORD;
   NodeId    : WORD;
END_VAR

inline {STDLIB(PROFI_GET_EMCY_INFO_SLAVE)}

END_FUNCTION
