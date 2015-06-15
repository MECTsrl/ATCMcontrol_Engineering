(*[[
    UserText1= "mb_read_input_bits",
    UserHelpURL= "ATCMfile:MODBUS_READ_BITS.HTML",
]]*)
FUNCTION MODBUS_READ_INPUT_BITS : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    nb_bits : UINT; (*[number of bits to read]*)
END_VAR
VAR_OUTPUT
	data : ARRAY[0..259] of BOOL; (*[bits read]*)
END_VAR
INLINE {STDLIB(MODBUS_READ_INPUT_BITS)}
END_FUNCTION
