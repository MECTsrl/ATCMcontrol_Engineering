(*[[
    UserURL= "ATCMfile:Profibus_ReadDiagnosis.htm"
]]*)
FUNCTION Profibus_ReadDiagnosis : DWORD

VAR_INPUT
   ExecMode			:	INT;
END_VAR

VAR_OUTPUT
   ExecStatus		:	DWORD;
END_VAR

VAR_INPUT
  Board				:	INT;
  Timeout			:	INT;
END_VAR

VAR_OUTPUT
  RemAddr			:	INT;
  SlotNumber		:	INT;
  SeqNr				:	INT;
  AlarmType			:	BYTE;
  AlarmSpeckAck		:	BYTE;
  LenDiagUserData	:	INT;
  DiagUserData		:	ARRAY [0..64] OF BYTE;
END_VAR

inline { STDLIB(PROFI_READ_DIAGNOSIS) }

END_FUNCTION

