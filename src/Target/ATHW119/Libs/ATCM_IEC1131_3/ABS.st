(*[[
    UserURL= "ATCMfile:ABS.htm"
]]*)

FUNCTION ABS : any_num
VAR_INPUT 
    IN : any_num; 
END_VAR


overload lreal(lreal) 
inline 
{   
	%1
	abs_lreal 
}

overload real(real)
inline
{
	%1
	abs_real
}

overload dint(dint)
inline
{
	%1
	abs_dint
}

overload int(int)
inline
{
	%1
	abs_int
}


END_FUNCTION
