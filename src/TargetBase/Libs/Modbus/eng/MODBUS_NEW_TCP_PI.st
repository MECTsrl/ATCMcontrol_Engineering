(*[[
    UserText1= "mb_new_tcp_pi",
    UserHelpURL= "ATCMfile:MODBUS_NEW_TCP_PI.HTML",
]]*)
FUNCTION MODBUS_NEW_TCP_PI : UINT
VAR_INPUT
    node : STRING; (*[node name]*)
    service : STRING; (*[service name]*)
END_VAR
INLINE {STDLIB(MODBUS_NEW_TCP_PI)}
END_FUNCTION
