
(*[[
    UserURL= "ATCM:MAKE_WORD.htm"
]]*)

(* combines two BYTEs to a WORD *)

FUNCTION MAKE_WORD : WORD
VAR_INPUT
    LO_BYTE : BYTE;
    HI_BYTE : BYTE;
END_VAR
    (* dummy assignment to avoid no return walue warnings *)
    MAKE_WORD := 0;
    NATIVE;
        _make_word = (((short) _hi_byte)<<8)|(0xff&_lo_byte);
    END_NATIVE;
END_FUNCTION
