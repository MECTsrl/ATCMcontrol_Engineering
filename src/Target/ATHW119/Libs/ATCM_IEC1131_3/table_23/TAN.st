FUNCTION TAN : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(tan_lreal) }
overload real(real)   inline { STDLIB(tan_real) }

END_FUNCTION
