FUNCTION SEL : ANY

VAR_INPUT
    G   : BOOL; 
    IN0 : ANY; 
    IN1 : ANY; 
END_VAR

overload ANY_ELEMENTARY(BOOL,ANY_ELEMENTARY,ANY_ELEMENTARY) inline 
{
    %1
    jmpcn L1%u
    %3
    jmp L2%u
:L1%u
    %2
:L2%u
} 
END_FUNCTION
