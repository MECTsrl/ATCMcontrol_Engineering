(*[[
    UserText1= "fn_16_mask_write_registers",
    UserHelpURL= "ATCMfile:MB_f16.HTML",
]]*)
FUNCTION MB_f16 : UINT
VAR_INPUT
	slave      : UINT; (*[slave number]*)
    start_addr : UINT; (*[start address]*)
(*
[0] = And Mask
[1] = Or Mask
*)
    data : ARRAY[0..2] of UINT; (*[0 = And Mask; 1 = Or Mask]*)
END_VAR
INLINE {STDLIB(MB_f16)}
END_FUNCTION
