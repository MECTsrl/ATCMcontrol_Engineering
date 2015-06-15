
(*[[
    UserURL= "ATCM:MAKE_LWORD.htm"
]]*)

(* combines two DWORDs to a LWORD *)

FUNCTION MAKE_LWORD : LWORD
VAR_INPUT
    LO_DWORD : DWORD;
    HI_DWORD : DWORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    MAKE_LWORD := 0;
    NATIVE;
        _make_lword = (((long) _hi_dword)<<32)|(0xffffffffL&_lo_dword);
    END_NATIVE;
END_FUNCTION
