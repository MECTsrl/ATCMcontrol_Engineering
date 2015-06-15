(*[[
    UserURL= "ATCMfile:LO_WORD.htm"
]]*)

(* extracts the low value WORD from a DWORD *)

FUNCTION LO_WORD : WORD
VAR_INPUT
    VALUE : DWORD;
END_VAR

inline { STDLIB ( lo_word ) }

END_FUNCTION
