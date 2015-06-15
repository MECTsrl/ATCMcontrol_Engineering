(*[[
    UserText1= "mb_set_response_timeout",
    UserHelpURL= "ATCMfile:MODBUS_SET_RESPONSE_TIMEOUT.HTML",
]]*)
FUNCTION MODBUS_SET_RESPONSE_TIMEOUT : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    timeout_ms : UINT; (*[timeout in milliseconds]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_RESPONSE_TIMEOUT)}
END_FUNCTION
