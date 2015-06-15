(* 
 * Note: 
 *  LWORD_TO_LREAL(x) is the same as 
 *  LINT_TO_LREAL(LWORD_TO_LINT(x))
 *  
 *  e.g.  LWORD_TO_LREAL(16#ffffffffffffffff) = -1.0E0
 *)
FUNCTION LWORD_TO_LREAL : LREAL
VAR_INPUT
    IN : LWORD;
END_VAR

inline{(double)%1}

END_FUNCTION
