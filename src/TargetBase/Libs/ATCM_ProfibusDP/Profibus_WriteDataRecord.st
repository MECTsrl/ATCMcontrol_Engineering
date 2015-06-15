(*[[
    UserURL= "ATCMfile:Profibus_WriteDataRecord.htm"
]]*)
FUNCTION Profibus_WriteDataRecord : DWORD

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
  Timeout		:	INT;
  LenDataWrite	:	INT;
  DataWrite		:	ARRAY [0..255] OF BYTE;
END_VAR

VAR_OUTPUT
  ErrorCode_1	:	BYTE;
  ErrorCode_2	:	BYTE;
END_VAR

inline { STDLIB(PROFI_WRITE_DATA_RECORD) }

END_FUNCTION
