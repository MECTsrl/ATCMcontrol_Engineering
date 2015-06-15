(*[[
    UserURL= "ATCMfile:TRUNC.htm"
]]*)

FUNCTION TRUNC : ANY_INT
VAR_INPUT 
    IN : ANY_REAL;
END_VAR

overload DINT        (REAL)  inline{ STDLIB(Trunc_real_dint) }
overload DINT        (LREAL) inline{ STDLIB(Trunc_lreal_dint) }
overload DINT to INT (REAL)  inline{ STDLIB(Trunc_real_dint) }
overload DINT to INT (LREAL) inline{ STDLIB(Trunc_lreal_dint) }


END_FUNCTION
