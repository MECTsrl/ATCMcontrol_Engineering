(*[[
    UserHelpURL= "ATCMfile:Date_diff.HTML",
]]*)
FUNCTION Date_diff : UINT
VAR_INPUT
	initDate  : STRING; (*[ starting date in format "DD-MM-YYYY" ]*)
	finalDate : STRING; (*[ end date in format "DD-MM-YYYY" ]*)
	format    : STRING; (*[ "D" for days, "W" for weeks, "M" for months and "Y" for years ]*)
END_VAR
VAR_OUTPUT
	diff      : DINT; (*[ difference between start date and end date expressed in "format" ]*)
END_VAR
	INLINE {STDLIB(Date_diff)}
END_FUNCTION
