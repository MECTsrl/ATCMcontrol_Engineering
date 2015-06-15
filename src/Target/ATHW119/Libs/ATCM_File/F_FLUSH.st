(*[[
    UserURL= "ATCMfile:F_FLUSH.htm"
]]*)

(* F_FLUSH
 * ----------------------------------------------------------------------------
 * This function causes any buffered output on all open files to be flushed.
 *
 * ret              Error code            
 *)
FUNCTION F_FLUSH : DINT

inline {STDLIB(f_flush)}

END_FUNCTION
