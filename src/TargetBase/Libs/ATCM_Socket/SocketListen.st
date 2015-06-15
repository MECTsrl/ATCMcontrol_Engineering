(*[[
    UserURL= "ATCMfile:SocketListen.htm"
]]*)
FUNCTION SocketListen:DWORD

VAR_INPUT
   Socket:  UDINT;
   Backlog: DINT;
END_VAR

inline {STDLIB(SocketListen)}

END_FUNCTION
