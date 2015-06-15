(*[[
    UserText1= "fn_14_read_general_reference",
    UserHelpURL= "ATCMfile:MB_f14.HTML",
]]*)
 (*
Return bits:
 [0] = the total number of valid entries in the following array location
 [1] = # of data N for group reference 1
 [2] = data0 for group reference 1
 [3] = data1 for group reference 1
 .....
 [K] = dataN for group reference 1
 [K+1] = # of data M for group reference 2
 [K+2] = data0 for group reference 2
 .....
 [H] = dataM for group reference 2
*)
FUNCTION MB_f14 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
(*
[0] = number of group of refrence to be read (from 1 ...to N)
[1] = extended Memory file number for group 1
[2] = starting register address for group 1
[3] = quantity of registers to be read for group 1
[4] = extended Memory file number for group 2
[5] = starting register address for group 2
[6] = quantity of registers to be read for group 2
...
*)
    query : ARRAY[0..257] of UINT; (*[query]*)
END_VAR
INLINE {STDLIB(MB_f14)}
END_FUNCTION
