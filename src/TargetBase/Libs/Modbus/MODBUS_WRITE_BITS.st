(*[[
    UserText1= "mb_write_bits",
    UserHelpURL= "ATCMfile:MODBUS_WRITE_BITS.HTML",
]]*)
FUNCTION MODBUS_WRITE_BITS : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    nb_bits : UINT; (*[number of bits]*)
	data : ARRAY[0..259] of BYTE; (*[bits array write]*)
END_VAR
INLINE {STDLIB(MODBUS_WRITE_BITS)}
END_FUNCTION
