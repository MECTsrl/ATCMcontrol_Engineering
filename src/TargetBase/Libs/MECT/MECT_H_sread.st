(*[[
    Alias="app_mect_enq",
    UserHelpURL= "ATCMfile:MECT_H_sread.HTML",
]]*)
FUNCTION MECT_H_sread : INT
VAR_INPUT
	id      : INT;                   (*[identifyer]*)
	command : STRING[2];             (*[command]*)
	width   : DINT;                  (*[protocol digit width]*)
END_VAR
INLINE {STDLIB(MECT_H_sread)}
END_FUNCTION
