
FUNCTION INSERT : ANY_STRING
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
    P : ANY_INT;
END_VAR

overload STRING(STRING,STRING,INT)
    inline{ STDLIB(InsertString_int)}

overload STRING(STRING,STRING,DINT)
    inline{
        %res
        %3
        dint_to_int   
        %2
        %1
        calfsys #InsertString_int   
		leave 10          
        }

(*
overload STRING(STRING,STRING,LINT)
    inline{
        %res
        %3
        lint_to_int
        %2
        %1
        calfsys #InsertString_int
	leave 14
        }

overload STRING(STRING,STRING,SINT)
    inline{
        %res
        %3
        sint_to_int
        %2
        %1
        calfsys #InsertString_int
	leave 14
        }
*)

END_FUNCTION
