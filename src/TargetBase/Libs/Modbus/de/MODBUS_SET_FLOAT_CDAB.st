(*[[
    UserText1= "mb_set_float_cdab",
    UserHelpURL= "ATCMfile:MODBUS_SET_FLOAT_CDAB.HTML",
]]*)
FUNCTION MODBUS_SET_FLOAT_CDAB : UINT
VAR_INPUT
	dest : ARRAY[0..2] of UINT; (*[source bytes array]*)
	value : REAL; (*[float value]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_FLOAT_CDAB)}END_FUNCTION
