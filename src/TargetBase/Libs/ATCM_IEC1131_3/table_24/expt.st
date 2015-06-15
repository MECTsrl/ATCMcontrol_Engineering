(*[[
    UserURL= "ATCMfile:EXPT.htm"
]]*)

FUNCTION EXPT : ANY_REAL
VAR_INPUT 
    IN1: ANY_REAL; 
    IN2: ANY_NUM; 
END_VAR

overload REAL(REAL,DINT)    inline
{
    %1
    %2
    pow_real_dint
}
overload REAL(REAL,INT)     inline
{
    %1
    %2
    pow_real_int
}
overload LREAL(LREAL,DINT)  inline
{
    %1
    %2
    pow_lreal_dint
}
overload LREAL(LREAL,INT)   inline
{
    %1
    %2
    pow_lreal_int
}
overload REAL(REAL,REAL)    inline
{
    %1
    %2
    pow_real_real
}
overload LREAL(LREAL,LREAL) inline
{
    %1
    %2
    pow_lreal_lreal
}
overload REAL(REAL,LREAL)   inline
{
    %1
    real_to_lreal
    %2
    pow_lreal_lreal
    lreal_to_real
}
overload LREAL(LREAL,REAL)  inline
{
    %1
    %2
    real_to_lreal
    pow_lreal_lreal
}


END_FUNCTION
