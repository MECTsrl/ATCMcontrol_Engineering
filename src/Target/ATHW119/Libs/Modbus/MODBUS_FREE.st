(*[[
    UserText1= "modbus_free",
    UserHelpURL= "ATCMfile:MODBUS_FREE.HTML",
]]*)
FUNCTION MODBUS_FREE : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
END_VAR
INLINE {STDLIB(MODBUS_FREE)}
END_FUNCTION
