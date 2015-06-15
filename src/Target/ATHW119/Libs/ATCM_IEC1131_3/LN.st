FUNCTION LN : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(log_lreal) }
overload real(real)   inline { STDLIB(log_real) }

END_FUNCTION
