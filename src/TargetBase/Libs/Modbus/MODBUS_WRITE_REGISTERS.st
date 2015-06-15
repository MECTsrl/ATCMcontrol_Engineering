(*[[
    UserText1= "mb_write_registers",
    UserHelpURL= "ATCMfile:MODBUS_WRITE_REGISTERS.HTML",
]]*)
FUNCTION MODBUS_WRITE_REGISTERS : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    status : INT; (*[status to set to the bit]*)
	data : ARRAY[0..259] of UINT; (*[register to write]*)
END_VAR
INLINE {STDLIB(MODBUS_WRITE_REGISTERS)}
END_FUNCTION
