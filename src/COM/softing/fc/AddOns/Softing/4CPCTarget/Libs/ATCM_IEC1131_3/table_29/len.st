
FUNCTION LEN : ANY_INT
VAR_INPUT  
    IN : STRING;
END_VAR

overload INT(STRING) inline { STDLIB(strlen_int) }
overload DINT(STRING) inline
{
	%res
	%1
    calfsys #strlen_int %leave
    int_to_dint
}

END_FUNCTION



