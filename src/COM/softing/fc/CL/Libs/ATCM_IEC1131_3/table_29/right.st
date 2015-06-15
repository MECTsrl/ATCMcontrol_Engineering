(*[[
    UserURL= "ATCMFile:RIGHT.htm"
]]*)

FUNCTION RIGHT : ANY_STRING
VAR_INPUT
    IN : ANY_STRING;
    L : ANY_INT;
END_VAR

overload STRING(STRING,INT) 
    inline{ STDLIB(RightString_int) }

overload STRING(STRING,DINT)
    inline{ 
        %res
        %2
        dint_to_int
        %1
        calfsys #RightString_int   
		leave  6
        }

(*
overload STRING(STRING,LINT)
    inline{ 
        %res
        %2
        lint_to_int
        %1
        calfsys #RightString_int
	leave 10
        }

overload STRING(STRING,SINT)
    inline{ 
        %res
        %2
        sint_to_int
        %1
        calfsys #RightString_int
	leave 10
        }
*)

END_FUNCTION
