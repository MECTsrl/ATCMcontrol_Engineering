(*[[
    UserText1= "mb_write_register",
    UserHelpURL= "ATCMfile:MODBUS_WRITE_REGISTER.HTML",
]]*)
FUNCTION MODBUS_WRITE_REGISTER : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
	data : UINT; (*[register to write]*)
END_VAR
INLINE {STDLIB(MODBUS_WRITE_REGISTER)}
END_FUNCTION
