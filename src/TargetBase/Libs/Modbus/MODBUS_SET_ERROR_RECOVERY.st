(*[[
  UserText1= "mb_set_error_recovery",
    UserHelpURL= "ATCMfile:MODBUS_SET_ERROR_RECOVERY.HTML",
]]*)
FUNCTION MODBUS_SET_ERROR_RECOVERY

VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    recovery_mode : UINT; (*[recovery mode: 0 - NONE , 1 - LINK, 2 - PROTOCOL]*)
END_VAR
INLINE {STDLIB(MODBUS_SET_ERROR_RECOVERY)}
END_FUNCTION
