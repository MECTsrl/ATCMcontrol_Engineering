(*[[
    UserURL= "ATCMfile:DIVTIME.htm"
]]*)

FUNCTION DIVTIME : TIME
VAR_INPUT  
    IN1 : TIME;  
    IN2 : ANY_NUM; 
END_VAR

overload TIME(TIME,INT)
inline
{
    %1
    %2
    int_to_dint
    div_dint
}

overload TIME(TIME,DINT)
inline
{
    %1
    %2
    div_dint
}

overload TIME(TIME,REAL)
inline
{
    %1
    dint_to_real
    %2
    div_real
    real_to_dint
}

overload TIME(TIME,LREAL)
inline
{
    %1
    dint_to_lreal
    %2
    div_lreal
    lreal_to_dint
}

END_FUNCTION



