(*[[
    UserText1= "mb_write_bit",
    UserHelpURL= "ATCMfile:MODBUS_WRITE_BIT.HTML",
]]*)
FUNCTION MODBUS_WRITE_BIT : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    status : INT; (*[status to set to the bit]*)
END_VAR
INLINE {STDLIB(MODBUS_WRITE_BIT)}
END_FUNCTION
