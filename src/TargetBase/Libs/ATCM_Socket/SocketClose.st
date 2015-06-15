(*[[
    UserURL= "ATCMfile:SocketClose.htm"
]]*)
FUNCTION SocketClose:DWORD

VAR_INPUT
   Socket:  UDINT;
END_VAR

inline {STDLIB(SocketClose)}

END_FUNCTION
