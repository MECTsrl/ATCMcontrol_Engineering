(*[[
    UserURL= "ATCMFile:MID.htm"
]]*)

FUNCTION MID : ANY_STRING
VAR_INPUT
    IN : ANY_STRING;
    L : ANY_INT;
    P : ANY_INT;
END_VAR

overload STRING(STRING,INT,INT) 
    inline{ STDLIB(MidString_int) }

overload STRING(STRING,DINT,DINT)
    inline{
        %res 
        %2
        dint_to_int
        %3
        dint_to_int
	    %1			
        calfsys #MidString_int    
		leave  8        
        }

(*
overload STRING(STRING,LINT,LINT)
    inline{
        %res
        %2
        lint_to_int
        %3
        lint_to_int
	%1			
        calfsys #MidString_int
        leave 12
        }

overload STRING(STRING,SINT,SINT)
    inline{
        %res
        %2
        sint_to_int
        %3
        sint_to_int
	%1			
        calfsys #MidString_int
        leave 12
        }
*)

END_FUNCTION
