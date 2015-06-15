(*[[
    UserURL= "ATCMFile:INT_TO_BYTE.htm"
]]*)

FUNCTION INT_TO_BYTE : BYTE
VAR_INPUT 
    IN : INT; 
END_VAR

inline{VMOP(word_to_byte
; int_to_byte
; push word#16#ff
; and_word
)
      }

END_FUNCTION
