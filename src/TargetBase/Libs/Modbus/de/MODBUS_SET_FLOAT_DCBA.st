(*[[
    UserText1= "mb_set_float_dcba",
    UserHelpURL= "ATCMfile:MODBUS_SET_FLOAT_DCBA.HTML",
]]*)
FUNCTION MODBUS_SET_FLOAT_DCBA : UINT
VAR_INPUT
	dest : ARRAY[0..2] of UINT; (*[source bytes array]*)
	value : REAL; (*[float value]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_FLOAT_DCBA)}END_FUNCTION
