(*[[
    UserURL= "ATCMFile:BYTE_SWAP_DWORD.htm"
]]*)

(* swaps from big to little endian byte order and vice versa *)

FUNCTION BYTE_SWAP_DWORD : DWORD
VAR_INPUT
    VALUE : DWORD;
END_VAR
    
    (* dummy assignment to avoid no return walue warnings *)
    BYTE_SWAP_DWORD := 0;
    NATIVE
        _byte_swap_dword = 
            ((_value&(0xff<<0))<<24)|
            ((_value&(0xff<<8))<<8)|
            ((_value&(0xff<<16))>>>8)|
            ((_value&(0xff<<24))>>>24);
    END_NATIVE;
    
END_FUNCTION
