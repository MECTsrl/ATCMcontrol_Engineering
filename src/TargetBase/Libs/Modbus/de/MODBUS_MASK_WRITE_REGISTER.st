(*[[
    UserText1= "mb_mask_write_register",
    UserHelpURL= "ATCMfile:MODBUS_MASK_WRITE_REGISTER.HTML",
]]*)
FUNCTION MODBUS_MASK_WRITE_REGISTER : UINT
VAR_INPUT
    context : STRING; (*[context name (TCP, RTU, TCPRTU)]*)
    addr : UINT; (*[address from start to read]*)
    and_mask : UINT; (*[AND mask]*)
    or_mask : UINT; (*[OR mask]*)
END_VAR
INLINE {STDLIB(MODBUS_MASK_WRITE_REGISTER)}
END_FUNCTION
