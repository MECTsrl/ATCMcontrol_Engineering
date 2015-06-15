FUNCTION SQRT : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(sqrt_lreal) } 
overload real(real) inline   { STDLIB(sqrt_real) }

END_FUNCTION
