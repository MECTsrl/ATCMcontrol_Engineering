
(*[[
    UserURL= "ATCM:LO_WORD.htm"
]]*)

(* extracts the low value WORD from a DWORD *)

FUNCTION LO_WORD : WORD
VAR_INPUT
    VALUE : DWORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    LO_WORD := 0;
    NATIVE;
        _lo_word = _value&0xffff;
    END_NATIVE;
END_FUNCTION
