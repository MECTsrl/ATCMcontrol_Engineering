FUNCTION LOG : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(log10_lreal) }
overload real(real)   inline { STDLIB(log10_real) }

END_FUNCTION
