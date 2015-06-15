(*[[
    UserText1= "fn_15_write_general_reference",
    UserHelpURL= "ATCMfile:MB_f15.HTML",
]]*)
FUNCTION MB_f15 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    count      : UINT; (*[counter]*)
(*
[0] = 0x06 the reference type fixed to 0x0006
[1] = Extended Memory file number (1 to 10, hex 0001 to 000A);
[2] = starting register address within the file
[3] = quantity of registers to be written
[4] =  data to be written
[5] =  data to be written
[6] =  data to be written
[7] =  data to be written
....
[n] = 0x06 the reference type fixed to 0x0006
[n+1] = Extended Memory file number (1 to 10, hex 0001 to 000A);
[n+2] = starting register address within the file
[n+3] = quantity of registers to be written
[n+4] =  data to be written
[n+5] =  data to be written
[n+6] =  data to be written
[n+7] =  data to be written
*)
    data : ARRAY[0..257] of UINT; (*[data to write]*)
END_VAR
INLINE {STDLIB(MB_f15)}
END_FUNCTION
