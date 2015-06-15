(*[[
    UserURL= "ATCMfile:SocketEnableNonblocking.htm"
]]*)
FUNCTION SocketEnableNonblocking:DWORD

VAR_INPUT
   Socket:  UDINT;
   NonblockingMode : BOOL;
END_VAR

inline {STDLIB(SocketEnableNonblocking)}

END_FUNCTION
