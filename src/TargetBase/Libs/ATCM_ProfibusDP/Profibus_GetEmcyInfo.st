(*[[
    UserURL= "ATCMfile:Profibus_GetEmcyInfo.htm"
]]*)
FUNCTION Profibus_GetEmcyInfo:DWORD

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
   StationStatus_1   : BYTE;
   StationStatus_2   : BYTE;
   StationStatus_3   : BYTE;
   MasterAddress     : BYTE;
   IdentNumber       : WORD;
   ExtendedDiagnostic: array[0..99] of BYTE;
END_VAR

inline { STDLIB(PROFI_GET_EMCY_INFO) }

END_FUNCTION
