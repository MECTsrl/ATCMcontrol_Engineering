(*[[
    UserText1= "mb_get_byte_from_bits",
    UserHelpURL= "ATCMfile:MODBUS_GET_BYTE_FROM_BITS.HTML",
]]*)
FUNCTION MODBUS_GET_BYTE_FROM_BITS : UINT
VAR_INPUT
	src : ARRAY[0..259] of BYTE; (*[source bit array]*)
    index : UINT; (*[index]*)
    nb_bits : UINT; (*[number of bits]*)
END_VAR
VAR_OUTPUT
	dest : BYTE; (*[byte to fill]*)
END_VAR
INLINE {STDLIB(MODBUS_GET_BYTE_FROM_BITS)}
END_FUNCTION
