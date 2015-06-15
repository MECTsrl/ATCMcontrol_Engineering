FUNCTION ATAN : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(atan_lreal) }
overload real(real)   inline { STDLIB(atan_real) }

END_FUNCTION
