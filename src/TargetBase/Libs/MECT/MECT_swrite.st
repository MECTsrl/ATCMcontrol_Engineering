(*[[
    Alias="app_mect_float_stx",
    UserHelpURL= "ATCMfile:MECT_swrite.HTML",
]]*)
FUNCTION MECT_swrite : INT 
VAR_INPUT
	id      : INT;                   (*[identifyer]*)
	command : STRING[2];             (*[command]*)
	value   : REAL;                  (*[value to write]*)
	width   : DINT;                  (*[protocol digit width]*)
END_VAR
INLINE {STDLIB(MECT_swrite)}
END_FUNCTION
