
(*[[
    UserURL= "ATCM:LO_DWORD.htm"
]]*)

(* extracts the low value DWORD from a LWORD *)

FUNCTION LO_DWORD : DWORD
VAR_INPUT
    VALUE : LWORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    LO_DWORD := 0;
    NATIVE;
        _lo_dword = (int) (_value&0xffffffffL);
    END_NATIVE;
END_FUNCTION
