(*[[
    UserText1= "mb_get_float_cdab",
    UserHelpURL= "ATCMfile:MODBUS_GET_FLOAT_CDAB.HTML",
]]*)
FUNCTION MODBUS_GET_FLOAT_CDAB : UINT
VAR_INPUT
	src : ARRAY[0..2] of UINT; (*[source bytes array]*)
END_VAR
VAR_OUTPUT
	value : REAL; (*[float value]*)
END_VAR
INLINE {STDLIB(MODBUS_GET_FLOAT_CDAB)}
END_FUNCTION
