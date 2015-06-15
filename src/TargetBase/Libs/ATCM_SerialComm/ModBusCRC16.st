(*[[
]]*)
FUNCTION ModBusCRC16:DWORD

VAR_INPUT
     Length   :   INT;
END_VAR

VAR_IN_OUT
     Buffer   :   ANY_ELEMENTARY;
END_VAR

VAR_OUTPUT
	getCRC	  :   WORD;
END_VAR

overload DWORD (INT, BYTE, WORD) inline { STDLIB(SLCRCModBus) }

END_FUNCTION
