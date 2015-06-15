(*[[
    UserText1= "mb_set_bits_from_byte",
    UserHelpURL= "ATCMfile:MODBUS_SET_BITS_FROM_BYTE.HTML",
]]*)
FUNCTION MODBUS_SET_BITS_FROM_BYTE : UINT
VAR_INPUT
    index : UINT; (*[index]*)
    value : BYTE; (*[source byte]*)
END_VAR
VAR_OUTPUT
	dest : ARRAY[0..259] of BYTE; (*[bits array to fill]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_BITS_FROM_BYTE)}
END_FUNCTION
