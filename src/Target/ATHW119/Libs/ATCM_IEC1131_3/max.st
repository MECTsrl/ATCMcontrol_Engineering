(*[[
]]*)

FUNCTION MAX : ANY_ELEMENTARY
VAR_INPUT 
    IN1 : ANY_ELEMENTARY; 
END_VAR
VAR_IN_EXT
    IN2 : ANY_ELEMENTARY;
END_VAR

overload BOOL(BOOL,BOOL) inline
{
    %1
    %2
    or_bool
}
overload BYTE(BYTE,BYTE) inline
{
    %1
    %2
    gt_byte
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload WORD(WORD,WORD) inline
{
    %1
    %2
    gt_word
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload DWORD(DWORD,DWORD) inline
{
    %1
    %2
    gt_dword
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload INT(INT,INT) inline
{
    %1
    %2
    gt_int
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload DINT|TIME(DINT|TIME,DINT|TIME) inline
{
    %1
    %2
    gt_dint
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload REAL(REAL,REAL) inline
{
    %1
    %2
    gt_real
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload LREAL(LREAL,LREAL) inline
{
    %1
    %2
    gt_lreal
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}
overload STRING(STRING,STRING) inline
{
    %1
    %2
    gt_string
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}

(*
TODO implement extensible funs in interpreter
overload BOOL(BOOL, BOOL, ... )                        inline{ STDLIB(max_bool) }
overload BYTE(BYTE,BYTE, ... )                         inline{ STDLIB(max_byte) } 
overload WORD(WORD,WORD, ... )                         inline{ STDLIB(max_word) }
overload DWORD(DWORD, DWORD, ... )                     inline{ STDLIB(max_dword) }

overload INT(INT,INT, ... )                            inline{ STDLIB(max_int) }
overload DINT(DINT,DINT, ... )                         inline{ STDLIB(max_dint) }

overload REAL(REAL,REAL, ... )                         inline{ STDLIB(max_real) }
overload LREAL(LREAL,LREAL, ... )                      inline{ STDLIB(max_lreal) }

overload TIME(TIME,TIME, ...)                          inline{ STDLIB(max_udint) }
overload STRING(STRING, STRING, ... )                  inline{ STDLIB(max_string) }
*)

END_FUNCTION
