FUNCTION EXP : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(exp_lreal) }
overload real(real)   inline { STDLIB(exp_real) }

END_FUNCTION
