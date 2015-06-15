(*[[
    UserURL= "ATCMfile:SocketConnect.htm"
]]*)
FUNCTION SocketConnect:DWORD

VAR_INPUT
   Socket: UDINT;
   SocketAddress: SocketAddressType;
END_VAR

inline {STDLIB(SocketConnect)}

END_FUNCTION
