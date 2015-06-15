
(*[[
    UserURL= "ATCM:LO_BYTE.htm"
]]*)

(* extracts the low value BYTE from a WORD *)

FUNCTION LO_BYTE : BYTE
VAR_INPUT
    VALUE : WORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    LO_BYTE := 0;
    NATIVE;
        _lo_byte = _value&0xff;
    END_NATIVE;
END_FUNCTION
