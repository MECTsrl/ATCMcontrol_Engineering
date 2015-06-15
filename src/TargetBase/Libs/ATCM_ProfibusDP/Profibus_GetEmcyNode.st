(*[[
    UserURL= "ATCMfile:Profibus_GetEmcyNode.htm"
]]*)
FUNCTION Profibus_GetEmcyNode:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
END_VAR

VAR_IN_OUT
   NodeId:         INT;
END_VAR

inline {STDLIB(PROFI_GET_EMCY_NODE)}

END_FUNCTION
