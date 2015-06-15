(*[[
    UserURL= "ATCMfile:Profibus_ReceiveAlarms.htm"
]]*)
FUNCTION Profibus_ReceiveAlarms : DWORD

VAR_INPUT
   ExecMode		:	INT;
END_VAR

VAR_OUTPUT
   ExecStatus	:	DWORD;
END_VAR

VAR_INPUT
  Board			:	INT;
  RemAddr		:	INT;
  Index			:	INT;
  SlotNumber	:	INT;
  SeqNr			:	INT;
  Timeout		:	INT;
END_VAR

VAR_OUTPUT
  AlarmType		:	BYTE;
  AlarmSpeckAck	:	BYTE;
END_VAR

inline { STDLIB(PROFI_RECEIVE_ALARMS) }

END_FUNCTION
