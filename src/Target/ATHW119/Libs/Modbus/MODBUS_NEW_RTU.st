(*[[
    UserText1= "mb_new_rtu",
    UserHelpURL= "ATCMfile:MODBUS_NEW_RTU.HTML",
]]*)
FUNCTION MODBUS_NEW_RTU : UINT
VAR_INPUT
    device : STRING; (*[device to connect]*)
    baud : UINT; (*[baud rate]*)
    parity : STRING; (*[parity bit]*)
    data_bit : UINT; (*[nb of data bit]*)
    stop_bit : UINT; (*[nb of stop bit]*)
END_VAR
INLINE {STDLIB(MODBUS_NEW_RTU)}
END_FUNCTION

