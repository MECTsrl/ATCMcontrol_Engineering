(*[[
    UserURL= "ATCMFile:ROL.htm"
]]*)

FUNCTION ROL : ANY_BIT
VAR_INPUT 
    IN : ANY_BIT; 
    N  : ANY_INT; 
END_VAR

overload BOOL(BOOL,DINT)   inline{%1}
overload BYTE(BYTE,DINT)   inline{Utils.rol(%1,%2,8)}
overload WORD(WORD,DINT)   inline{Utils.rol(%1,%2,16)}
overload DWORD(DWORD,DINT) inline{Utils.rol(%1,%2,32)}
overload LWORD(LWORD,DINT) inline{Utils.rol_l(%1,%2)}

END_FUNCTION
