FUNCTION MIN : ANY_ELEMENTARY
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
    and_bool
}
overload BYTE(BYTE,BYTE) inline
{
    %1
    %2
    lt_byte
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
    lt_word
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
    lt_dword
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
    lt_int
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
    lt_dint
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
    lt_real
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
    lt_lreal
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
    lt_string
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
:Lend%u
}

(*
TODO implement extensible funs in interpreter
overload BOOL(BOOL, BOOL, ... )                        inline{ STDLIB(min_bool) }
overload BYTE(BYTE,BYTE, ... )                         inline{ STDLIB(min_byte) } 
overload WORD(WORD,WORD, ... )                         inline{ STDLIB(min_word) }
overload DWORD(DWORD, DWORD, ... )                     inline{ STDLIB(min_dword) }

overload INT(INT,INT, ... )                            inline{ STDLIB(min_int) }
overload DINT(DINT,DINT, ... )                         inline{ STDLIB(min_dint) }

overload REAL(REAL,REAL, ... )                         inline{ STDLIB(min_real) }
overload LREAL(LREAL,LREAL, ... )                      inline{ STDLIB(min_lreal) }

overload TIME(TIME,TIME, ...)                          inline{ STDLIB(min_udint) }
overload STRING(STRING, STRING, ... )                  inline{ STDLIB(min_string) }
*)

END_FUNCTION
