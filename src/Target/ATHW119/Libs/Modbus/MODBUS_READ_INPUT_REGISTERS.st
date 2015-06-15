(*[[
    UserText1= "mb_read_input_registers",
    UserHelpURL= "ATCMfile:MODBUS_READ_INPUT_REGISTERS.HTML",
]]*)
FUNCTION MODBUS_READ_INPUT_REGISTERS : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    nb_regs : UINT; (*[number of register to read]*)
END_VAR
VAR_OUTPUT
	data : ARRAY[0..259] of UINT; (*[registers read]*)
END_VAR
INLINE {STDLIB(MODBUS_READ_INPUT_REGISTERS)}
END_FUNCTION
