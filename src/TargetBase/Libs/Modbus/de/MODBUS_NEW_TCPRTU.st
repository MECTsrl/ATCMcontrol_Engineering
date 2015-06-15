(*[[
    UserText1= "mb_new_tcprtu",
    UserHelpURL= "ATCMfile:MODBUS_NEW_TCPRTU.HTML",
]]*)
FUNCTION MODBUS_NEW_TCPRTU : UINT
VAR_INPUT
    ip_address : STRING; (*[ip address to connect]*)
    port : UINT; (*[port to connect]*)
END_VAR
INLINE {STDLIB(MODBUS_NEW_TCPRTU)}
END_FUNCTION
