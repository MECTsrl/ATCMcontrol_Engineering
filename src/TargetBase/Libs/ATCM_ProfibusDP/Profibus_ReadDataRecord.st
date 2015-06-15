(*[[
    UserURL= "ATCMfile:Profibus_ReadDataRecord.htm"
]]*)
FUNCTION Profibus_ReadDataRecord : DWORD

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
  TimeOut		:	INT;
END_VAR

VAR_OUTPUT
  LenDataRead	:	INT;
  DataRead		:	ARRAY [0..255] OF BYTE;
  ErrorCode_1	:	BYTE;
  ErrorCode_2	:	BYTE;
END_VAR


inline { STDLIB(PROFI_READ_DATA_RECORD) }

END_FUNCTION
