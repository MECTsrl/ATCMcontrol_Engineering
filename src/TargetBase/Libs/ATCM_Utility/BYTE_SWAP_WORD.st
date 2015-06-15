(*[[
    UserURL= "ATCMfile:BYTE_SWAP_WORD.htm"
]]*)

(* swaps from big to little endian byte order and vice versa *)

FUNCTION BYTE_SWAP_WORD : WORD
VAR_INPUT
    VALUE : WORD;
END_VAR
    
inline { STDLIB ( byte_swap_word ) }
    
END_FUNCTION
