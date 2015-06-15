
(*[[
    UserURL= "ATCM:HI_BYTE.htm"
]]*)

(* extracts the high value BYTE from a WORD *)

FUNCTION HI_BYTE : BYTE
VAR_INPUT
    VALUE : WORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    HI_BYTE := 0;
    NATIVE;
        _hi_byte = (_value&0xff00)>>>8;
    END_NATIVE;
END_FUNCTION
