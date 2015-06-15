(*[[
    UserURL= "CpuUsage.htm"
]]*)

FUNCTION CpuUsage:DWORD

VAR_OUTPUT

   CpuUsePercentage:         UINT;

END_VAR

inline { STDLIB(CPUUSAGE) }

END_FUNCTION
