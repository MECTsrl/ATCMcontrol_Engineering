(*[[
    UserURL= "SerialLineTransmit.htm",
]]*)

FUNCTION SerialLineTransmit:DWORD

VAR_INPUT

   ExecMode   :  INT;

END_VAR

VAR_OUTPUT

   ExecStatus :  DWORD;

END_VAR

VAR_INPUT

     Line     : INT;
     Length   : INT;
     
END_VAR

VAR_IN_OUT

     Buffer   : ANY_ELEMENTARY;

END_VAR

overload DWORD (INT, DWORD, INT, INT, BYTE)   inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, WORD)   inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, INT)    inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, REAL)   inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, DWORD)  inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, DINT)   inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, LREAL)  inline { STDLIB(SLCTRXB) }
overload DWORD (INT, DWORD, INT, INT, STRING) inline { STDLIB(SLCTRX)  }

END_FUNCTION
