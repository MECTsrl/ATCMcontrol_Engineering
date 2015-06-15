(*[[
    UserText1= "hw119_read_cross_table_record",
    UserHelpURL= "ATCMfile:HW119_ReadCrossTableRecord.HTML",
]]*)
FUNCTION HW119_ReadCrossTableRecord : UINT
VAR_INPUT
    Alarm : BOOL; (*[set true if it is an alarm or event]*) 
END_VAR
INLINE {STDLIB(HW119_ReadCrossTableRecord)}
END_FUNCTION
