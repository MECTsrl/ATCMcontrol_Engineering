(*[[
    UserHelpURL= "ATCMfile:DateTimeWrite.HTML",
]]*)
FUNCTION DateTimeWrite : UINT
VAR_INPUT
	DateTime : ARRAY[0..6] of UINT; (*[date in  YEAR, MONTH, DAY, HOUR, MINUTE, SECONDS]*)
END_VAR
	INLINE {STDLIB(DateTimeWrite)}
END_FUNCTION
