(*[[
    UserURL= "ATCMFile:MAKE_DWORD.htm"
]]*)

(* combines two WORDs to a DWORD *)

FUNCTION MAKE_DWORD : DWORD
VAR_INPUT
    LO_WORD : WORD;
    HI_WORD : WORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    MAKE_DWORD := 0;
    NATIVE;
        _make_dword = (((int) _hi_word)<<16)|(0xffff&_lo_word);
    END_NATIVE;
END_FUNCTION
