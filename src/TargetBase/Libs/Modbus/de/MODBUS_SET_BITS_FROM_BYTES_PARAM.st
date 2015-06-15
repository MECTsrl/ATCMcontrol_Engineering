(*[[
    UserText1= "mb_set_bits_from_bytes",
    UserHelpURL= "ATCMfile:MODBUS_SET_BITS_FROM_BYTES_PARAM.HTML",
]]*)
FUNCTION MODBUS_SET_BITS_FROM_BYTES_PARAM : UINT
VAR_INPUT
	tab_byte : ARRAY[0..260] of UINT; (*[source bytes array]*)
    index : UINT; (*[index]*)
    nb_bits : UINT; (*[number of bits]*)
END_VAR
VAR_OUTPUT
	dest : ARRAY[0..260] of UINT; (*[bits array to fill]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_BITS_FROM_BYTES_PARAM)}
END_FUNCTION
