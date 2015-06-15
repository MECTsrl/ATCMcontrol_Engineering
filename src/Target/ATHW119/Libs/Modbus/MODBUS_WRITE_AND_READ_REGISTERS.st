(*[[
    UserText1= "mb_write_and_read_registers",
    UserHelpURL= "ATCMfile:MODBUS_WRITE_AND_READ_REGISTERS.HTML",
]]*)
FUNCTION MODBUS_WRITE_AND_READ_REGISTERS : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    write_addr : DINT; (*[address from start to write]*)
	src : ARRAY[0..259] of UINT; (*[bits to write]*)
    write_nb : UINT; (*[number of register to write]*)
    read_addr : DINT; (*[address from start to read]*)
    read_nb : UINT; (*[number of register to read]*)
END_VAR
VAR_OUTPUT
	dest : ARRAY[0..259] of UINT; (*[bits to write]*)
END_VAR
INLINE {STDLIB(MODBUS_WRITE_AND_READ_REGISTERS)}
END_FUNCTION
