(*[[
    UserURL= "ATCMfile:SocketCreate.htm"
]]*)
FUNCTION SocketCreate:DWORD

VAR_INPUT
   AddressFamily:   DINT;
   ProtocolType:    DINT;
END_VAR

VAR_OUTPUT
   Socket:  UDINT;
END_VAR

inline {STDLIB(SocketCreate)}

END_FUNCTION
