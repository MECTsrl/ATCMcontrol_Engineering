(*[[
    UserURL= "SerialLineReceive.htm",
]]*)

FUNCTION SerialLineReceive:DWORD

VAR_INPUT

   ExecMode   :  INT;

END_VAR

VAR_OUTPUT

   ExecStatus :  DWORD;

END_VAR

VAR_INPUT

     Line     : INT;

END_VAR

VAR_IN_OUT

     Length   : INT;
     Buffer   : ANY_ELEMENTARY;

END_VAR

overload DWORD (INT, DWORD, INT, INT, BYTE)   inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, WORD)   inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, INT)    inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, REAL)   inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, DWORD)  inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, DINT)   inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, LREAL)  inline { STDLIB(SLCRCXB) }
overload DWORD (INT, DWORD, INT, INT, STRING) inline { STDLIB(SLCRCX)  }

END_FUNCTION

