(*[[
    UserText1= "mb_get_response_timeout",
    UserHelpURL= "ATCMfile:MODBUS_GET_RESPONSE_TIMEOUT.HTML",
]]*)
FUNCTION MODBUS_GET_RESPONSE_TIMEOUT : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
END_VAR
VAR_OUTPUT
    timeout_ms : UINT; (*[timeout in milliseconds]*)
END_VAR
INLINE {STDLIB(MODBUS_GET_RESPONSE_TIMEOUT)}
END_FUNCTION
