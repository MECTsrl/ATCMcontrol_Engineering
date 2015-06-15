(*[[
    UserText1= "mb_new_tcp",
    UserHelpURL= "ATCMfile:MODBUS_NEW_TCP.HTML",
]]*)
FUNCTION MODBUS_NEW_TCP : UINT
VAR_INPUT
    ip_address : STRING; (*[ip address to connect]*)
    port : UINT; (*[port to connect]*)
END_VAR
INLINE {STDLIB(MODBUS_NEW_TCP)}
END_FUNCTION
