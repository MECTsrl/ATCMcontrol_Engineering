FUNCTION ACOS : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(acos_lreal) }
overload real(real)   inline { STDLIB(acos_real) }

END_FUNCTION
