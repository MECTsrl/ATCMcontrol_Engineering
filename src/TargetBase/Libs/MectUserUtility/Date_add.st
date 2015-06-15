(*[[
    UserHelpURL= "ATCMfile:Date_add.HTML",
]]*)
FUNCTION Date_add : STRING
VAR_INPUT
	initDate  : STRING; (*[ starting date in format "DD-MM-YYYY" ]*)
	format    : STRING; (*[ "D" for days, "W" for weeks, "M" for months and "Y" for years ]*)
	increment : INT; (*[ value to add ]*)
END_VAR
VAR_OUTPUT
	finalDate     : STRING; (*[ starting date in format "DD-MM-YYYY" ]*)
END_VAR
	INLINE {STDLIB(Date_add)}
END_FUNCTION
