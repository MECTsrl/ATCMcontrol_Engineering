(*[[
    UserURL= "ATCMFile:LWORD_TO_WSTRING.htm"
]]*)

FUNCTION LWORD_TO_WSTRING : WSTRING
VAR_INPUT 
    IN : LWORD; 
END_VAR          

inline{Utils.lword_to_wstring(%1)}

END_FUNCTION
