
(*[[
    UserURL= "ATCM:LIMIT.htm"
]]*)

FUNCTION LIMIT : ANY_ELEMENTARY
VAR_INPUT 
    MN : ANY_ELEMENTARY; 
    IN : ANY_ELEMENTARY; 
    MX : ANY_ELEMENTARY; 
END_VAR

overload DINT|LINT|LREAL|TIME|ANY_DATE|BYTE|WORD(
         DINT|LINT|LREAL|TIME|ANY_DATE|BYTE|WORD, 
         DINT|LINT|LREAL|TIME|ANY_DATE|BYTE|WORD, 
         DINT|LINT|LREAL|TIME|ANY_DATE|BYTE|WORD)       inline{Math.min(Math.max(%2,%1),%3)}

overload DWORD(DWORD, DWORD, DWORD)                     inline{Utils.minDword(Utils.maxDword(%2,%1),%3)}
overload LWORD(LWORD, LWORD, LWORD)                     inline{Utils.minLword(Utils.maxLword(%2,%1),%3)}
overload ANY_STRING(ANY_STRING, ANY_STRING, ANY_STRING) inline{Utils.minString(Utils.maxString(%2,%1),%3)}
overload BOOL(BOOL, BOOL, BOOL)                         inline{(%2||%1) && %3}


END_FUNCTION
