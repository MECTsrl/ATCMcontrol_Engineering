(*[[
    UserURL= "ATCMfile:MAKE_WORD.htm"
]]*)

(* combines two BYTEs to a WORD *)

FUNCTION MAKE_WORD : WORD
VAR_INPUT
    LO_BYTE : BYTE;
    HI_BYTE : BYTE;
END_VAR

inline { STDLIB ( make_word ) }

END_FUNCTION
