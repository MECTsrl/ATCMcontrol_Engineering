(*[[
    UserText1= "mb_set_float_badc",
    UserHelpURL= "ATCMfile:MODBUS_SET_FLOAT_BADC.HTML",
]]*)
FUNCTION MODBUS_SET_FLOAT_BADC : UINT
VAR_INPUT
	dest : ARRAY[0..2] of UINT; (*[source bytes array]*)
	value : REAL; (*[float value]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_FLOAT_BADC)}END_FUNCTION
