(* 
 * Note: 
 *  LREAL_TO_LWORD(x) is the same as 
 *  LINT_TO_LWORD(LREAL_TO_LINT(x))
 *  
 *  e.g.  LREAL_TO_LWORD(-1.0E0) = 16#ffffffffffffffff;
 *)

FUNCTION LREAL_TO_LWORD : LWORD
VAR_INPUT 
    IN : LREAL; 
END_VAR

inline{VMOP(not_yet_implem)}

END_FUNCTION
