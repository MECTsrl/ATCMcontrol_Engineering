(*[[
    UserText1= "mb_rtu_set_rts",
    UserHelpURL= "ATCMfile:MODBUS_RTU_SET_RTS.HTML",
]]*)
FUNCTION MODBUS_RTU_SET_RTS : UINT
VAR_INPUT
    rts : UINT; (*[rtu rts]*)
END_VAR
INLINE {STDLIB(MODBUS_RTU_SET_RTS)}
END_FUNCTION
