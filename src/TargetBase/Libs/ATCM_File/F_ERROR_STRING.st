(*[[
    UserURL= "ATCMfile:F_ERROR_STRING.htm"
]]*)

(* F_ERROR_STRING
 * ----------------------------------------------------------------------------
 * Retrieves the error text that corresponds to the error number.
 *
 * in   ERRCD       Error code          
 * out  
 * ret              Error string              
 *)
FUNCTION F_ERROR_STRING : STRING

VAR_INPUT
    ERRCD : DINT;
END_VAR

inline {STDLIB(f_error_string)}

END_FUNCTION
