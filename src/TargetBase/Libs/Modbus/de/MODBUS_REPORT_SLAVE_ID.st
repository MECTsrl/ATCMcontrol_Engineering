(*[[
    UserText1= "mb_report_slave_id",
    UserHelpURL= "ATCMfile:MODBUS_REPORT_SLAVE_ID.HTML",
]]*)
FUNCTION MODBUS_REPORT_SLAVE_ID : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
END_VAR
VAR_OUTPUT
	dest : ARRAY[0..259] of BYTE; (*[slave id report]*)
END_VAR
INLINE {STDLIB(MODBUS_REPORT_SLAVE_ID)}
END_FUNCTION
