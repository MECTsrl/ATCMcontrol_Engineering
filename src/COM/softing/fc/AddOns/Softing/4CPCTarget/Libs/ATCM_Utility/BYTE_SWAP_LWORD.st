
(*[[
    UserURL= "ATCM:BYTE_SWAP_LWORD.htm"
]]*)

(* swaps from big to little endian byte order and vice versa *)

FUNCTION BYTE_SWAP_LWORD:LWORD
VAR_INPUT
    VALUE : LWORD;
END_VAR
    
    (* dummy assignment to avoid no return walue warnings *)
    BYTE_SWAP_LWORD := 0;
    NATIVE
        _byte_swap_lword = 
            ((_value&(0xffL<<0))<<56)|
            ((_value&(0xffL<<8))<<40)|
            ((_value&(0xffL<<16))<<24)|
            ((_value&(0xffL<<24))<<8)|
            ((_value&(0xffL<<32))>>>8)|
            ((_value&(0xffL<<40))>>>24)|
            ((_value&(0xffL<<48))>>>40)|
            ((_value&(0xffL<<56))>>>56);
    END_NATIVE;
    
END_FUNCTION
