FUNCTION ASIN : any_real
VAR_INPUT 
    IN : any_real; 
END_VAR

overload lreal(lreal) inline { STDLIB(asin_lreal) }
overload real(real) inline   { STDLIB(asin_real) }

END_FUNCTION
