(*[[
    UserURL= "ATCMfile:SocketAccept.htm"
]]*)
FUNCTION SocketAccept:DWORD

VAR_INPUT
   ListeningSocket: UDINT;
END_VAR

VAR_OUTPUT
   AcceptSocket:  UDINT;
END_VAR

inline {STDLIB(SocketAccept)}

END_FUNCTION
