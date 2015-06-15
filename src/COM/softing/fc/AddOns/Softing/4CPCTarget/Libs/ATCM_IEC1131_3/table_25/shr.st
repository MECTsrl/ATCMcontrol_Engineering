
(*[[
    UserURL= "ATCM:SHR.htm"
]]*)

FUNCTION SHR : ANY_BIT
VAR_INPUT 
    IN : ANY_BIT; 
    N  : ANY_INT; 
END_VAR

overload BOOL (BOOL, DINT)   inline{Utils.shift_bool(%1,%2)}
overload BYTE (BYTE, DINT)   inline{Utils.shr(%1,%2,8)}
overload WORD (WORD, DINT)   inline{Utils.shr(%1,%2,16)}
overload DWORD(DWORD,DINT)   inline{Utils.shr(%1,%2,32)}
overload LWORD(LWORD,DINT)   inline{Utils.shr_l(%1,%2)}

END_FUNCTION
