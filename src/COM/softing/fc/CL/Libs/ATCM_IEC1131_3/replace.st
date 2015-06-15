(*[[
    UserURL= "ATCMFile:REPLACE.htm"
]]*)

FUNCTION REPLACE : ANY_STRING
VAR_INPUT
    IN1 : ANY_STRING;
    IN2 : ANY_STRING;
    L : ANY_INT;
    P : ANY_INT;
END_VAR

overload STRING(STRING,STRING,INT,INT)
    inline{ STDLIB(ReplaceString_int) }    

overload STRING(STRING,STRING,DINT,DINT)
    inline{ 
        %res
        %3
        dint_to_int
        %4
        dint_to_int
        %2
        %1
        calfsys #ReplaceString_int   
		leave  12
        }    

(* 
overload STRING(STRING,STRING,LINT,LINT)
    inline{
        %res
        %3
        lint_to_int
        %4
        lint_to_int
        %2
        %1
        calfsys #ReplaceString_int
	leave 16
        }

overload STRING(STRING,STRING,SINT,SINT)
    inline{
        %res
        %3
        sint_to_int
        %4
        sint_to_int
        %2
        %1
        calfsys #ReplaceString_int
	leave 16
        }
*)

END_FUNCTION
