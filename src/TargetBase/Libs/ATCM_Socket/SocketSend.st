(*[[
    UserURL= "ATCMfile:SocketSend.htm"
]]*)
FUNCTION SocketSend:DWORD

VAR_INPUT
   Socket:  UDINT;
   Data:    ARRAY OF BYTE;
   DataLen: DINT;
   DestinationAddress: SocketAddressType;
END_VAR

VAR_OUTPUT
   SentDataLen:  DINT;
END_VAR

inline {STDLIB(SocketSend)}

END_FUNCTION
