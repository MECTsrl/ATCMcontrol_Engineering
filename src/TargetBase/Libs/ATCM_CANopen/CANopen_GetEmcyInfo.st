(*[[
    UserURL= "ATCMfile:CANopen_GetEmcyInfo.htm"
]]*)
FUNCTION CANopen_GetEmcyInfo:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
   NodeId:         INT;
   Timeout:        INT;
END_VAR

VAR_OUTPUT
   NodeStatus:     WORD;
   AddInfo:        WORD;
   ProfileNum:     INT;
   NodeState:      INT;
   ActualErr:      INT;
   EmcyLen:        INT;
   EmcyData: array[0..4] of CANopen_EmcyType;
END_VAR

inline { STDLIB(GET_EMCY_INFO) }

END_FUNCTION
