(*[[
    UserText1= "mb_set_float",
    UserHelpURL= "ATCMfile:MODBUS_SET_FLOAT.HTML",
]]*)
FUNCTION MODBUS_SET_FLOAT : UINT
VAR_INPUT
	dest : ARRAY[0..2] of UINT; (*[source bytes array]*)
	value : REAL; (*[float value]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_FLOAT)}
END_FUNCTION
