(*[[
    UserURL= "ATCMFile:HI_DWORD.htm"
]]*)

(* extracts the high value DWORD from a LWORD *)

FUNCTION HI_DWORD : DWORD
VAR_INPUT
    VALUE : LWORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    HI_DWORD := 0;
    NATIVE;
        _hi_dword = (int) ((_value&0xffffffff00000000L)>>>32);
    END_NATIVE;
END_FUNCTION
