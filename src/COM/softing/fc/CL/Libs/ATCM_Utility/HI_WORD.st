(*[[
    UserURL= "ATCMFile:HI_WORD.htm"
]]*)

(* extracts the high value WORD from a DWORD *)

FUNCTION HI_WORD : WORD
VAR_INPUT
    VALUE : DWORD;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    HI_WORD := 0;
    NATIVE;
        _hi_word = (_value&0xffff0000)>>>16;
    END_NATIVE;
END_FUNCTION
