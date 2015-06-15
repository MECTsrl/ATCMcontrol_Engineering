(*[[
    UserURL= "ATCMfile:CANopen_ReadSdoCmd.htm"
]]*)
FUNCTION CANopen_ReadSdoCmd:DWORD

VAR_INPUT
   ExecMode:       INT;
END_VAR

VAR_OUTPUT
   ExecStatus:     DWORD;
END_VAR

VAR_INPUT
   Board:          INT;
   NodeId:         INT;
   Index:          WORD;
   subIndex:       INT;
   Timeout:        INT;
END_VAR

VAR_OUTPUT
 DataLength:       INT;
 Data:             ANY_ELEMENTARY;
END_VAR

overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, BYTE)   inline { STDLIB(READ_SDO_CMD) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, INT)    inline { STDLIB(READ_SDO_CMD_W) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, WORD)   inline { STDLIB(READ_SDO_CMD_W) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, DINT)   inline { STDLIB(READ_SDO_CMD_DW) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, DWORD)  inline { STDLIB(READ_SDO_CMD_DW) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT, STRING) inline { STDLIB(READ_SDO_CMD_S) }

END_FUNCTION
