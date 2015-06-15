(*[[
    UserURL= "ATCMFile:TIME_TO_WSTRING.htm"
]]*)

FUNCTION TIME_TO_WSTRING : WSTRING
VAR_INPUT 
   IN : TIME; 
END_VAR

inline{"t#"+java.lang.Long.toString(%1)+"ms"}

END_FUNCTION
