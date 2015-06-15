(*[[
    UserURL= "ATCMFile:MIN.htm"
]]*)

FUNCTION MIN : ANY_ELEMENTARY
VAR_INPUT 
    IN1 : ANY_ELEMENTARY; 
END_VAR
VAR_IN_EXT
    IN2 : ANY_ELEMENTARY;
END_VAR


overload DINT|LINT|LREAL|BYTE|WORD|TIME|ANY_DATE(
         DINT|LINT|LREAL|BYTE|WORD|TIME|ANY_DATE, 
         DINT|LINT|LREAL|BYTE|WORD|TIME|ANY_DATE, ...) inline{@Math.min}
overload BOOL(BOOL,BOOL, ...)                          inline{@&&}
overload DWORD(DWORD, DWORD, ...)                      inline{@Utils.minDword}
overload LWORD(LWORD, LWORD, ...)                      inline{@Utils.minLword}
overload ANY_STRING(ANY_STRING, ANY_STRING, ...)       inline{@Utils.minString}

END_FUNCTION
