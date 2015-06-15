FUNCTION COS : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(cos_lreal) }
overload real(real) inline   { STDLIB(cos_real) }

END_FUNCTION
