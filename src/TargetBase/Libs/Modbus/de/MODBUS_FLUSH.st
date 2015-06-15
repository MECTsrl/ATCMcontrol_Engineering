(*[[
    UserText1= "modbus_flush",
    UserHelpURL= "ATCMfile:MODBUS_FLUSH.HTML",
]]*)
FUNCTION MODBUS_FLUSH : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
END_VAR
INLINE {STDLIB(MODBUS_FLUSH)}
END_FUNCTION
