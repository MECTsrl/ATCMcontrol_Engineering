(*[[
    UserURL= "ATCMfile:CANopen_WriteSdoCmd.htm"
]]*)
FUNCTION CANopen_WriteSdoCmd:DWORD

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
   DataLength:     INT;
END_VAR

VAR_IN_OUT
   Data:           ANY_ELEMENTARY;
END_VAR

overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  BYTE)   inline { STDLIB(WRITE_SDO_CMD) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  INT)    inline { STDLIB(WRITE_SDO_CMD_W) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  WORD)   inline { STDLIB(WRITE_SDO_CMD_W) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  DINT)   inline { STDLIB(WRITE_SDO_CMD_DW) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  DWORD)  inline { STDLIB(WRITE_SDO_CMD_DW) }
overload DWORD(INT, DWORD, INT, INT, WORD, INT, INT, INT,  STRING) inline { STDLIB(WRITE_SDO_CMD_S) }

END_FUNCTION
