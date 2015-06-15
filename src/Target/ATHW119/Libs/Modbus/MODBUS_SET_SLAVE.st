(*[[
    UserText1= "mb_set_slave",
    UserHelpURL= "ATCMfile:MODBUS_SET_SLAVE.HTML",
]]*)
FUNCTION MODBUS_SET_SLAVE : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    slave : BYTE; (*[salve id]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_SLAVE)}
END_FUNCTION

