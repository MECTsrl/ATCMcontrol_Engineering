(*[[
    UserURL= "ATCMFile:FIND.htm"
]]*)

FUNCTION FIND : ANY_INT
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
END_VAR

overload INT(STRING,STRING)
    inline{ STDLIB(FindString_int) }

overload DINT(STRING,STRING)
    inline{ 
        %res
        dint_to_int
        %2
        %1
        calfsys #FindString_int   
		leave 8
		int_to_dint
        }

(*
overload LINT(STRING,STRING)
    inline{
        %res
        lint_to_int
        %2
        %1
        calfsys #FindString_int 
	leave 10		
        }

overload SINT(STRING,STRING)
    inline{
        %res
        sint_to_int
        %2
        %1
        calfsys #FindString_int 
	leave 10		
        }
*)

END_FUNCTION
