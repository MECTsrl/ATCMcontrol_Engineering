(*[[
    UserURL= "ATCMfile:SocketBind.htm"
]]*)
FUNCTION SocketBind:DWORD

VAR_INPUT
   Socket:        UDINT;
   SocketAddress: SocketAddressType;
END_VAR

inline {STDLIB(SocketBind)}

END_FUNCTION
