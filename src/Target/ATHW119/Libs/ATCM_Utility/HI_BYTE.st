(*[[
    UserURL= "ATCMfile:HI_BYTE.htm"
]]*)

(* extracts the high value BYTE from a WORD *)

FUNCTION HI_BYTE : BYTE
VAR_INPUT
    VALUE : WORD;
END_VAR

inline { STDLIB ( hi_byte ) }

END_FUNCTION
