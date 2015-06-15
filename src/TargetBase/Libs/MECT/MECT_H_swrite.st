(*[[
    Alias="app_mect_hex_stx",
    UserHelpURL= "ATCMfile:MECT_H_swrite.HTML",
]]*)
FUNCTION MECT_H_swrite : INT
VAR_INPUT
	id      : INT;                   (*[identifyer]*)
	command : STRING[2];             (*[command]*)
	value   : INT;                   (*[value to write]*)
	width   : DINT;                  (*[protocol digit width]*)
END_VAR
INLINE {STDLIB(MECT_H_swrite)}
END_FUNCTION
