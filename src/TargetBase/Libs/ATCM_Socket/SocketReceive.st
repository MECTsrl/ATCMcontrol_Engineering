(*[[
    UserURL= "ATCMfile:SocketReceive.htm"
]]*)
FUNCTION SocketReceive:DWORD

VAR_INPUT
   Socket:    UDINT;
END_VAR

VAR_OUTPUT
   Buffer:          ARRAY OF BYTE;
   ReceivedDataLen: DINT;
END_VAR

inline {STDLIB(SocketReceive)}

END_FUNCTION
