FUNCTION SIN : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(sin_lreal) }
overload real(real)   inline { STDLIB(sin_real) }

END_FUNCTION
