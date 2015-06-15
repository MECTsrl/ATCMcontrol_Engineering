(*[[
    UserURL= "ATCMfile:HI_WORD.htm"
]]*)

(* extracts the high value WORD from a DWORD *)

FUNCTION HI_WORD : WORD
VAR_INPUT
    VALUE : DWORD;
END_VAR

inline { STDLIB ( hi_word ) }

END_FUNCTION
