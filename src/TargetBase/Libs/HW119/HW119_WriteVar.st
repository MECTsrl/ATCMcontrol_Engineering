(*[[
    UserHelpURL= "ATCMfile:HW119_WriteVar.HTML",
    UserText1= "hw119_write_var",
]]*)
FUNCTION HW119_WriteVar : UINT
VAR_INPUT
    varname : STRING; (*[variable name to write]*)
    value : ANY; (*[value to write]*)
END_VAR

overload STRING(STRING, BOOL)   inline{ EXTLIB((hw119_write_var_bit) }
overload STRING(STRING, BYTE)   inline{ EXTLIB((hw119_write_var_byte) }
overload STRING(STRING, UINT)   inline{ EXTLIB((hw119_write_var_uint) }
overload STRING(STRING, INT)    inline{ EXTLIB((hw119_write_var_int) }
overload STRING(STRING, UDINT)  inline{ EXTLIB((hw119_write_var_udint) }
overload STRING(STRING, DINT)   inline{ EXTLIB((hw119_write_var_dint) }
overload STRING(STRING, REAL)   inline{ EXTLIB((hw119_write_var_real) }
END_FUNCTION
