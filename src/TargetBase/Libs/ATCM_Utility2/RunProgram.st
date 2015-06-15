(*[[
    UserURL= "RunProgram.htm",
]]*)
FUNCTION RunProgram:DWORD
VAR_INPUT
   ExecMode:    INT;
   ProgramName: STRING;
   CommandLine: STRING;
   Timeout:     UDINT; (* expressed in ms *)
END_VAR

inline { STDLIB(RUNPROGRAM) }

END_FUNCTION
