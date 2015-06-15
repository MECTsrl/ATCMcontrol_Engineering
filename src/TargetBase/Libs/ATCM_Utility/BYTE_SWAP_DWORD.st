(*[[
    UserURL= "ATCMfile:BYTE_SWAP_DWORD.htm"
]]*)

(* swaps from big to little endian byte order and vice versa *)

FUNCTION BYTE_SWAP_DWORD : DWORD
VAR_INPUT
    VALUE : DWORD;
END_VAR
    
inline { STDLIB ( byte_swap_dword ) }
    
END_FUNCTION
