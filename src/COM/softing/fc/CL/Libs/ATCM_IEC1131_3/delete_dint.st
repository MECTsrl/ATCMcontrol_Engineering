(*[[
    UserURL= "ATCMFile:DELETE_DINT.htm"
]]*)

FUNCTION DELETE_DINT : ANY_STRING
VAR_INPUT  
    IN : ANY_STRING; 
    L  : DINT;
    P  : DINT;
END_VAR

overload() inline{Utils.deleteString(%1,%2,%3)}

END_FUNCTION



