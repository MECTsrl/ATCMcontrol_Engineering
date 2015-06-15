(*[[
    UserURL= "ATCMfile:MAKE_DWORD.htm"
]]*)

(* combines two WORDs to a DWORD *)

FUNCTION MAKE_DWORD : DWORD
VAR_INPUT
    LO_WORD : WORD;
    HI_WORD : WORD;
END_VAR

inline { STDLIB ( make_dword ) }

END_FUNCTION
