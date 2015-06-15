(*[[
    UserURL= "ATCMfile:LIMIT.htm"
]]*)

FUNCTION LIMIT : ANY_ELEMENTARY
VAR_INPUT 
    MN : ANY_ELEMENTARY; 
    IN : ANY_ELEMENTARY; 
    MX : ANY_ELEMENTARY; 
END_VAR

overload BOOL(BOOL,BOOL,BOOL) inline
{
    %2
    %1
    or_bool
    %3
    and_bool
}
overload BYTE(BYTE, BYTE, BYTE) inline
{
    %2
    %1
    lt_byte
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_byte
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}
overload WORD(WORD, WORD, WORD) inline
{
    %2
    %1
    lt_word
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_word
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}
overload DWORD(DWORD, DWORD, DWORD) inline
{
    %2
    %1
    lt_dword
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_dword
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}
overload INT(INT, INT, INT) inline
{
    %2
    %1
    lt_int
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_int
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}
overload DINT|TIME(DINT|TIME, DINT|TIME, DINT|TIME) inline
{
    %2
    %1
    lt_dint
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_dint
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}
overload REAL(REAL,REAL,REAL) inline
{
    %2
    %1
    lt_real
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_real
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}

overload LREAL(LREAL,LREAL,LREAL) inline
{
    %2
    %1
    lt_lreal
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_lreal
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}

overload STRING(STRING, STRING, STRING) inline
{
    %2
    %1
    lt_string
    jmpcn L1%u
    %1
    jmp Lend%u
:L1%u
    %2
    %3
    gt_string
    jmpcn L2%u
    %3
    jmp Lend%u
:L2%u
    %2
:Lend%u
}

END_FUNCTION
