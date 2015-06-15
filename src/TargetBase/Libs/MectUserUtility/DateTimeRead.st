(*[[
    UserHelpURL= "ATCMfile:DateTimeRead.HTML",
]]*)
FUNCTION DateTimeRead : UINT
VAR_OUTPUT
	DateTime : ARRAY[0..6] of UINT; (*[date in  YEAR, MONTH, DAY, HOUR, MINUTE, SECONDS]*)
END_VAR
	INLINE {STDLIB(DateTimeRead)}
END_FUNCTION
