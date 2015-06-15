(*[[
    UserURL= "ATCMFile:BYTE_SWAP_WORD.htm"
]]*)

(* swaps from big to little endian byte order and vice versa *)

FUNCTION BYTE_SWAP_WORD : WORD
VAR_INPUT
    VALUE : WORD;
END_VAR
    
    (* dummy assignment to avoid no return walue warnings *)
    BYTE_SWAP_WORD := 0;
    NATIVE
        _byte_swap_word = 
            ((_value&(0xff<<0))<<8)|
            ((_value&(0xff<<8))>>>8);
    END_NATIVE;
    
END_FUNCTION
