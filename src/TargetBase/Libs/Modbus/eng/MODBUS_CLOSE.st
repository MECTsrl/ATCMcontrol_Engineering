(*[[
    UserText1= "mb_close",
    UserHelpURL= "ATCMfile:MODBUS_CLOSE.HTML",
]]*)
FUNCTION MODBUS_CLOSE : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
END_VAR
INLINE {STDLIB(MODBUS_CLOSE)}
END_FUNCTION
