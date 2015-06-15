(*[[
    UserURL= "ATCMfile:SocketSelect.htm"
]]*)
FUNCTION SocketSelect:DWORD

VAR_INPUT
   Socket:    UDINT;
   Timeout:   INT;
END_VAR

VAR_OUTPUT
	Status:   UINT;
END_VAR

inline {STDLIB(SocketSelect)}

END_FUNCTION
