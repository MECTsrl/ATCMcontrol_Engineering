(*[[
    UserURL= "ATCMfile:LO_BYTE.htm"
]]*)

(* extracts the low value BYTE from a WORD *)

FUNCTION LO_BYTE : BYTE
VAR_INPUT
    VALUE : WORD;
END_VAR

inline { STDLIB ( lo_byte ) }

END_FUNCTION
